#include "user_server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define USER_PORT 8080

void* client_handler(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    handle_client(client_socket);
    close(client_socket);
    free(socket_desc);
    pthread_exit(NULL);
}

int main() {
    int server_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = init_server_user(USER_PORT);
    if (server_fd < 0) {
        perror("Server initialization failed");
        return 1;
    }
    printf("main(O): Server started on port %d\n", USER_PORT);

    while(1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        printf("Client connected: %d\n", client_socket);
        
        int* new_sock = malloc(sizeof(int));
        *new_sock = client_socket;
        
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, (void*)new_sock) < 0) {
            perror("Thread creation failed");
            free(new_sock);
            close(client_socket);
            continue;
        }
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}