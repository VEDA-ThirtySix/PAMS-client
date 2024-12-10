#include "plate_server.h"
#include "errno.h"
#define MAX_CLIENTS 10

int init_server_plate(int port) {
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

unsigned char* get_packet(size_t* out_size) {
    FILE* fp = fopen("./images/image_2.jpg", "rb");
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
    //printf("get_packet: packet: %s\n", packet);

    *out_size = read_size;
    printf("");

    return packet;
}

/* char* encode_base64(void) {
    printf("\n########## encode_base64 ##########\n");
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
} */

char* encode_base64(void) {
    printf("\n########## encode_base64 ##########\n");
    size_t packet_size = 0;
    unsigned char* packet = get_packet(&packet_size);
    
    if(!packet || packet_size == 0) {
        printf("encode_base64: get_packet failed\n");
        return NULL;
    }
    
    char* encoded = b64_encode(packet, packet_size);
    free(packet);
    
    if(!encoded) {
        printf("encode_base64: b64_encode Failed\n");
        return NULL;
    }
    
    printf("encode_base64: Encoding Successful\n");
    return encoded;
}


char* build_json(const TimeInfo* timeInfo, const char* encoded) {
    struct json_object *response, *status, *data, *image;

    // 최상위 객체 생성
    response = json_object_new_object();
    if(!response) return NULL;
    status = json_object_new_object();
    data = json_object_new_object();
    image = json_object_new_object();

    // code(status) 객체 설정
    json_object_object_add(status, "code",
        json_object_new_string("success"));
    // message(status) 객체 설정
    json_object_object_add(status, "message",
        json_object_new_string("successful!"));

    // plate, time, type(data) 객체 설정
    json_object_object_add(data, "plate",
        json_object_new_string(timeInfo->plate));
    json_object_object_add(data, "time",
        json_object_new_string(timeInfo->time));
    json_object_object_add(data, "type",
        json_object_new_string(timeInfo->type));

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

    // response(root) 객체에 추가
    json_object_object_add(response, "status", status);
    json_object_object_add(response, "data", data);
    json_object_object_add(response, "image", image);
    
    // JSON 문자열로 변환
    const char* json_string = json_object_to_json_string(response);
    char* result = strdup(json_string);
    
    // JSON 전체 구조 출력
    printf("\n===== JSON Structure =====\n");
    printf("{\n"
           "    \"status\": {\n"
           "        \"code\": \"success\",\n"
           "        \"message\": \"successful!\"\n"
           "    },\n"
           "    \"data\": {\n"
           "        \"plate\": \"%s\",\n"
           "        \"time\": \"%s\",\n"
           "        \"type\": \"%s\"\n"
           "    },\n"
           "    \"image\": {\n"
           "        \"image\": \"[base64-encoded-data...]\"\n"
           "    }\n"
           "}\n", 
           timeInfo->plate, timeInfo->time, timeInfo->type);
    printf("========================\n\n");


    // 메모리 해제
    json_object_put(response);
    
    return result;
}

void send_plateData(int client_socket, char* json) {
    printf("DEBUG: send_plateData\n");

    // HTTP 응답 헤더 생성
    char header[1024];
    size_t json_len = strlen(json);
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        json_len);
    size_t header_len = strlen(header);
    write(client_socket, header, header_len);
    
    // JSON 데이터 전송
    size_t total_sent = 0;
    while (total_sent < json_len) {
        ssize_t bytes_written = write(client_socket, json + total_sent, json_len - total_sent);
        if (bytes_written <= 0) {
            printf("Error sending data: %s\n", strerror(errno));
            break;
        }
        total_sent += bytes_written;
        printf("Progress: %zu/%zu bytes sent\n", total_sent, json_len);
    }
    
    printf("Total sent: %zu bytes\n", total_sent + header_len);
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