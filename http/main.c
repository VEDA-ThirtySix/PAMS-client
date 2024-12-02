#include "server.h"

#define PORT 8080

int main() {
    int server_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // 서버 초기화
    server_fd = init_server(PORT);
    printf("Server listening on port %d\n", PORT);
    
    while(1) {
        // 클라이언트 연결 수락
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        
        printf("New client connected\n");
        
        // 클라이언트 요청 처리
        handle_client(client_socket);
    }
    
    return 0;
}