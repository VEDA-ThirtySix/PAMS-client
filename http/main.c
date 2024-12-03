#include "server.h"
#define PORT 8080

#include <pthread.h>

struct thread_args {
    int client_socket;
};

void* command_handler(void* arg) {
    struct thread_args* args = (struct thread_args*)arg;
    int client_socket = args->client_socket;
    
    char command[256];
    while(1) {
        printf("Enter command (send to send plate data): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  // 개행문자 제거

        if(strcmp(command, "send") == 0) {
            TimeInfo* timeInfo = get_timeInfo();
            char* encoded = encode_base64();
            char* json = build_json(timeInfo, encoded);
            
            if(json && encoded) {
                send_plateData(client_socket, json);
                // json은 send_plateData에서 해제됨
                free(encoded);
            }
            free(timeInfo);
        }
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // 서버 초기화
    server_fd = init_server(PORT);
    printf("Server listening on port %d\n", PORT);
    
    while(1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        printf("New client connected\n");
        
        // 스레드 인자 구조체 생성
        struct thread_args* args = malloc(sizeof(struct thread_args));
        args->client_socket = client_socket;
        
        // 명령어 처리를 위한 스레드 생성
        pthread_t command_thread;
        if(pthread_create(&command_thread, NULL, command_handler, args) != 0) {
            perror("Failed to create command thread");
            free(args);
            close(client_socket);
            continue;
        }
        
        handle_client(client_socket);
    }
    
    return 0;
}