#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{	
	/**Variables para guardar el tiempo*/
	struct timeval begin, end;
	int status;
	/**Aqui comienzo el timer*/
    gettimeofday(&begin,NULL);
    pid_t pid = fork();//Creo un proceso hijo
    if(pid == 0){//Si estoy en el proceso hijo
		char* arr[1] = {argv[1]};
		if(execve("imagen",arr,NULL)<0) //Intento ejecutar la imagen
		{
			/**Ocurrio un error*/
			printf("Ocurrio un error ejecutando la imagen.\n");
		}
	}
	else{//Estoy en el padre
		do { //Con esto espero a que termine la ejecucion del hijo
            pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
           if (WIFEXITED(status)) { //Termino la ejecucion del hijo
				gettimeofday(&end,NULL);
				printf("Crear el proceso hijo ha tomado: %ld microsegundos\n",
					((long)1.0e6*end.tv_sec + end.tv_usec) - 
					((long)1.0e6*begin.tv_sec + begin.tv_usec));
           } 
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 0;
}

