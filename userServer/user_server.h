#ifndef SERVER_H
#define SERVER_H

#include "types.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <json-c/json.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sqlite3.h>

#define MAX_CLIENTS 10

sqlite3* init_database();
int save_user_data(sqlite3 *db, BasicInfo *basicInfo);
int edit_user_data(sqlite3 *db, BasicInfo *basicInfo);
//int delete_user_data(sqlite3 *db, BasicInfo *basicInfo);

int init_server_user(int port);
void handle_client(int client_socket);

void parse_init(char* jsonBuffer, ClientInfo* clientInfo);
void parse_user(char* jsonBuffer, BasicInfo* basicInfo);
void parse_clip(char* jsonBuffer, TimeInfo *timeInfo);
void convertToFilename(const char* timeStr, char* filename);
int check_plate_exists(sqlite3 *db, const char* plate);

void send_http_response(int client_socket, const char* json_response);  //사용중

#endif//SERVER_H