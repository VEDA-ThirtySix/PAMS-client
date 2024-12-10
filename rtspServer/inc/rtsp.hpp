#ifndef RTSP_HPP
#define RTSP_HPP

#include <cstddef>
#include <cstdint>

#include "rtp_packet.hpp"
#include "h264_parser.hpp"

class RTSP
{
public:
    H264Parser h264_file;

    explicit RTSP(const char *filename);
    ~RTSP();

    void Start(int ssrcNum, const char *sessionID,
                int timeout, float fps = 30);
private:    
    int server_rtsp_sock_fd{-1};
    int server_rtp_sock_fd{-1};
    int server_rtcp_sock_fd{-1};

    int client_rtp_port{-1};
    int client_rtcp_port{-1};

    void serve_client(int clientfd,          const sockaddr_in &cliAddr,
                      int rtpFD,             int ssrcNum,
                      const char *sessionID, int timeout, float fps);

    static int64_t push_stream(int sockfd,          RtpPacket &rtpPack,
                               const uint8_t *data, int64_t dataSize,
                               const sockaddr *to,  uint32_t timeStampStep);
};

#endif //RTSP_HPP