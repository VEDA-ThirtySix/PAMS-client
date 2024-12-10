#include "utils.hpp"
#include "rtp_packet.hpp"

#include <cstring>
#include <iostream>
#include <sys/ioctl.h>

char *Utils::line_parser(char *src, char *line)
{
    if (src == nullptr || line == nullptr){
        return nullptr;
    }

    while (*src != '\0' && *src != '\n'){
        *(line++) = *(src++);
    }

    if (*src == '\n'){
        *line = '\n';
        *(++line) = 0;
        return (src + 1);
    } else {
        *line = '\0';
        return nullptr;
    }
}

int Utils::Socket(int domain, int type, int protocol)
{
	int sockfd;
	const int optval = 1;

	if((sockfd = socket(domain, type, protocol)) < 0) {
        	fprintf(stderr, "RTSP::Socket() failed: %s\n", strerror(errno));
        	return sockfd;
    	}

    	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        	fprintf(stderr, "setsockopt() failed: %s\n", strerror(errno));
        	return -1;
    	}

    	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &MAX_UDP_PACKET_SIZE,
			sizeof(MAX_UDP_PACKET_SIZE)) < 0)
	{
		fprintf(stderr, "setsockopt() failed: %s\n", strerror(errno));
		return -1;
	}
	return sockfd;
}

bool Utils::Bind(int sockfd, const char *IP, const uint16_t port)
{
    sockaddr_in addr{};
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, IP, &addr.sin_addr);
    if (bind(sockfd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        fprintf(stderr, "bind() failed: %s\n", strerror(errno));
        return false;
    }
    return true;
}

bool Utils::Listen(int sockfd, const int64_t ListenQueue)
{
    if(listen(sockfd, ListenQueue) < 0) {
        fprintf(stderr, "listen() failed: %s\n", strerror(errno));
        return false;
    }
    return true;
}

void Utils::xioctl(int fd, int request, void *arg)
{
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while (r == -1 && (errno == EINTR || errno == EAGAIN));

	if (r == -1) {
        perror("xioctl: ");
        exit(EXIT_FAILURE);
	}
}