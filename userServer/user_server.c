#include "user_server.h"  
#include "errno.h"
#include "sqlite3.h"

int init_server_user(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt =1;

    //Create Socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("init_server: socket failed");
        exit(EXIT_FAILURE);
    } else {
        printf("init_server: socket created\n");
    }

    //Socket Config
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("init_server: setsockopt failed");
        exit(EXIT_FAILURE);
    } else {
        printf("init_server: setsockopt success\n");
    }

    //init Address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //Bind Socket
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("init_server: bind failed");
        exit(EXIT_FAILURE);
    } else {
        printf("init_server: bind success\n");
    }

    //Listen
    if(listen(server_fd, MAX_CLIENTS) < 0) {
        perror("init_server: listen failed");
        exit(EXIT_FAILURE);
    } else {
        printf("init_server: listen success\n");
    }

    return server_fd;
}

void handle_client(int client_socket) {
    printf("handle_client(O): handle_client started\n");
    char buffer[4096] = {0};
    ssize_t bytes_read;
    size_t total_bytes = 0;
    char *body_start = NULL;

    printf("handle_client(O): waiting for data...\n");

    // 먼저 헤더를 읽음
    while ((bytes_read = read(client_socket, buffer + total_bytes, sizeof(buffer) - total_bytes)) > 0) {
        total_bytes += bytes_read;
        body_start = strstr(buffer, "\r\n\r\n");
        if (body_start) break;  // 헤더의 끝을 찾으면 중단
    }

    if (bytes_read <= 0) {
        printf("handle_client(X): Failed to read header\n");
        return;
    }

    // Content-Length 확인
    char *content_length_str = strstr(buffer, "Content-Length:");
    if (!content_length_str) {
        printf("handle_client(X): Content-Length not found\n");
        return;
    }
    int content_length = atoi(content_length_str + 16);
    printf("Content-Length: %d\n", content_length);

    // body 시작 위치 이동
    body_start += 4;
    
    // 현재까지 읽은 body 크기 계산
    size_t header_size = body_start - buffer;
    size_t current_body_size = total_bytes - header_size;
    
    // body 데이터를 완전히 읽을 때까지 계속 읽기
    while (current_body_size < content_length) {
        bytes_read = read(client_socket, 
                         buffer + total_bytes, 
                         sizeof(buffer) - total_bytes);
        if (bytes_read <= 0) {
            printf("handle_client(X): Failed to read body\n");
            return;
        }
        total_bytes += bytes_read;
        current_body_size += bytes_read;
    }

    printf("\n########## DATA RECEIVED ##########\n");
    printf("client socket: %d\n", client_socket);
    printf("Total bytes read: %zu\n", total_bytes);

    // JSON 파싱
    struct json_object *parsed_json;
    struct json_object *requestType_obj;
    struct json_object *reqType_obj;
    enum json_tokener_error error;

    parsed_json = json_tokener_parse_verbose(body_start, &error);
    if (!parsed_json) {
        printf("JSON parsing failed: %s\n", json_tokener_error_desc(error));
        return;
    }
    
    // requestType 처리
    char reqType[5] = {0};
    if(json_object_object_get_ex(parsed_json, "requestType", &requestType_obj) &&
       json_object_object_get_ex(requestType_obj, "reqType", &reqType_obj)) {
        
        strcpy(reqType, json_object_get_string(reqType_obj));
        printf("Received reqType: %s\n", reqType);

        ClientInfo clientInfo;
        BasicInfo basicInfo;
        TimeInfo timeInfo;

        sqlite3 *db = init_database();
        if(!db) {
            printf("Failed to initialize database\n");
            const char* json_response = "{\"status\":\"error\",\"message\":\"Database error\"}";
            send_http_response(client_socket, json_response);
            return;
        }

        if(strcmp(reqType, "init") == 0) {
            parse_init(body_start, &clientInfo);
            printf("Processed init request:\n");
            printf("ClientName  : %s\n", clientInfo.cliName);
            printf("IP Address  : %s\n", clientInfo.ipAddr);
            printf("Connect Time: %s\n", clientInfo.connectTime);

            if(save_user_data(db, &clientInfo)) {
                const char* json_response = "{\"status\":\"init_success\",\"message\":\"Client initialized\"}";
                send_http_response(client_socket, json_response);
            } else {
                const char* json_response = "{\"status\":\"error\",\"message\":\"Failed to save user data\"}";
                send_http_response(client_socket, json_response);
            }
        }   
        else if(strcmp(reqType, "user") == 0) {
            parse_user(body_start, &basicInfo);
            printf("Processed user request:\n");
            printf("Name : %s\n", basicInfo.name);
            printf("Plate: %s\n", basicInfo.plate);
            printf("Home : %s\n", basicInfo.home);
            printf("Phone: %s\n", basicInfo.phone);
            
            // 차량번호로 기존 데이터 확인
            if(check_plate_exists(db, basicInfo.plate)) {
                // 기존 데이터가 있으면 수정
                if(edit_user_data(db, &basicInfo)) {
                    const char* json_response = "{\"status\":\"success\",\"message\":\"User data updated\"}";
                    send_http_response(client_socket, json_response);
                } else {
                    const char* json_response = "{\"status\":\"error\",\"message\":\"Update failed\"}";
                    send_http_response(client_socket, json_response);
                }
            } else {
                // 새로운 데이터 저장
                if(save_user_data(db, &basicInfo)) {
                    const char* json_response = "{\"status\":\"success\",\"message\":\"User data saved\"}";
                    send_http_response(client_socket, json_response);
                } else {
                    const char* json_response = "{\"status\":\"error\",\"message\":\"Save failed\"}";
                    send_http_response(client_socket, json_response);
                }
            }
        } 
        else if(strcmp(reqType, "clip") == 0) {
            parse_clip(body_start, &timeInfo);
            printf("Processed clip request:\n");
            printf("Plate: %s\n", timeInfo.plate);
            printf("Time : %s\n", timeInfo.time);
            printf("Type : %s\n", timeInfo.type);
            
            /*
            if(save_clip_data(db, &timeInfo)) {
                const char* json_response = "{\"status\":\"clip_success\",\"message\":\"Clip saved\"}";
                send_http_response(client_socket, json_response);
            } else {
                const char* json_response = "{\"status\":\"error\",\"message\":\"Failed to save clip data\"}";
                send_http_response(client_socket, json_response);
            }*/
        }
        else {
            printf("Unknown request type: %s\n", reqType);
            
            const char* json_response = "{\"status\":\"error\",\"message\":\"Unknown request type\"}";
            send_http_response(client_socket, json_response);
        }

        sqlite3_close(db);
    }
    
    json_object_put(parsed_json);
    printf("handle_client(O): request processed successfully\n");
}

