#include "rtp_header.hpp"
#include "common.hpp"

RtpHeader::RtpHeader(const uint8_t  _version,     const uint8_t  _padding,
                     const uint8_t  _extension,   const uint8_t  _csrcCount,
                     const uint8_t  _marker,      const uint8_t  _payloadType,
                     const uint16_t _seq,         const uint32_t _timestamp,
                     const uint32_t _ssrc)
{
    this->version = _version;
    this->padding = _padding;
    this->extension = _extension;
    this->csrcCount = _csrcCount;
    this->marker = _marker;
    this->payloadType = _payloadType;
    this->seq = htons(_seq);
    this->timestamp = htonl(_timestamp);
    this->ssrc = htonl(_ssrc);
}

RtpHeader::RtpHeader(const uint16_t _seq, const uint32_t _timestamp, const uint32_t _ssrc)
{
    this->version = RTP_VERSION;
    this->padding = 0;
    this->extension = 0;
    this->csrcCount = 0;
    this->marker = 0;
    this->payloadType = RTP_PAYLOAD_TYPE_H264;
    this->seq = htons(_seq);
    this->timestamp = htonl(_timestamp);
    this->ssrc = htonl(_ssrc);
}