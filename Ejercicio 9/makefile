all: cliente server clienteMedio serverMedio
.PHONY:all

rpcprog.h rpcprog_clnt.c rpcprog_svc.c : rpcprog.x
	rpcgen rpcprog.x


cliente : cliente.c rpcprog_clnt.c
	gcc -lnsl -o cliente cliente.c rpcprog_clnt.c

clienteMedio : clienteMedio.c rpcprog_clnt.c
	gcc -lnsl -o clienteMedio clienteMedio.c rpcprog_clnt.c

serverMedio : serverMedio.c rpcprog_svc.c
	gcc -lnsl -o serverMedio serverMedio.c rpcprog_svc.c

server : server.c rpcprog_svc.c
	gcc -lnsl -o server server.c rpcprog_svc.c

