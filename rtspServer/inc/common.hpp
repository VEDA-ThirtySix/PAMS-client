#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdint>

#define WIDTH 800
#define HEIGHT 600
#define FRAME_COUNT 30
#define VIDEODEV "/dev/video0"
#define OUTPUT_FILENAME "output.h264"

constexpr int64_t IP_V4_HEADER_SIZE = 20;
constexpr int64_t UDP_HEADER_SIZE = 8;
constexpr int64_t RTP_HEADER_SIZE = 12;
constexpr int64_t RTP_VERSION = 2;
constexpr int64_t RTP_PAYLOAD_TYPE_H264 = 96;
constexpr int64_t FU_SIZE = 2;

constexpr uint16_t SERVER_RTP_PORT = 12354;
constexpr uint16_t SERVER_RTCP_PORT = SERVER_RTP_PORT + 1;
constexpr uint16_t SERVER_RTSP_PORT = 7554;

constexpr int64_t MAX_UDP_PACKET_SIZE = 65535;
constexpr int64_t MAX_RTP_DATA_SIZE = MAX_UDP_PACKET_SIZE - IP_V4_HEADER_SIZE
                                      - UDP_HEADER_SIZE - RTP_HEADER_SIZE - FU_SIZE;
constexpr int64_t MAX_RTP_PACKET_LEN = MAX_RTP_DATA_SIZE + RTP_HEADER_SIZE + FU_SIZE;

//constexpr uint8_t NALU_F_MASK = 0x80;
constexpr uint8_t NALU_NRI_MASK = 0x60;
constexpr uint8_t NALU_F_NRI_MASK = 0xe0;
constexpr uint8_t NALU_TYPE_MASK = 0x1F;
constexpr uint8_t FU_S_MASK = 0x80;
constexpr uint8_t FU_E_MASK = 0x40;
constexpr uint8_t SET_FU_A_MASK = 0x1C;

#endif //COMMON_HPP