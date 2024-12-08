#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;

void* receive_thread(void* arg) {
    int sock = *((int*)arg);
    char buffer[1024];
    
    while(1) {
        pthread_mutex_lock(&socket_mutex);
        int bytes_read = read(sock, buffer, sizeof(buffer));
        pthread_mutex_unlock(&socket_mutex);
        
        if(bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("\n서버로부터 받은 메시지: %s\n", buffer);
            printf("보낼 메시지: ");
            fflush(stdout);
        }
    }
    return NULL;
}

void* send_thread(void* arg) {
    int sock = *((int*)arg);
    char message[1024];
    
    while(1) {
        printf("보낼 메시지: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;
        
        if(strlen(message) > 0) {
            pthread_mutex_lock(&socket_mutex);
            write(sock, message, strlen(message));
            pthread_mutex_unlock(&socket_mutex);
        }
    }
    return NULL;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("연결 실패\n");
        return -1;
    }
    
    printf("서버에 연결되었습니다.\n");
    
    pthread_t recv_thread, snd_thread;
    int* sock_ptr = malloc(sizeof(int));
    *sock_ptr = sock;
    
    pthread_create(&recv_thread, NULL, receive_thread, sock_ptr);
    pthread_create(&snd_thread, NULL, send_thread, sock_ptr);
    
    pthread_join(recv_thread, NULL);
    pthread_join(snd_thread, NULL);
    
    close(sock);
    free(sock_ptr);
    return 0;
}