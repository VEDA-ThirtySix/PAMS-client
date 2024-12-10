#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "rtsp.hpp"
#include "rtp_packet.hpp"
#include "common.hpp"
#include "request_handler.hpp"
#include "utils.hpp"

RTSP::RTSP(const char *filename) : h264_file(filename)
{
}

RTSP::~RTSP()
{
	close(this->server_rtcp_sock_fd);
	close(this->server_rtp_sock_fd);
	close(this->server_rtsp_sock_fd);
}

void RTSP::Start(const int ssrcNum, const char *sessionID,
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

void RTSP::serve_client(int clientfd,          const sockaddr_in &cliAddr,
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

        char *bufferPtr = Utils::line_parser(recvBuf, line); // 첫 번째 줄 파싱
        if (bufferPtr == nullptr || sscanf(line, "%s %s %s", method, url, version) != 3) {
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

            while (true) {
                auto cur_frame = this->h264_file.get_next_frame();
                const auto ptr_cur_frame = cur_frame.first;
                const auto cur_frame_size = cur_frame.second;

                if (cur_frame_size < 0) {
                    fprintf(stderr, "RTSP::serve_client() H264::getOneFrame() failed\n");
                    break;
                } else if (!cur_frame_size) {
                    fprintf(stdout, "Finish serving the user\n");
                    return;
                }

                const int64_t start_code_len = H264Parser::is_start_code(ptr_cur_frame, 
                                               cur_frame_size, 4) ? 4 : 3;
                
                RTSP::push_stream(rtpFD,
                                  rtpPack,
                                  ptr_cur_frame + start_code_len,
                                  cur_frame_size - start_code_len,
                                  (sockaddr *)&clientSock,
                                  timeStampStep);
                usleep(sleepPeriod);
            }
            break;
        }
    }
    fprintf(stdout, "finish\n");
    close(clientfd);
}

int64_t RTSP::push_stream(int sockfd,          RtpPacket &rtpPack,
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