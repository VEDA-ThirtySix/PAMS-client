#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <json-c/json.h>
#include <sys/time.h>
#include <time.h>
#include "b64.c/b64.h"  //$ git clone https://github.com/jwerle/b64.c.git
#include "time.h"

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

/* 4. [PLATE] SEND PLATE DATA */
void get_pakcet();
TimeInfo* get_timeInfo();
char* build_json(const TimeInfo* timeInfo, const char* encoded);
char* encode_base64(void); //packet -> base64
void send_plateData(int client_socket, char* json);
TimeInfo* get_timeInfo(void);
int init_server_plate(int port);
