#ifndef RTSP_CAM_HPP
#define RTSP_CAM_HPP

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>

#include "rtp_packet.hpp"
#include "h264_parser.hpp"
#include "common.hpp"

struct Buffer {
    void *start;
    size_t length;
};

extern Buffer *buffers;
extern unsigned int n_buffers;

struct YUV420Buffer {
    unsigned char *y_data[FRAME_COUNT];
    unsigned char *u_data[FRAME_COUNT];
    unsigned char *v_data[FRAME_COUNT];
    int count = 0;
    std::mutex lock;
};

struct YUV420Frame {
    std::vector<unsigned char> y_data;
    std::vector<unsigned char> u_data;
    std::vector<unsigned char> v_data;
};

// 전역 YUV420 버퍼
extern YUV420Buffer yuv420_buffer;

class RTSPCam
{
public:
    explicit RTSPCam();
    ~RTSPCam();

    void capture_frames();
    void Start(int ssrcNum, const char *sessionID, int timeout, float fps = 30);

    std::queue<YUV420Frame> frame_queue; // 캡처된 프레임 큐
    std::mutex queue_mutex;             // 큐 동기화를 위한 뮤텍스
    std::condition_variable queue_cond; // 큐 조건 변수
    
private:

    int pts = 1;
    
    int server_rtsp_sock_fd{-1};
    int server_rtp_sock_fd{-1};
    int server_rtcp_sock_fd{-1};

    int client_rtp_port{-1};
    int client_rtcp_port{-1};

    void init_device(int fd);
    void init_mmap(int fd);

    void serve_client(int clientfd, const sockaddr_in &cliAddr, int rtpFD,
                      int ssrcNum,  const char *sessionID,      int timeout, float fps);

    static int64_t push_stream(int sockfd, RtpPacket &rtpPack, const uint8_t *data, int64_t dataSize, const sockaddr *to, uint32_t timeStampStep);
};

#endif //RTSP_CAM_HPP