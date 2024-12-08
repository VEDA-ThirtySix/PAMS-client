
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
<<<<<<< HEAD
#include <pthread.h>
#include <sqlite3.h>
=======
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87

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

typedef struct {
    uint32_t size;
    uint16_t width;
    uint16_t height;
    unsigned char* data;
} ImagePacket;


/* 1. [INIT] INTIALIZE WITH HTTP PROTOCOL */
int init_server(int port);
void handle_client(int client_socket);
<<<<<<< HEAD
//void parse_header(char* jsonBuffer, struct http_request *request); 
//int manage_request(char* jsonBuffer, ClientInfo* clientInfo, BasicInfo* basicInfo, TimeInfo* timeInfo);

void parse_init(char* jsonBuffer, ClientInfo* clientInfo);
void parse_user(char* jsonBuffer, BasicInfo* basicInfo);
void parse_clip(char* jsonBuffer, TimeInfo *timeInfo);

/* 2. [USER] HANDLE USER REQUEST[POST] */
void handle_user();
=======
void parse_header(char* jsonBuffer, struct http_request *request);
int manage_request(char* jsonBuffer, ClientInfo* clientInfo, BasicInfo* basicInfo, TimeInfo* timeInfo);

void parse_init(char* jsonBuffer, ClientInfo* clientInfo);
void parse_clip(char* jsonBuffer, TimeInfo* timeInfo);

/* 2. [USER] HANDLE USER REQUEST[POST] */
void handle_user();
void parse_user(char* jsonBuffer, BasicInfo* basicInfo);
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
void saveDB_user();

/* 3. [CLIP] HANDLE USER REQUEST[GET] */
void handle_clip();
<<<<<<< HEAD
=======
void parse_clip(char* jsonBuffer, TimeInfo *timeInfo);
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
void stream_clip();

/* 4. [PLATE] SEND PLATE DATA */
void get_pakcet();
TimeInfo* get_timeInfo();
char* build_json(const TimeInfo* timeInfo, const char* encoded);
char* encode_base64(void); //packet -> base64
void send_plateData(int client_socket, char* json);
//Qt: parse_metadata();
//Qt: decode_base64();
//Qt: logManager->save_log(timeInfo, imgBuffer);

<<<<<<< HEAD
void send_http_response(int client_socket, const char* json_response);  //사용중
void send_response(struct http_response *response, int status_code, const char* content_type, const char* body);

/* DATABASE */
sqlite3* init_database(void);
int save_user_data(sqlite3 *db, BasicInfo *basicInfo);
int edit_user_data(sqlite3 *db, BasicInfo *basicInfo);
//int delete_user_data(sqlite3 *db, BasicInfo *basicInfo);
int check_plate_exists(sqlite3 *db, const char* plate);

//int save_clip_data(sqlite3 *db, TimeInfo *timeInfo);


=======
void send_response(struct http_response *response, int status_code, const char* content_type, const char* body);

>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
#endif//SERVER_H