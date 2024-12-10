
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
#include <pthread.h>
#include <sqlite3.h>

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

/* 1. [INIT] INTIALIZE WITH HTTP PROTOCOL */
int init_server_user(int port);
void handle_client(int client_socket);

void parse_init(char* jsonBuffer, ClientInfo* clientInfo);
void parse_user(char* jsonBuffer, BasicInfo* basicInfo);
void parse_clip(char* jsonBuffer, TimeInfo *timeInfo);

void handle_clip();

void send_http_response(int client_socket, const char* json_response);  //사용중

/* DATABASE */
sqlite3* init_database(void);
int save_user_data(sqlite3 *db, BasicInfo *basicInfo);
int edit_user_data(sqlite3 *db, BasicInfo *basicInfo);
//int delete_user_data(sqlite3 *db, BasicInfo *basicInfo);
int check_plate_exists(sqlite3 *db, const char* plate);


#endif//SERVER_H