#include <cstring>
#include <iostream>

#include "request_handler.hpp"
#include "common.hpp"

void RequestHandler::replyCmd_OPTIONS(char *buffer, 
                                      const int64_t bufferLen,
                                      const int cseq)
{
    snprintf(buffer, bufferLen,
             "RTSP/1.0 200 OK\r\n"
             "CSeq: %d\r\n"
             "Public: OPTIONS, DESCRIBE, SETUP, PLAY\r\n\r\n",
             cseq);
}

void RequestHandler::replyCmd_SETUP(char *buffer,
                                    const int64_t bufferLen,
                                    const int cseq,
                                    const int clientRTP_Port,
                                    const int ssrcNum,
                                    const char *sessionID,
                                    const int timeout)
{
    snprintf(buffer, bufferLen,
             "RTSP/1.0 200 OK\r\n"
             "CSeq: %d\r\n"
             "Transport: RTP/AVP;unicast;client_port=%d-%d;server_port=%d-%d;ssrc=%d;mode=play\r\n"
             "Session: %s; timeout=%d\r\n\r\n",
             cseq, clientRTP_Port, clientRTP_Port + 1,
             SERVER_RTP_PORT, SERVER_RTCP_PORT, ssrcNum, sessionID, timeout);
}

void RequestHandler::replyCmd_PLAY(char *buffer,
                                   const int64_t bufferLen,
                                   const int cseq,
                                   const char *sessionID,
                                   const int timeout)
{
    snprintf(buffer, bufferLen,
             "RTSP/1.0 200 OK\r\n"
             "CSeq: %d\r\n"
             "Range: npt=0.000-\r\n"
             "Session: %s; timeout=%d\r\n\r\n",
             cseq, sessionID, timeout);
}

void RequestHandler::replyCmd_HEARTBEAT(char *buffer,
                                        const int64_t bufferLen,
                                        const int cseq,
                                        const char *sessionID)
{
    snprintf(buffer, bufferLen,
             "RTSP/1.0 200 OK\r\n"
             "CSeq: %d\r\n"
             "Range: npt=0.000-\r\n"
             "Heartbeat: %s; \r\n\r\n",
             cseq, sessionID);
}

void RequestHandler::replyCmd_DESCRIBE(char *buffer,
                                       const int64_t bufferLen,
                                       const int cseq,
                                       const char *url)
{
    char ip[100]{0};
    char sdp[500]{0};

    sscanf(url, "rtsp://%[^:]:", ip);
    snprintf(sdp, sizeof(sdp),
             "v=0\r\n"
             "o=- 9%ld 1 IN IP4 %s\r\n"
             "t=0 0\r\n"
             "a=control:*\r\n"
             "m=video 0 RTP/AVP 96\r\n"
             "a=rtpmap:96 H264/90000\r\n"
             "a=control:track0\r\n",
             time(nullptr), ip);

    snprintf(buffer, bufferLen,
             "RTSP/1.0 200 OK\r\n"
             "CSeq: %d\r\n"
             "Content-Base: %s\r\n"
             "Content-type: application/sdp\r\n"
             "Content-length: %ld\r\n\r\n%s",
             cseq, url, strlen(sdp), sdp);
}