CC = gcc
CFLAG = 
SERVER_FILES = server.c
CLIENT_FILES = client.c

all: server client

server: $(SERVER_FILES) 
	$(CC) $(CFLAG) -o server $(SERVER_FILES) -w

client: $(CLIENT_FILES)
	$(CC) $(CFLAG) -o client $(CLIENT_FILES) -w

clean: 
	$(RM) log/*