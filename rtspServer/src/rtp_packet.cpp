#include "rtp_packet.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>

RtpPacket::RtpPacket(const RtpHeader &rtpHeader) : header(rtpHeader)
{
    this->cached_cur_seq = rtpHeader.get_seq();
    this->cached_cur_timestamp = rtpHeader.get_timestamp();
}

void RtpPacket::load_data(const uint8_t *data, const int64_t dataSize, const int64_t bias)
{
    memcpy(this->RTP_Payload + bias, data,
           std::min(dataSize, static_cast<int64_t>(sizeof(this->RTP_Payload) - bias)));
}

int64_t RtpPacket::rtp_sendto(int sockfd,                   const int64_t _bufferLen,
                              const int flags,              const sockaddr *to,
                              const uint32_t timeStampStep)
{
    auto sentBytes = sendto(sockfd, this, _bufferLen, flags, to, sizeof(sockaddr));
    this->set_header_seq(this->get_header_seq() + 1);
    this->set_header_timestamp(this->get_header_timestamp() + timeStampStep);
    return sentBytes;
}

inline void RtpPacket::set_header_seq(const uint32_t _seq)
{
    this->header.set_seq(_seq);
    this->cached_cur_seq = _seq;
}

inline void RtpPacket::set_header_timestamp(const uint32_t _newtimestamp)
{
    this->header.set_timestamp(_newtimestamp);
    this->cached_cur_timestamp = _newtimestamp;
}