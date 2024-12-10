#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <cstdint>

class RequestHandler
{
public:
    static void replyCmd_OPTIONS  (char *buffer,      const int64_t bufferLen,
                                   const int cseq);

    static void replyCmd_SETUP    (char *buffer,      const int64_t bufferLen,
                                   const int cseq,    const int clientRTP_Port,
                                   const int ssrcNum, const char *sessionID,
                                   const int timeout);

    static void replyCmd_PLAY     (char *buffer,      const int64_t bufferLen,
                                   const int cseq,    const char *sessionID,
                                   const int timeout);

    static void replyCmd_HEARTBEAT(char *buffer,      const int64_t bufferLen,
                                   const int cseq,    const char *sessionID);
                                   
    static void replyCmd_DESCRIBE (char *buffer,      const int64_t bufferLen,
                                   const int cseq,    const char *url);
};

#endif //REQUEST_HANDLER_HPP