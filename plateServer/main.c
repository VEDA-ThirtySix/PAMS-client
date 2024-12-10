#include "plate_server.h"
#define PORT 8088

volatile int current_client_socket = -1;

int main() {
    int server_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = init_server_plate(PORT);
    printf("main(O): Server started on port %d\n", PORT);
    
    char command[256];
    while(1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        current_client_socket = client_socket;
        printf("received client: %d\n", client_socket);
        
        while(1) {
            printf("Enter command (send to send plate data): ");
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = 0;

            if(strcmp(command, "send") == 0) {
                TimeInfo* timeInfo = get_timeInfo();
                char* encoded = encode_base64();
                char* json = build_json(timeInfo, encoded);
                
                if(json && encoded) {
                    send_plateData(client_socket, json);
                    free(encoded);
                }
                free(timeInfo);
            }
        }
    }
    
    close(server_fd);
    return 0;
}