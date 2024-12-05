#include "server.h"
#include "b64.c/b64.h"  //$ git clone https://github.com/jwerle/b64.c.git
#include "time.h"

int init_server(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt =1;

    //Create Socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //Socket Config
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    //init Address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //Bind Socket
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Listen
    if(listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void parse_header(char* jsonBuffer, struct http_request *request) {
    printf("DEBUG: parse_header\n");
    printf("=== HTTP Request Start ===\n%s\n=== HTTP Request End ===\n", jsonBuffer);
    
    char *line = strtok(jsonBuffer, "\r\n");
    char *body = NULL;

    //START LINE
    sscanf(line, "%s %s %s", request->method, request->path, request->version);
    printf("DEBUG: %s %s %s\n", request->method, request->path, request->version);
    
    //HEADER
    while((line = strtok(NULL, "\r\n")) != NULL) {
        if(strlen(line) == 0) {
            body = strtok(NULL, "");
            break;
        }
        if(strncmp(line, "Content-Type: ", 14) == 0) {
            strcpy(request->content_type, line + 14);
        }
    }

    // BODY
    if(body) {
        strcpy(request->body, body);
        printf("Body: %s\n", request->body);
    }
}

int manage_request(char* jsonBuffer, ClientInfo *clientInfo, BasicInfo* basicInfo, TimeInfo *timeInfo) {
    printf("DEBUG: manage_request\n");
    //request Type
    struct json_object *parsed_json;
    struct json_object *requestType_obj;
    struct json_object *reqType_obj;

    enum json_tokener_error error;
    parsed_json = json_tokener_parse_verbose(jsonBuffer, &error);
    if (!parsed_json) {
        printf("JSON parsing failed: %s\n", json_tokener_error_desc(error));
        return -1;
    }

    char reqType[5];
    if(json_object_object_get_ex(parsed_json, "requestType", &requestType_obj)) {
        if(json_object_object_get_ex(requestType_obj, "reqType", &reqType_obj)) {
            strcpy(reqType, json_object_get_string(reqType_obj));
            if(strcmp(reqType, "init") == 0) {
                printf("manage_request: ");
                for(int i=0; i<5; i++) printf("%c ", reqType[i]);
                printf("return: %d\n", 1);

                parse_init(jsonBuffer, clientInfo);
                return 1;
            } else if(strcmp(reqType, "user") == 0) {
                printf("manage_request: ");
                for(int i=0; i<5; i++) printf("%c ", reqType[i]);
                printf("return: %d\n", 2);

                parse_user(jsonBuffer, basicInfo);
                return 2;
            } else if(strcmp(reqType, "clip") == 0) {
                printf("manage_request: ");
                for(int i=0; i<5; i++) printf("%c ", reqType[i]);
                printf("return: %d\n", 3);

                parse_clip(jsonBuffer, timeInfo);
                return 3;
            }
        } 
    }
    return -1;
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

    printf("cliName: %s\n, ipAddr: %s\n, connectTime: %s\n",\
            clientInfo->cliName, clientInfo->ipAddr, clientInfo->connectTime);
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

    printf("name: %s\n, plate: %s\n, home: %s\n, phone: %s\n",  \
            basicInfo->name, basicInfo->plate, basicInfo->home, basicInfo->phone);
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

    printf("plate: %s\n, time: %s\n, type: %s\n", \
            timeInfo->plate, timeInfo->time, timeInfo->type);
    json_object_put(parsed_json);   //delete parsed_json(memory leak)
}

void handle_client(int client_socket) {
    printf("DEBUG: handle_client\n");
    char buffer[4096] = {0};
    struct http_request req;

    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }

    parse_header(buffer, &req);

    if (strcmp(req.method, "GET") == 0) {
        // GET 요청 처리
    } 
    else if (strcmp(req.method, "POST") == 0) {
        if (strstr(req.content_type, "application/json") != NULL) {
            ClientInfo clientInfo;
            BasicInfo basicInfo;
            TimeInfo timeInfo;
            
            int result = manage_request(req.body, &clientInfo, &basicInfo, &timeInfo);
            if (result > 0) {
                printf("Successful request\n");
            } else {
                printf("Invalid request\n");
            }
        }
    }
}

void send_response(struct http_response *response, int status_code,\
                   const char* content_type, const char* body) {
    strcpy(response->version, "HTTP/1.1");

    switch(status_code) {
        case 100:
            strcpy(response->status_message, "Continue");
            break;
        case 101:
            strcpy(response->status_message, "Switching Protocols");
            break;                                                          
        case 102:    
            strcpy(response->status_message, "Processing");                                                                    
            break;
        case 103:
            strcpy(response->status_message, "Early Hints");
            break;
        case 200:
            strcpy(response->status_message, "OK");
            break;
        case 201:
            strcpy(response->status_message, "Created");
            break;
        case 202:
            strcpy(response->status_message, "Accepted");
            break;
        case 204:
            strcpy(response->status_message, "No Content");
            break;
        case 301:
            strcpy(response->status_message, "Moved Permanently");
            break;
        case 302:
            strcpy(response->status_message, "Found");
            break;
        case 304:
            strcpy(response->status_message, "Not Modified");
            break;
        case 307:
            strcpy(response->status_message, "Temporary Redirect");
            break;
        case 308:
            strcpy(response->status_message, "Permanent Redirect");
            break;
        case 400:
            strcpy(response->status_message, "Bad Request");
            break;
        case 401:
            strcpy(response->status_message, "Unauthorized");
            break;
        case 403:
            strcpy(response->status_message, "Forbidden");
            break;      
        case 404:
            strcpy(response->status_message, "Not Found");
            break;
        case 500:
            strcpy(response->status_message, "Internal Server Error");
            break;
        case 503:
            strcpy(response->status_message, "Service Unavailable");
            break;
        case 504:
            strcpy(response->status_message, "Gateway Timeout");
            break;
        case 505:
            strcpy(response->status_message, "HTTP Version Not Supported");
            break;
        case 506:
            strcpy(response->status_message, "Variant Also Negotiates");
            break;
        case 507:
            strcpy(response->status_message, "Insufficient Storage");
            break;
        case 508:
            strcpy(response->status_message, "Loop Detected");
            break;
        case 509:
            strcpy(response->status_message, "Bandwidth Limit Exceeded");
            break;
        case 510:
            strcpy(response->status_message, "Not Extended");
            break;
        case 511:
            strcpy(response->status_message, "Network Authentication Required");
            break;
        default:
            strcpy(response->status_message, "Internal Server Error");
            break;
    }
    
    strcpy(response->content_type, content_type);
    response->content_length = strlen(body);
    strcpy(response->connection, "keep-alive");
    strcpy(response->body, body);
}

char* build_json(const TimeInfo* timeInfo, const char* encoded) {
    struct json_object *response, *status, *data, *image;

    // 최상위 객체 생성
    response = json_object_new_object();
    status = json_object_new_object();
    data = json_object_new_object();
    image = json_object_new_object();

    // code(status) 객체 설정
    json_object_object_add(status, "code",
        json_object_new_string("success"));
    // message(status) 객체 설정
    json_object_object_add(status, "message",
        json_object_new_string("successful!"));
    printf("STATUS: %s\n", json_object_to_json_string(status));

    // plate, time, type(data) 객체 설정
    json_object_object_add(data, "plate",
        json_object_new_string(timeInfo->plate));
    json_object_object_add(data, "time",
        json_object_new_string(timeInfo->time));
    json_object_object_add(data, "type",
        json_object_new_string(timeInfo->type));
    printf("DATA: %s\n", json_object_to_json_string(data));

    // image 객체 생성
    image = json_object_new_object();
    // base64(image) 객체 설정
    json_object_object_add(image, "image",
        json_object_new_string(encoded));
    // DEBUG: base64 출력(앞의 20자까지)   
    const char* encode_charArray = json_object_to_json_string(image);
    char debugArray[21];
    strncpy(debugArray, encode_charArray, 20);
    debugArray[20] = '\0';
    printf("IMAGE: %s\n", debugArray);

    // response(root) 객체에 추가
    json_object_object_add(response, "status", status);
    json_object_object_add(response, "data", data);
    json_object_object_add(response, "image", image);
    
    // JSON 문자열로 변환
    const char* json_string = json_object_to_json_string(response);
    char* result = strdup(json_string);
    
    // 메모리 해제
    json_object_put(response);
    
    return result;
}

unsigned char* get_packet(size_t* out_size) {
    FILE* fp = fopen("./images/image_1.jpg", "rb");
    if(!fp) {
        printf("get_packet: fopen failed\n");
        *out_size = 0;
        return NULL;
    }
    //get file size
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    //memory allocation
    unsigned char* packet = (unsigned char*)malloc(file_size);
    if(!packet) {
        printf("get_packet: malloc failed\n");
        fclose(fp);
        *out_size = 0;
        return NULL;
    }

    //read file
    size_t read_size = fread(packet, 1, file_size, fp);
    fclose(fp);

    //when file read fail
    if(read_size != file_size) {
        printf("get_packet: fread failed\n");
        free(packet);
        *out_size = 0;
        return NULL;
    }
    
    printf("get_packet: file_size: %zu\n", file_size);
    printf("get_packet: read_size: %zu\n", read_size);  //packet_size
    printf("get_packet: packet: %s\n", packet);

    *out_size = read_size;
    printf("");

    return packet;
}

char* encode_base64(void) {
    printf("DEBUG: encode_base64\n");
    size_t packet_size = 0;
    unsigned char* packet = get_packet(&packet_size);
    
    //
    size_t encode_size = (packet_size * 4 + 2) / 3;
    if(!packet || encode_size == 0) {
        printf("encode_base64: get_packet failed\n");
        return NULL;
    }
    printf("encode_base64: packet_size: %zu\n", packet_size);
    printf("encode_base64: encode_size: %zu\n", encode_size);

    char* encoded = (char*)malloc(encode_size + 1);
    encoded = b64_encode(packet, packet_size);
    
    if(!encoded) {
        printf("encode_base64: b64_encode Failed\n");
        free(packet);
        return NULL;
    } else {
        printf("encode_base64: Encoding Successful\n");
    }

    free(packet);
    return encoded;
}

void send_plateData(int client_socket, char* json) {
    printf("DEBUG: send_plateData\n");
    //printf("json: %s\n", json);
    
    //send json
    write(client_socket, json, strlen(json));
    printf("send_plateData: json sent\n");
    printf("send_plateData: client_socket: %d\n", client_socket);
    free(json);
}

TimeInfo* get_timeInfo(void) {
    time_t currentTime = time(NULL);
    struct tm* tm_info = localtime(&currentTime);
    char buffer[26];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    TimeInfo* timeInfo = malloc(sizeof(TimeInfo));
    strcpy(timeInfo->plate, "123가4568");
    strcpy(timeInfo->time, buffer);
    strcpy(timeInfo->type, "entry");

    return timeInfo;
}
