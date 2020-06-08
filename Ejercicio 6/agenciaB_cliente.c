#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "header.h"


#define PORT 14550 //Puerto agencia B
#define MAXDATASIZE 128 //Maximos datos del paquete

/*
 * Este es el cliente en la agencia A, deberia ser capaz de comunicarse
 * con el servidor en la agencia B.
 * */
int sockfd;
char* patentarAuto()
{
	return "Patentando auto.";
}

char* transferirVehiculo()
{
	return "Transfiriendo vehiculo.";
}

char* dominioVehiculo()
{
	return "Obteniendo informacion del dominio del vehiculo.";
}


/*Metodo auxiliar que maneja los pedidos del cliente con el sv*/
void pedirASv(int fn)
{
	/*Ahora le envio la funcion al servidor de la agencia B
	 * aunque yo no deberia saberlo jeje :) */
	Cabecera* c = malloc(sizeof(Cabecera)); //Creo la cabecera
	c->identificador = IdpedirFuncion;
	//printf("%d.\n",c->identificador);
	Funcion* f = malloc(sizeof(Funcion));
	f->Idfuncion = fn;
	if(send(sockfd, c, sizeof(Cabecera), 0) == -1)//Envio la cabecera
	{
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	if(send(sockfd, f, sizeof(Funcion), 0) == -1)//Envio la funcion deseada
	{
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	
	/*Ahora debo esperar a que lleguen los mensajes con los datos*/
	int recibi = 0;
	while(!recibi) //Ejecuto hasta que reciba el mensaje que quiero
	{
		if((recv(sockfd, c, sizeof(Cabecera), MSG_PEEK)) == -1)//Recibo pero no quito de la cola
		{
		}
		int tipo = c->identificador; //Obtengo el tipo de mensaje
		if(tipo == Iddardatos)
		{
			recv(sockfd, c, sizeof(c), 0); //Consumo el mensaje
			Datos* datos = malloc(sizeof(Datos));
			if(recv(sockfd, datos, sizeof(Datos), 0)==-1) //Consumo el mensaje
			{
			}
			printf("Resultado: %s\n",datos->datos);
			recibi = 1;
		}	
	}
}

void cerrarPrograma(pid_t pid)
{
	Cabecera* cabecera = malloc(sizeof(Cabecera));
	cabecera->identificador = IdCerrar;
	if(send(sockfd, cabecera, sizeof(Cabecera), 0) == -1)//Envio la cabecera
	{
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	kill(pid, SIGKILL); //Mato el proceso receptor de mensajes
	close(sockfd); //Cierro el socket
	exit(0); //Salgo
}

/**En este caso la comunicacion es orientada a sockets con conexion*/
int main(int argc, char **argv)
{
	struct hostent *he;
	struct sockaddr_in their_addr;
	/*Formato de entrada esperado: 
	 * 	./agenteA_cliente <ip_sv_agenciaA> <ip_sv_agenciaB>
	 */
	if(argc != 2){//Reviso la cantidad de parametros ingresados
		printf("Error con los argumentos.\n");
		exit(-1);
	}
	if((he=gethostbyname(argv[1]))==NULL){//Reviso el host ingresado
		printf("Error: Hostname invalido");
		exit(-1);
	}
	//Ahora abro el socket
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("Error: Ocurrio un problema al abrir el socket");
		exit(-1);
	}
	//Configuracion
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);
	bzero(&(their_addr.sin_zero),8);
	
	//Voy a conectarme con el server
	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr))==-1){
	}
	
	//Creo otro proceso que se encargue de escuchar mensajes con requerimientos del servidor
	pid_t pid = fork();
	if(!pid)
	{
		int numbytes;
		while(1) //Hago que se ejecute siempre
		{	
			Cabecera* cabecera = malloc(sizeof(Cabecera));
			if((numbytes = recv(sockfd, cabecera, sizeof(Cabecera), MSG_PEEK)) == -1)//Recibo el mensaje pero no lo quito de la cola hasta revisar el tipo
			{
			}
			int tipo = cabecera->identificador;
			//~ printf("Recibido tipo: %d\n", tipo); //Escribo lo que recibi del sv.		
			if(tipo == Idpedirdato) //Es un pedido de la otra agencia
			{
				//~ printf("Entre al if. Recibi un pedirDato.\n");
				recv(sockfd, cabecera, sizeof(Cabecera), 0); //Consumo el mensaje
				Funcion* funcion = malloc(sizeof(Funcion));
				if((recv(sockfd, funcion, sizeof(Funcion), 0)) == -1)//Recibo los datos
				{
				}
				//~ printf("Recibi la funcion.\n");
				int f = funcion->Idfuncion;
				//~ printf("La funcion es: %d\n",f);
				char msg[MAXDATASIZE];
				switch(f)
				{
					case 4: strcpy(msg,patentarAuto());
							break;
					case 5: strcpy(msg,transferirVehiculo());
							break;
					case 6: strcpy(msg,dominioVehiculo());
							break;
				}
				//~ printf("El mensaje a enviar es: %s\n",msg);
				//Ahora debo enviar el mensaje obtenido
				cabecera->identificador = Iddardatos;
				//~ printf("El id de la cabecera es: %d\n",cabecera->identificador);
				Datos* datos = malloc(sizeof(Datos));
				strcpy(datos->datos,msg);
				if(send(sockfd, cabecera, sizeof(Cabecera), 0) == -1)//Envio la cabecera
				{
					perror("sendto");
					exit(EXIT_FAILURE);
				}
				//~ printf("Envie la cabecera.\n");
				if(send(sockfd, datos, sizeof(Datos), 0) == -1)//Envio la funcion deseada
				{
					perror("sendto");
					exit(EXIT_FAILURE);
				}
				//~ printf("Envie los datos.\n");				
			}
			if(tipo == IdCerrar)
			{
				printf("Se cerro el servidor.\n");
				kill(getppid(),SIGKILL);
				exit(0);
			}
		}	
	}
	else //Sigo con la ejecucion del proceso principal
	{
		while(1) //Se ejecuta siempre
		{
			int verificacionLista = 0;
			/*Imprimo el menu*/
			printf("Bienvenido a la agencia B.\n");
			printf("==========================\n\n");
			printf("Ingrese la funcion deseada:\n");
			printf("1. Pedir turno para licencia de matrimonio.\n");
			printf("2. Obtener informacion de la partida de nacimiento.\n");
			printf("3. Pedir turno para registrar bebe.\n");
			printf("4. Patentar el auto.\n");
			printf("5. Transferir vehiculo.\n");
			printf("6. Informacion del dominio de vehiculo.\n");
			printf("0. Salir.\n");
			char eleccion = ' ';
			while(!verificacionLista) //Mientras el usuario no ingrese una funcion correcta ejecuto.
			{
				scanf( " %c", &eleccion);
				//Valido la eleccion del usuario.
				if(isdigit(eleccion)) //Reviso que sea un digito
				{
					int f = atoi(&eleccion); //Chequeo que este en el rango
					if(f < 0 || f > 6)
						printf("Error: La funcion ingresada no es valida.\n");
					else
						verificacionLista = 1;
				}
				else //No ingreso un digito
					printf("Error: La funcion ingresada no es valida.\n");
				fflush(stdin);
			}
			
			int fn = atoi(&eleccion);
			switch(fn) //Ahora ejecuto la funcion correspondiente
			{
				case 0: cerrarPrograma(pid);
						break;
				case 4: printf("%s\n",patentarAuto());
						break;
				case 5: printf("%s\n",transferirVehiculo());
						break;
				case 6: printf("%s\n",dominioVehiculo());
						break;
				default:pedirASv(fn);  //Si no esta aca debo pedirla al sv.
			}
		}
	}
	
	
	
	return 0;
}

