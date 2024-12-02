
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <json-c/json.h>

#define MAX_CLIENTS 10

// 구조체 정의
/* === HTTP Request Start ===
GET / HTTP/1.1
Host: 192.168.3.11:8080
Content-Type: application/json
Connection: Keep-Alive
Accept-Encoding: gzip, deflate
Accept-Language: ko-KR,en,*
User-Agent: Mozilla/5.0


=== HTTP Request End === */

struct http_request {
    // Request Line
    char method[16];
    char path[256];
    char version[16];
    
    // Headers
    char host[256];
    char content_type[128];
    char connection[64];
    char accept_encoding[128];
    char accept_language[128];
    char user_agent[256];
    
    // Body (POST 요청의 경우)
    char body[4096];
};


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



int init_server(int port);
void handle_client(int client_socket);
void parse_header(char* jsonBuffer, struct http_request *request);
int manage_request(char* jsonBuffer, ClientInfo* clientInfo, BasicInfo* basicInfo, TimeInfo* timeInfo);

void parse_init(char* jsonBuffer, ClientInfo* clientInfo);
void parse_user(char* jsonBuffer, BasicInfo* basicInfo);
void parse_clip(char* jsonBuffer, TimeInfo* timeInfo);

void send_response(struct http_response *response, int status_code, const char* content_type, const char* body);

#endif//SERVER_H