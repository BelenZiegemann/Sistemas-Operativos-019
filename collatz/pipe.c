#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 100

void collatz (int n);

int p[2],readbytes;
char buffer[SIZE];
int dato=-1;
int pid, n;

int main (int argc, char *argv[] ){

	
	if(argc!=2){
		printf("Cantidad de argumentos invalida\n");
		exit(1);
	}
	n=atoi(argv[1]);
	if(n>1){
		if (pipe(p) == -1) {
               perror("fallo creacion del pipe\n");
               exit(EXIT_FAILURE);
    	}

   		pid=fork();

		if(pid==-1){
			perror("El proceso hijo no fue creado \n");
			exit(1);
		}

		else{
			//crea el proceso hijo. Cierro el lado de lectura del pipe. El hijo ejecuta el algoritmo. 
			if (pid==0){
				close(p[0]);
				collatz(n);
				exit(0);
				close(p[1]);
			}
			else{
				//Cierro el lado de lectura del pipe. 
				close(p[1]);
				while (dato != 1){
					read(p[0], &dato, sizeof(int));
					printf("%d ", dato);
				}
				wait(NULL);
				close(p[0]);
			}	
		}	
	}
	else{
		printf("Argumento invalido\n");
		exit(1);
	}
		

	printf("\n");
	return 0;
}

void collatz (int n){

	printf("%d ",n);
	while(n!=1){
		if(n%2==0){
			n=n/2;
			write(p[1],&n, sizeof(int));
		}
		else{
			n=3*n+1;
			write(p[1],&n, sizeof(int));
		}
	}
}