void send_http_response(int client_socket, const char* json_response) {
    char response[4096];
    int content_length = strlen(json_response);
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        //"Connection: keep-alive\r\n"
        "connection: close\r\n"
        "\r\n"
        "%s", content_length, json_response);
    write(client_socket, response, strlen(response));
}


void parse_init(char* jsonBuffer, ClientInfo *clientInfo) {
    //request Type
    struct json_object *parsed_json = json_tokener_parse(jsonBuffer);
    //INIT - ClientInfo
    struct json_object *clientInfo_obj;
    struct json_object *cliName_obj;
    struct json_object *ipAddr_obj;
    struct json_object *connectTime_obj;

    if(!parsed_json) {
        printf("JSON parsing failed\n");
        return;
    }

    if(json_object_object_get_ex(parsed_json, "clientInfo", &clientInfo_obj)) {
        if(json_object_object_get_ex(clientInfo_obj, "cliName", &cliName_obj)) {
            strcpy(clientInfo->cliName, json_object_get_string(cliName_obj));
        } 
        if(json_object_object_get_ex(clientInfo_obj, "ipAddr", &ipAddr_obj)) {
            strcpy(clientInfo->ipAddr, json_object_get_string(ipAddr_obj));       
        }
        if(json_object_object_get_ex(clientInfo_obj, "connectTime", &connectTime_obj)) {
            strcpy(clientInfo->connectTime, json_object_get_string(connectTime_obj));
        } 
    }
    
    json_object_put(parsed_json);   //delete parsed_json(memory leak)
}

void parse_user(char* jsonBuffer, BasicInfo *basicInfo) {
    //request Type
    struct json_object *parsed_json = json_tokener_parse(jsonBuffer);
    //USER - BasicInfo
    struct json_object *basicInfo_obj;
    struct json_object *name_obj;
    struct json_object *plate_obj;
    struct json_object *home_obj;
    struct json_object *phone_obj;

    if(!parsed_json) {
        printf("JSON parsing failed\n");
        return;
    }

    if(json_object_object_get_ex(parsed_json, "basicInfo", &basicInfo_obj)) {
        if(json_object_object_get_ex(basicInfo_obj, "name", &name_obj)) {
            strcpy(basicInfo->name, json_object_get_string(name_obj));
        } 
        if(json_object_object_get_ex(basicInfo_obj, "plate", &plate_obj)) {
            strcpy(basicInfo->plate, json_object_get_string(plate_obj));       
        }
        if(json_object_object_get_ex(basicInfo_obj, "home", &home_obj)) {
            strcpy(basicInfo->home, json_object_get_string(home_obj));
        }
        if(json_object_object_get_ex(basicInfo_obj, "phone", &phone_obj)) {
            strcpy(basicInfo->phone, json_object_get_string(phone_obj));
        } 
    }

    json_object_put(parsed_json);   //delete parsed_json(memory leak)
}

void parse_clip(char* jsonBuffer, TimeInfo *timeInfo) {
    //request Type
    struct json_object *parsed_json = json_tokener_parse(jsonBuffer);
    //CLIP - TimeInfo
    struct json_object *timeInfo_obj;
    struct json_object *plate_obj;
    struct json_object *time_obj;
    struct json_object *type_obj;

    if(!parsed_json) {
        printf("JSON parsing failed\n");
        return;
    }

    if(json_object_object_get_ex(parsed_json, "timeInfo", &timeInfo_obj)) {
        if(json_object_object_get_ex(timeInfo_obj, "plate", &plate_obj)) {
            strcpy(timeInfo->plate, json_object_get_string(plate_obj));
        } 
        if(json_object_object_get_ex(timeInfo_obj, "time", &time_obj)) {
            strcpy(timeInfo->time, json_object_get_string(time_obj));       
        }
        if(json_object_object_get_ex(timeInfo_obj, "type", &type_obj)) {
            strcpy(timeInfo->type, json_object_get_string(type_obj));
        } 
    }

    json_object_put(parsed_json);   //delete parsed_json(memory leak)
}



