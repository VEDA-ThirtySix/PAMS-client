#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_thread(void* arg) {
    int client_socket = *((int*)arg);
    char buffer[1024];
    
    while(1) {
        pthread_mutex_lock(&socket_mutex);
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        pthread_mutex_unlock(&socket_mutex);
        
        if(bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("받은 메시지: %s\n", buffer);
        }
    }
    return NULL;
}

void* send_thread(void* arg) {
    int client_socket = *((int*)arg);
    char message[1024];
    
    while(1) {
        printf("보낼 메시지: ");
        fgets(message, sizeof(message), stdin);
        
        pthread_mutex_lock(&socket_mutex);
        write(client_socket, message, strlen(message));
        pthread_mutex_unlock(&socket_mutex);
    }
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);
    
    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    pthread_t recv_thread, snd_thread;
    int* client_sock = malloc(sizeof(int));
    *client_sock = client_socket;
    
    pthread_create(&recv_thread, NULL, receive_thread, client_sock);
    pthread_create(&snd_thread, NULL, send_thread, client_sock);
    
    pthread_join(recv_thread, NULL);
    pthread_join(snd_thread, NULL);
    
    close(client_socket);
    close(server_fd);
    return 0;
}