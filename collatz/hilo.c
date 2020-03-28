#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

void*collatz (void *num);

struct lista{
	int num;
	struct lista *sig;
};

struct  lista *elemento=NULL;

int n;
pthread_t hilo;

int main(int argc, char *argv[]){

	if(argc != 2){
		printf("Cantidad de argumentos invalida\n");
		exit(1);
	}

	n=atoi(argv[1]);

	if(n>=1){
	 	if(0!=pthread_create(&hilo, NULL, collatz, &n))
		return -1;
	}
	else{
		printf("Argumento invalido.Pruebe con valores mayores a 1\n");
		exit(1);
	}
	void *retorno;
	pthread_join(hilo,&retorno);
	elemento= (struct lista *) retorno;
	struct lista * elem=NULL;
	
	while(elemento!=NULL){
		printf("%i ",elemento->num);
		elem=elemento;
		elemento=elemento->sig;
		free(elem);
	}
	printf("\n");
	return 0;
}


void *collatz (void *num){

	int n=*(int*)num;
	struct lista *secuencia = malloc(sizeof(struct lista));
	secuencia->num = n;
	elemento = secuencia;
	while(n!=1){
		if(n%2==0){
			n=n/2;
		}
		else{
			n=3*n+1;
		}
		struct lista *siguiente=malloc(sizeof(struct lista));
		siguiente->num= n;
		secuencia->sig=siguiente;
		secuencia=siguiente;
	}
	pthread_exit(elemento);

}
