#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/error.h>
#include <libswscale/swscale.h>
}

#include "rtsp_cam.hpp"
#include "rtp_packet.hpp"
#include "common.hpp"
#include "request_handler.hpp"
#include "utils.hpp"

Buffer *buffers = nullptr;
unsigned int n_buffers = 0;
YUV420Buffer yuv420_buffer;

RTSPCam::RTSPCam()
{
}

RTSPCam::~RTSPCam()
{
	close(this->server_rtcp_sock_fd);
	close(this->server_rtp_sock_fd);
	close(this->server_rtsp_sock_fd);
}

void RTSPCam::init_device(int fd)
{
    v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    Utils::xioctl(fd, VIDIOC_S_FMT, &fmt);

    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV) {
        std::cerr << "Unsupported format." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (fmt.fmt.pix.width != WIDTH || fmt.fmt.pix.height != HEIGHT) {
        std::cerr << "Unsupported resolution." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Device Initialization Complete: "
              << WIDTH << "x" << HEIGHT
              << " YUYV" << std::endl;
}

void RTSPCam::init_mmap(int fd)
{
    v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));

    req.count = 6;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    Utils::xioctl(fd, VIDIOC_REQBUFS, &req);

    if (req.count < 2) {
        std::cerr << "At least 2 buffers required for memory mapping." << std::endl;
        exit(EXIT_FAILURE);
    }

    buffers = new Buffer[req.count];

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        Utils::xioctl(fd, VIDIOC_QUERYBUF, &buf);

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = mmap(nullptr, buf.length,
                                        PROT_READ | PROT_WRITE, MAP_SHARED,
                                        fd, buf.m.offset);

        if (buffers[n_buffers].start == MAP_FAILED) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "Memory Mapping Complete." << std::endl;
}

void RTSPCam::capture_frames() {
    int camfd = open(VIDEODEV, O_RDWR | O_NONBLOCK, 0);
    if (camfd == -1) {
        perror("Failed to open video device");
        return;
    }
    init_device(camfd);
    init_mmap(camfd);

    for (unsigned int i = 0; i < n_buffers; ++i) {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        Utils::xioctl(camfd, VIDIOC_QBUF, &buf);
    }

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    Utils::xioctl(camfd, VIDIOC_STREAMON, &type);

    std::cout << "Streaming started" << std::endl;

    SwsContext *img_convert_ctx = sws_getContext(
        WIDTH, HEIGHT, AV_PIX_FMT_YUYV422,
        WIDTH, HEIGHT, AV_PIX_FMT_YUV420P,
        SWS_BICUBIC, nullptr, nullptr, nullptr
    );
    
    if (!img_convert_ctx) {
        std::cerr << "Failed to initialize sws_getContext" << std::endl;
        close(camfd);
        return;
    }

    AVFrame *pFrameIn = av_frame_alloc();
    if (!pFrameIn) {
        std::cerr << "Failed to allocate input frame" << std::endl;
        sws_freeContext(img_convert_ctx);
        close(camfd);
        return;
    }

    pFrameIn->format = AV_PIX_FMT_YUYV422;
    pFrameIn->width = WIDTH;
    pFrameIn->height = HEIGHT;
    av_frame_get_buffer(pFrameIn, 1);

    AVFrame *pFrameOut = av_frame_alloc();
    if (!pFrameOut) {
        std::cerr << "Failed to allocate output frame" << std::endl;
        av_frame_free(&pFrameIn);
        sws_freeContext(img_convert_ctx);
        close(camfd);
        return;
    }

    pFrameOut->format = AV_PIX_FMT_YUV420P;
    pFrameOut->width = WIDTH;
    pFrameOut->height = HEIGHT;
    av_frame_get_buffer(pFrameOut, 1);

    while (true) {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        Utils::xioctl(camfd, VIDIOC_DQBUF, &buf);

        av_image_fill_arrays(pFrameIn->data, pFrameIn->linesize,
                             static_cast<uint8_t *>(buffers[buf.index].start), 
                             AV_PIX_FMT_YUYV422, WIDTH, HEIGHT, 1);

        sws_scale(img_convert_ctx, 
                  (const uint8_t * const *)pFrameIn->data, pFrameIn->linesize,
                  0, HEIGHT, pFrameOut->data, pFrameOut->linesize);

        YUV420Frame frame;
        frame.y_data.assign(pFrameOut->data[0], pFrameOut->data[0] + WIDTH * HEIGHT);
        frame.u_data.assign(pFrameOut->data[1], pFrameOut->data[1] + WIDTH * HEIGHT / 4);
        frame.v_data.assign(pFrameOut->data[2], pFrameOut->data[2] + WIDTH * HEIGHT / 4);

        // 프레임을 큐에 추가
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
			if (frame_queue.size() >= 4){
				frame_queue.pop();
			}
            frame_queue.push(frame);
        }
        queue_cond.notify_one(); // 대기 중인 컨슈머를 깨움

        Utils::xioctl(camfd, VIDIOC_QBUF, &buf);
    }

    av_frame_free(&pFrameIn);
    av_frame_free(&pFrameOut);
    sws_freeContext(img_convert_ctx);
    close(camfd);
}

