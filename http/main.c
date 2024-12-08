#include "server.h"
<<<<<<< HEAD
#include <pthread.h>
#define PORT 8080


// 클라이언트 요청을 처리하는 스레드 함수
void* handle_client_thread(void* arg) {
    printf("handle_client_thread(O): handle_client_thread started\n");
    // void* 타입의 인자를 int*로 캐스팅하여 소켓 번호 추출
    int client_socket = *((int*)arg);
    // 동적 할당된 인자 메모리 해제
    free(arg);
    
    // 클라이언트 요청 처리 함수 호출
    handle_client(client_socket);
    return NULL;
}

// 사용자 명령어를 처리하는 스레드 함수
void* command_handler(void* arg) {
    printf("command_handler(O): command_handler started\n");
    // 클라이언트 소켓 번호 추출
    int client_socket = *((int*)arg);
    
    // 명령어를 저장할 버퍼
    char command[256];
    // 무한 루프로 계속해서 명령어 입력 받음
    while(1) {
        printf("Enter command (send to send plate data): ");
        // 표준 입력에서 명령어 읽기
        fgets(command, sizeof(command), stdin);
        // 개행 문자 제거
        command[strcspn(command, "\n")] = 0;

        // 'send' 명령어 처리
        if(strcmp(command, "send") == 0) {
            // 현재 시간 정보 가져오기
            TimeInfo* timeInfo = get_timeInfo();
            // 이미지 base64 인코딩
            char* encoded = encode_base64();
            // JSON 데이터 생성
            char* json = build_json(timeInfo, encoded);
            
            // JSON 생성 성공 시 데이터 전송
            if(json && encoded) {
                send_plateData(client_socket, json);
=======
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
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
                free(encoded);
            }
            free(timeInfo);
        }
    }
    return NULL;
}

<<<<<<< HEAD
/* int main() { 
    // 서버 소켓 관련 변수 선언
    int server_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 서버 초기화 (소켓 생성, 바인드, 리스닝)
    server_fd = init_server(PORT);
    printf("main(O): Server started on port %d\n", PORT);
    
    // 단일 클라이언트 연결 수락
    while(1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        printf("New client connected: %d\n", client_socket);

        // 클라이언트 처리 스레드 생성 준비
        pthread_t client_thread;
        int* client_arg = malloc(sizeof(int));
        *client_arg = client_socket;

        // 클라이언트 처리 스레드 생성
        if(pthread_create(&client_thread, NULL, handle_client_thread, client_arg) != 0) {
            perror("Failed to create client_thread");
            free(client_arg);
            close(client_socket);
            continue;
        }
        printf("main(O): client_thread created\n");
        //pthread_detach(client_thread);


        // 명령어 처리 스레드 생성 준비
        pthread_t command_thread;
        int* command_arg = malloc(sizeof(int));
        *command_arg = client_socket;
        
        // 명령어 처리 스레드 생성
        if(pthread_create(&command_thread, NULL, command_handler, command_arg) != 0) {
            perror("Failed to create command thread");
            free(command_arg);
            close(client_socket);
            return -1;
        }
        printf("main(O): command_thread created\n");

        // 두 스레드가 종료될 때까지 대기
        pthread_join(client_thread, NULL);
        pthread_join(command_thread, NULL);
        
        // 소켓 연결 종료 및 자원 정리
        close(client_socket);
    }
    close(server_fd);
    return 0;
}
*/


=======
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
int main() {
    int server_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
<<<<<<< HEAD

    server_fd = init_server(PORT);
    printf("main(O): Server started on port %d\n", PORT);
    
    // command_handler는 한 번만 생성
    pthread_t command_thread;
    int* command_arg = malloc(sizeof(int));
    *command_arg = -1;  // 초기값
    if(pthread_create(&command_thread, NULL, command_handler, command_arg) != 0) {
        perror("Failed to create command thread");
        free(command_arg);
        return -1;
    }
    pthread_detach(command_thread);
=======
    
    // 서버 초기화
    server_fd = init_server(PORT);
    printf("Server listening on port %d\n", PORT);
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
    
    while(1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
<<<<<<< HEAD
        printf("New client connected: %d\n", client_socket);

        pthread_t client_thread;
        int* client_arg = malloc(sizeof(int));
        *client_arg = client_socket;

        if(pthread_create(&client_thread, NULL, handle_client_thread, client_arg) != 0) {
            perror("Failed to create client_thread");
            free(client_arg);
=======
        printf("New client connected\n");
        
        // 스레드 인자 구조체 생성
        struct thread_args* args = malloc(sizeof(struct thread_args));
        args->client_socket = client_socket;
        
        // 명령어 처리를 위한 스레드 생성
        pthread_t command_thread;
        if(pthread_create(&command_thread, NULL, command_handler, args) != 0) {
            perror("Failed to create command thread");
            free(args);
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
            close(client_socket);
            continue;
        }
        
<<<<<<< HEAD
        // 클라이언트 스레드를 detach하여 자동 정리되도록 함
        pthread_detach(client_thread);
    }
    
    close(server_fd);
    return 0;
}

=======
        handle_client(client_socket);
    }
    
    return 0;
}
>>>>>>> 8ee41eb9980699f13714288471cc6f9edb973b87
