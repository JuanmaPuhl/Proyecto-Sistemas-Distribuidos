all: ej2b_cliente ej2b_server imagen
.PHONY:all

imagen : imagen.c
	gcc -o imagen imagen.c

rpcprog.h rpcprog_clnt.c rpcprog_svc.c : rpcprog.x
	rpcgen rpcprog.x


ej2b_cliente : ej2b_cliente.c rpcprog_clnt.c
	gcc -lnsl -o ej2b_cliente ej2b_cliente.c rpcprog_clnt.c


ej2b_server : ej2b_server.c rpcprog_svc.c
	gcc -lnsl -o ej2b_server ej2b_server.c rpcprog_svc.c

