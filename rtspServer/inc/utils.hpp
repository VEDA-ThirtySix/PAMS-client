#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>

class Utils
{
public:
    static char *line_parser(char *src, char *line);
    static int  Socket(int domain, int type, int protocol = 0);
    static bool Bind(int sockfd, const char *IP, uint16_t port);
    static bool Listen(int sockfd, int64_t ListenQueue = 5);
    static void xioctl(int fd, int request, void *arg);
};

#endif //UTILS_HPP