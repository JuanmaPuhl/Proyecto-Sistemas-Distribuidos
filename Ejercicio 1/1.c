#include <stdio.h>
#include <stdlib.h>

int main(){
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	int pid = fork();
	gettimeofday(&end, NULL);
	if(pid!=0){
		unsigned int t = end.tv_usec - begin.tv_usec;
		printf("tiempo en microsegundos: %u\n",t);
	}
	return 0;
}