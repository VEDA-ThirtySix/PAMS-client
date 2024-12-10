CC = gcc

UNAME_S := $(shell uname -s)

# Linux 환경
ifeq ($(UNAME_S),Linux)
	CFLAGS = -Wall -g -I. -I/usr/include/json-c
	LDFLAGS = -ljson-c -lsqlite3
endif

# macOS 환경
ifeq ($(UNAME_S),Darwin)
	CFLAGS = -I/opt/homebrew/Cellar/json-c/0.18/include
	LDFLAGS = -L/opt/homebrew/Cellar/json-c/0.18/lib -ljson-c -lsqlite3
endif

all: userServer

userServer: main.o user_server.o database.o
	$(CC) $(CFLAGS) -o userServer main.o user_server.o database.o $(LDFLAGS)

main.o: main.c user_server.h
	$(CC) $(CFLAGS) -c main.c

user_server.o: user_server.c user_server.h
	$(CC) $(CFLAGS) -c user_server.c

database.o: database.c user_server.h
	$(CC) $(CFLAGS) -c database.c

clean:
	rm -f *.o userServer
