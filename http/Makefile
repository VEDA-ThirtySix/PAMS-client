CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I./b64.c
LIBS = -ljson-c -lpthread

# 소스 파일 정의
SRC_DIR = .
B64_SRCS = b64.c/encode.c b64.c/decode.c b64.c/buffer.c
SRCS = main.c server.c $(B64_SRCS)

# 오브젝트 파일 생성
OBJS = $(SRCS:.c=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean