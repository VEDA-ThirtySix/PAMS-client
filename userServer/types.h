#ifndef TYPES_H
#define TYPES_H

#include <string.h>

struct http_response {
    char version[16];
    int status_code;
    char status_message[256];
    char content_type[128];
    size_t content_length;
    char connection[64];
    char body[4096];
};

typedef struct {
    char cliName[32];
    char ipAddr[32];
    char connectTime[16];
} ClientInfo;

typedef struct {
    char name[32];
    char plate[32];
    char home[16];
    char phone[16];
} BasicInfo;

typedef struct {
    char plate[32];
    char time[32];
    char type[16];
} TimeInfo;

#endif//TYPES_H