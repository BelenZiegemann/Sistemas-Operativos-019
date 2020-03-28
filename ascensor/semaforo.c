#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CANT_PERSONAS 3
#define CANT_PISOS 6

sem_t origen[CANT_PISOS], destino[CANT_PISOS],hayPersonas;

sem_t abrirPuertaSubir,cerrarPuertaSubir, abrirPuertaBajar, cerrarPuertaBajar;

pthread_t h_personas[CANT_PERSONAS], h_ascensor;

void *personas();
void *ascensor();
void llamarAscensor(int piso_origen);
void entrarAscensor(int piso_destino);
void salirAscensor(int piso_destino);
void subir();
void bajar();

void main(){


	for(int i=0; i<CANT_PISOS; i++){
		sem_init(&origen[i],0,0);
		sem_init(&destino[i],0,0);
	}

	sem_init(&hayPersonas,0,0);
	sem_init(&abrirPuertaSubir,0,0);
	sem_init(&cerrarPuertaSubir,0,0);
	sem_init(&abrirPuertaBajar,0,0);
	sem_init(&cerrarPuertaBajar,0,0);

	
	pthread_create(&h_ascensor,NULL, ascensor,NULL);
	for(int j=0;j<CANT_PERSONAS;j++){
		pthread_create(&h_personas[j],NULL,personas,NULL);
	}

	pthread_join(h_ascensor,NULL);
	for(int k=0;k<CANT_PERSONAS;k++){
		pthread_join(h_personas[k],NULL);
	}
}

void *personas(){

	printf("En personas\n");

	while(1){
		int pOrigen=random()%CANT_PISOS;
		int pDestino=random()%CANT_PISOS;
		
		printf("origen: %i\n",pOrigen);
		printf("destino: %i\n",pDestino);
		llamarAscensor(pOrigen);
		entrarAscensor(pDestino);
		salirAscensor(pDestino);
		sleep(4);
		printf("\n");
	}
}

void llamarAscensor(int piso_origen){

	sem_post(&hayPersonas);
	printf("Una persona LLAMO al ascensor en %i\n", piso_origen);
	sem_post(&origen[piso_origen]);
	
}

void entrarAscensor(int piso_destino){

	sem_wait(&abrirPuertaSubir);
	printf("Una persona ENTRO al ascensor y marco su piso destino: %i\n",piso_destino);
	sleep(4);
	sem_post(&destino[piso_destino]);
	sem_post(&cerrarPuertaSubir);
}

void salirAscensor(int piso_destino){

	sem_wait(&abrirPuertaBajar);
	printf("Una persona LLEGO a su piso destino: %i\n",piso_destino);
	sleep(4);
	sem_wait(&hayPersonas);
	sem_post(&cerrarPuertaBajar);
}


void *ascensor(){
	printf("En ascensor\n");

	while(1){
		
			sem_wait(&hayPersonas);
			sem_post(&hayPersonas);
			int ubicacion=0;

			subir();
			bajar();
}

void subir(){

	for(int i=0; i<=CANT_PISOS;i++){
		printf("Estoy en el piso %i SUBIENDO\n", i);
		while(sem_trywait(&origen[i])==0){				//Si hay alguien que quiera bajar en el piso actual
			sem_post(&abrirPuertaSubir);
			sem_wait(&cerrarPuertaSubir);
		}

		while(sem_trywait(&destino[i])==0){			//Si hay alguien que quiera subir en el piso actual
			sem_post(&abrirPuertaBajar);
			sem_wait(&cerrarPuertaBajar);
		}
		if(i!=CANT_PISOS)
			printf("Continuo SUBIENDO..\n");
		printf("\n");
	}
}

void bajar(){

	for(int i=CANT_PISOS;i>=0;i--){
		printf("Estoy en el piso %i BAJANDO\n", i);

		while(sem_trywait(&origen[i])==0){				//Si hay alguien que quiera subir en el piso actual
			sem_post(&abrirPuertaSubir);
			sem_wait(&cerrarPuertaSubir);
		}

		while(sem_trywait(&destino[i])==0){			//Si hay alguien que quiera bajar en el piso actual
			sem_post(&abrirPuertaBajar);
			sem_wait(&cerrarPuertaBajar);
		}
		if(i!=0)
			printf("Continuo BAJANDO \n");
		printf("\n");
	}
}
