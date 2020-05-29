#include <stdio.h>
#include "rpcprog.h"
#include <rpc/rpc.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
extern int errno;

int *ejecutar_1_svc(char** arg,struct svc_req *req)
{
	int status;
	static int  result;
	pid_t pid = fork();//Creo un proceso hijo
    if(pid == 0)
    {//Si estoy en el proceso hijo
		if(execve("imagen",arg,NULL)<0) //Intento ejecutar la imagen
		{
			/**Ocurrio un error*/
			printf("Ocurrio un error ejecutando la imagen.\n");
			exit(-1);
		}
		exit(0);
	}
	else
	{//Estoy en el padre
		do 
		{ //Con esto espero a que termine la ejecucion del hijo
			
            pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) 
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
           if (WIFEXITED(status)) 
           { //Termino la ejecucion del hijo
				printf("Ya termine la ejecucion.\n");
				if(status!=0)
					result = -1;
				else
					result = 0;
				return &result;
           }

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return &result;
} 

