#ifndef RTP_HEADER_HPP
#define RTP_HEADER_HPP

#include <cstddef>
#include <cstdint>
#include <arpa/inet.h>

#pragma pack(1)

class RtpHeader
{
public:
    RtpHeader(uint8_t _version,     uint8_t _padding,
              uint8_t _extension,   uint8_t _csrcCount,
              uint8_t _marker,      uint8_t _payloadType,
              uint16_t _seq,        uint32_t _timestamp,
              uint32_t _ssrc);

    RtpHeader(uint16_t _seq, uint32_t _timestamp, uint32_t _ssrc);
    RtpHeader(const RtpHeader &) = default;

    ~RtpHeader() = default;

    void set_timestamp(const uint32_t _newtimestamp);
    void set_ssrc(const uint32_t SSRC);
    void set_seq(const uint32_t _seq);

    void *get_header() const;
    uint32_t get_timestamp() const;
    uint32_t get_seq() const;
    
private:
    //byte 0
    uint8_t csrcCount : 4;
    uint8_t extension : 1;
    uint8_t padding : 1;
    uint8_t version : 2;
    //byte 1
    uint8_t payloadType : 7;
    uint8_t marker : 1;
    //byte 2, 3
    uint16_t seq;
    //byte 4-7
    uint32_t timestamp;
    //byte 8-11
    uint32_t ssrc;
};

inline void RtpHeader::set_timestamp(const uint32_t _newtimestamp)
{
    this->timestamp = htonl(_newtimestamp);
}

inline void RtpHeader::set_seq(const uint32_t _seq)
{
    this->seq = htons(_seq);
}

inline void RtpHeader::set_ssrc(const uint32_t SSRC)
{
    this->ssrc = htonl(SSRC);
}

inline void *RtpHeader::get_header() const
{
    return (void *)this;
}

inline uint32_t RtpHeader::get_timestamp() const
{
    return ntohl(this->timestamp);
}

inline uint32_t RtpHeader::get_seq() const
{
    return ntohs(this->seq);
}

#pragma pack()

#endif //RTP_HEADER_HPP