void RTSPCam::Start(const int ssrcNum, const char *sessionID,
                    const int timeout, const float fps)
{
    this->server_rtsp_sock_fd = Utils::Socket(AF_INET, SOCK_STREAM);
    if (!Utils::Bind(this->server_rtsp_sock_fd, "0.0.0.0", SERVER_RTSP_PORT)) {
        fprintf(stderr, "failed to create RTSP socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    Utils::Listen(this->server_rtsp_sock_fd);
    

    this->server_rtp_sock_fd = Utils::Socket(AF_INET, SOCK_DGRAM);
    if (!Utils::Bind(this->server_rtp_sock_fd, "0.0.0.0", SERVER_RTP_PORT)) {
        fprintf(stderr, "failed to create RTP socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    this->server_rtcp_sock_fd = Utils::Socket(AF_INET, SOCK_DGRAM);
    if (!Utils::Bind(this->server_rtcp_sock_fd, "0.0.0.0", SERVER_RTCP_PORT)) {
        fprintf(stderr, "failed to create RTCP socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "rtsp://127.0.0.1:%d\n", SERVER_RTSP_PORT);

    //while (true) {
    sockaddr_in cliAddr{};
    memset(&cliAddr, 0, sizeof(cliAddr));
    socklen_t addrLen = sizeof(cliAddr);
    auto cli_sockfd = accept(this->server_rtsp_sock_fd,
                             reinterpret_cast<sockaddr *>(&cliAddr),
                             &addrLen);
    if (cli_sockfd < 0) {
        fprintf(stderr, "accept error(): %s\n", strerror(errno));
        //continue;
        return;
    }
    char IPv4[16]{0};
    fprintf(stdout,
            "Connection from %s:%d\n",
            inet_ntop(AF_INET, &cliAddr.sin_addr, IPv4, sizeof(IPv4)),
            ntohs(cliAddr.sin_port));

    this->serve_client(cli_sockfd,               cliAddr,
                       this->server_rtp_sock_fd, ssrcNum,
                       sessionID,                timeout, fps);
    //}
}

void RTSPCam::serve_client(int clientfd,          const sockaddr_in &cliAddr,
                           int rtpFD,             const int ssrcNum,
                           const char *sessionID, const int timeout,
                           const float fps)
{
    char method[10]{0};
    char url[100]{0};
    char version[10]{0};
    char line[500]{0};
    int cseq;
    int64_t heartbeatCount = 0;
    char recvBuf[1024]{0}, sendBuf[1024]{0};

    while (true)
    {
        auto recvLen = recv(clientfd, recvBuf, sizeof(recvBuf), 0);
        if (recvLen <= 0)
            break;
        recvBuf[recvLen] = 0;
        fprintf(stdout, "--------------- [C->S] --------------\n");
        fprintf(stdout, "%s", recvBuf);

        char *bufferPtr = Utils::line_parser(recvBuf, line);
        if (sscanf(line, "%s %s %s\r\n", method, url, version) != 3) {
            fprintf(stdout, "RTSP::line_parser() parse method error\n");
            break;
        }

        char *cseqPtr = strstr(recvBuf, "CSeq:");
        if (cseqPtr == nullptr || sscanf(cseqPtr, "CSeq: %d", &cseq) != 1) {
            fprintf(stdout, "RTSP::line_parser() parse seq error\n");
            break;
        }

        if (!strcmp(method, "SETUP")) {
            char *transPtr = strstr(recvBuf, "Transport:");
            if (transPtr == nullptr) {
                fprintf(stderr, "RTSP::line_parser() Transport parse error\n");
                break;
            }
            sscanf(transPtr,
                   "Transport: RTP/AVP/UDP;unicast;client_port=%d-%d\r\n",
                   &this->client_rtp_port,
                   &this->client_rtcp_port);
        }

        if (!strcmp(method, "OPTIONS")) {
            RequestHandler::replyCmd_OPTIONS(sendBuf, sizeof(sendBuf), cseq);
        } else if (!strcmp(method, "DESCRIBE")) {
            RequestHandler::replyCmd_DESCRIBE(sendBuf, sizeof(sendBuf), cseq, url);
        } else if (!strcmp(method, "SETUP")) {
            RequestHandler::replyCmd_SETUP(sendBuf, sizeof(sendBuf),
                                           cseq,    this->client_rtp_port,
                                           ssrcNum, sessionID,             timeout);
        } else if (!strcmp(method, "PLAY")) {
            RequestHandler::replyCmd_PLAY(sendBuf, sizeof(sendBuf),
                                          cseq,    sessionID,       timeout);
        } else {
            fprintf(stderr, "Parse method error\n");
            break;
        }

        fprintf(stdout, "--------------- [S->C] --------------\n");
        fprintf(stdout, "%s", sendBuf);
        if (send(clientfd, sendBuf, strlen(sendBuf), 0) < 0) {
            fprintf(stderr, "RTSP::serve_client() send() failed: %s\n", strerror(errno));
            break;
        }

        if (!strcmp(method, "PLAY")) {
            char IPv4[16]{0};
            inet_ntop(AF_INET, &cliAddr.sin_addr, IPv4, sizeof(IPv4));

            struct sockaddr_in clientSock{};
            bzero(&clientSock, sizeof(sockaddr_in));
            clientSock.sin_family = AF_INET;
            inet_pton(clientSock.sin_family, IPv4, &clientSock.sin_addr);
            clientSock.sin_port = htons(this->client_rtp_port);

            fprintf(stdout,
                    "start send stream to %s:%d\n",
                    IPv4, ntohs(clientSock.sin_port));

            const auto timeStampStep = uint32_t(90000 / fps);
            const auto sleepPeriod = uint32_t(1000 * 1000 / fps);
            RtpHeader rtpHeader(0, 0, ssrcNum);
            RtpPacket rtpPack{rtpHeader};

            const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
            if (!codec) {
                fprintf(stderr, "Cannot find H.264 Codec\n");
                return;
            }

            AVCodecContext *c = avcodec_alloc_context3(codec);
            if (!c) {
                fprintf(stderr, "Failed to allocate codec context.\n");
                return;
            }

            c->bit_rate = 400000;
            c->width = WIDTH;
            c->height = HEIGHT;
            c->time_base = {1, 30};
            c->framerate = {30, 1};
            c->gop_size = 30;
            c->max_b_frames = 0;
            c->pix_fmt = AV_PIX_FMT_YUV420P;

            if (avcodec_open2(c, codec, NULL) < 0) {
                fprintf(stderr, "Failed to open codec.\n");
                avcodec_free_context(&c);
                return;
            }

            FILE *f = fopen(OUTPUT_FILENAME, "wb");
            if (!f) {
                perror("Failed to open output file");
                avcodec_free_context(&c);
                return;
            }

            AVFrame *frame = av_frame_alloc();
            if (!frame) {
                fprintf(stderr, "Failed to allocate frame.\n");
                fclose(f);
                avcodec_free_context(&c);
                return;
            }

            frame->format = c->pix_fmt;
            frame->width = c->width;
            frame->height = c->height;

            if (av_image_alloc(frame->data,
                               frame->linesize,
                               c->width,
                               c->height,
                               c->pix_fmt, 32) < 0)
            {
                fprintf(stderr, "Failed to allocate image buffer.\n");
                av_frame_free(&frame);
                fclose(f);
                avcodec_free_context(&c);
                return;
            }

            AVPacket *pkt = av_packet_alloc();
            if (!pkt) {
                fprintf(stderr, "Failed to allocate packet.\n");
                av_freep(&frame->data[0]);
                av_frame_free(&frame);
                fclose(f);
                avcodec_free_context(&c);
                return;
            }

            printf("H.264 encoding & streaming started\n");

            while (true) {
                YUV420Frame capframe;
                // 큐에서 프레임을 가져옴
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    queue_cond.wait(lock, [this] { return !frame_queue.empty(); });
                    capframe = frame_queue.front();
                    frame_queue.pop();
                }


                // 프레임 데이터를 RTP 전송에 맞게 설정
                for (int y = 0; y < HEIGHT; y++) {
                    memcpy(frame->data[0] + y * frame->linesize[0],
                        capframe.y_data.data() + y * WIDTH, WIDTH);
                }

                int chroma_height = HEIGHT / 2;
                int chroma_width = WIDTH / 2;
                for (int y = 0; y < chroma_height; y++) {
                    memcpy(frame->data[1] + y * frame->linesize[1],
                        capframe.u_data.data() + y * chroma_width, chroma_width);
                    memcpy(frame->data[2] + y * frame->linesize[2],
                        capframe.v_data.data() + y * chroma_width, chroma_width);
                }

                // RTP 패킷 전송
                frame->pts = this->pts; // PTS 설정
                this->pts++;

                if (avcodec_send_frame(c, frame) < 0) {
                    fprintf(stderr, "Failed to send frame\n");
                }

                while (avcodec_receive_packet(c, pkt) == 0) {
                    fwrite(pkt->data, 1, pkt->size, f);

                    const int64_t start_code_len
                        = H264Parser::is_start_code(pkt->data,pkt->size, 4) ? 4 : 3;
                    RTSPCam::push_stream(rtpFD,
                                         rtpPack,
                                         pkt->data + start_code_len,
                                         pkt->size - start_code_len,
                                         (sockaddr *)&clientSock,
                                         timeStampStep);
                    av_packet_unref(pkt);
                }
                //std::this_thread::sleep_for(std::chrono::milliseconds(1));
                usleep(sleepPeriod);
            }
            fclose(f);
            av_packet_free(&pkt);
            av_freep(&frame->data[0]);
            av_frame_free(&frame);
            avcodec_free_context(&c);
            break;
        }
    }
    fprintf(stdout, "finish\n");
    close(clientfd);
}

int64_t RTSPCam::push_stream(int sockfd,          RtpPacket &rtpPack,
                             const uint8_t *data, const int64_t dataSize,
                             const sockaddr *to,  const uint32_t timeStampStep)
{
    const uint8_t naluHeader = data[0];
    if (dataSize <= MAX_RTP_DATA_SIZE) {
        rtpPack.load_data(data, dataSize);
        auto ret = rtpPack.rtp_sendto(sockfd,
                                      dataSize + RTP_HEADER_SIZE,
                                      0, to, timeStampStep);
        if (ret < 0)
            fprintf(stderr, "RTP_Packet::rtp_sendto() failed: %s\n", strerror(errno));
        return ret;
    }

    const int64_t packetNum = dataSize / MAX_RTP_DATA_SIZE;
    const int64_t remainPacketSize = dataSize % MAX_RTP_DATA_SIZE;
    int64_t pos = 1;
    int64_t sentBytes = 0;
    auto payload = rtpPack.get_payload();
    for (int64_t i = 0; i < packetNum; i++) {
        rtpPack.load_data(data + pos, MAX_RTP_DATA_SIZE, FU_SIZE);
        payload[0] = (naluHeader & NALU_F_NRI_MASK) | SET_FU_A_MASK;
        payload[1] = naluHeader & NALU_TYPE_MASK;
        if (!i)
            payload[1] |= FU_S_MASK;
        else if (i == packetNum - 1 && remainPacketSize == 0)
            payload[1] |= FU_E_MASK;

        auto ret = rtpPack.rtp_sendto(sockfd, MAX_RTP_PACKET_LEN, 0, to, timeStampStep);
        if (ret < 0) {
            fprintf(stderr, "RTP_Packet::rtp_sendto() failed: %s\n", strerror(errno));
            return -1;
        }
        sentBytes += ret;
        pos += MAX_RTP_DATA_SIZE;
    }
    if (remainPacketSize > 0) {
        rtpPack.load_data(data + pos, remainPacketSize, FU_SIZE);
        payload[0] = (naluHeader & NALU_F_NRI_MASK) | SET_FU_A_MASK;
        payload[1] = (naluHeader & NALU_TYPE_MASK) | FU_E_MASK;
        auto ret = rtpPack.rtp_sendto(sockfd,
                                      remainPacketSize + RTP_HEADER_SIZE + FU_SIZE,
                                      0, to, timeStampStep);
        if (ret < 0) {
            fprintf(stderr, "RTP_Packet::rtp_sendto() failed: %s\n", strerror(errno));
            return -1;
        }
        sentBytes += ret;
    }
    return sentBytes;
}
