CC = gcc

# OS 감지
UNAME_S := $(shell uname -s)

# Linux 환경
ifeq ($(UNAME_S),Linux)
    CFLAGS = -Wall -g -I. -I/usr/include/json-c -Ib64.c
    LDFLAGS = -ljson-c
endif

# macOS 환경
ifeq ($(UNAME_S),Darwin)
    CFLAGS = -I/opt/homebrew/Cellar/json-c/0.18/include -Ib64.c
    LDFLAGS = -L/opt/homebrew/Cellar/json-c/0.18/lib -ljson-c
endif

all: plateServer

plateServer: main.o plate_server.o b64.c/encode.o b64.c/decode.o b64.c/buffer.o
	$(CC) $(CFLAGS) -o plateServer main.o plate_server.o b64.c/encode.o b64.c/decode.o b64.c/buffer.o $(LDFLAGS)

main.o: main.c plate_server.h
	$(CC) $(CFLAGS) -c main.c

plate_server.o: plate_server.c plate_server.h
	$(CC) $(CFLAGS) -c plate_server.c

b64.c/encode.o: b64.c/encode.c b64.c/b64.h
	$(CC) $(CFLAGS) -c b64.c/encode.c -o b64.c/encode.o

b64.c/decode.o: b64.c/decode.c b64.c/b64.h
	$(CC) $(CFLAGS) -c b64.c/decode.c -o b64.c/decode.o

b64.c/buffer.o: b64.c/buffer.c b64.c/b64.h
	$(CC) $(CFLAGS) -c b64.c/buffer.c -o b64.c/buffer.o

clean:
	rm -f *.o plateServer b64.c/*.o
