# 컴파일러 설정
CC = gcc

# 운영 체제 확인
UNAME_S := $(shell uname -s)

# 공통 설정
CFLAGS = -Wall -g -I. -I/usr/include/json-c
LDFLAGS = -ljson-c -lsqlite3

# macOS 환경 설정
ifeq ($(UNAME_S),Darwin)
    CFLAGS = -I/opt/homebrew/Cellar/json-c/0.18/include
    LDFLAGS = -L/opt/homebrew/Cellar/json-c/0.18/lib -ljson-c -lsqlite3
endif

# 타겟 이름
TARGET = userServer

# 소스 파일 및 오브젝트 파일
SRCS = main.c user_server.c database.c
OBJS = $(SRCS:.c=.o)

# 기본 타겟
all: $(TARGET)

# 타겟 빌드 규칙
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 개별 오브젝트 파일 빌드 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 클린 규칙
clean:
	rm -f $(OBJS) $(TARGET)