all: client server key
.PHONY: all

client : client.c
	gcc -lnsl -o client client.c

server : server.c
	gcc -lnsl -o server server.c
	
key	:
	@echo "" > mqueue_server_key
