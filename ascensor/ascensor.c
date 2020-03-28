#include "common.h"


void ascensor();
void subir();
void bajar();

void main(){

	printf("Ejecutando ascensor...\n");
	
	/*Creo las colas de mensajes y obtengo un identificador.*/
	idColaMensaje1=msgget((key_t)1234, 0777|IPC_CREAT);
	idColaMensaje2=msgget((key_t)2341, 0777|IPC_CREAT);
	idColaMensaje3=msgget((key_t)3412, 0777|IPC_CREAT);

	if(idColaMensaje1==-1 || idColaMensaje2==-1 || idColaMensaje3==-1){
		printf("No fue posible crear la cola de mensajes\n");
		exit(1);
	}

	ascensor();
}

void ascensor(){

	printf("En ascensor...\n");
	while(1){
		
		struct mensaje *msg= (struct mensaje*) malloc(sizeof(struct mensaje));

		msgrcv(idColaMensaje3,msg,tamanio,hayPersonas,0);
		msg->tipo=hayPersonas;
		msgsnd(idColaMensaje3,msg,tamanio,0);

		subir();
		bajar();
	}
}

void subir(){

	struct mensaje *msg= (struct mensaje*) malloc(sizeof(struct mensaje));

	for(int i=0; i<=CANT_PISOS; i++){
		printf("Estoy en el piso %i SUBIENDO\n", i);
		int tipo=i+1;

		
		while((msgrcv(idColaMensaje1, msg, tamanio,tipo,IPC_NOWAIT))!=-1){

			msg->tipo=abrirPuertaSubir;
			msgsnd(idColaMensaje3,msg,tamanio,0);

			msgrcv(idColaMensaje3,msg,tamanio,cerrarPuertaSubir,0);
			printf("Subio una persona..\n");
				
		}

		while((msgrcv(idColaMensaje2,msg,tamanio,tipo, IPC_NOWAIT))!=-1){

			msg->tipo=abrirPuertaBajar;
			msgsnd(idColaMensaje3,msg,tamanio,1);

			msgrcv(idColaMensaje3,msg,tamanio, cerrarPuertaBajar,0);
			printf("Bajo una persona..\n");
		}
		sleep(2);
		if(i!=CANT_PISOS)
			printf("Continuo SUBIENDO...\n");
		printf("\n");
	}
	free(msg);
}

void bajar(){

	struct mensaje *msg= (struct mensaje*) malloc(sizeof(struct mensaje));

	for(int i=CANT_PISOS; i>=0; i--){
		printf("Estoy en el piso %i BAJANDO\n",i);

		int tipo=i+1;

		while((msgrcv(idColaMensaje1,msg,tamanio,tipo,IPC_NOWAIT))!=-1){
			msg->tipo=abrirPuertaSubir;
			msgsnd(idColaMensaje3, msg,tamanio,0);
			msgrcv(idColaMensaje3,msg,tamanio, cerrarPuertaSubir,0);
			printf("Subio una persona..\n");
		}

		while((msgrcv(idColaMensaje2,msg,tamanio,tipo,IPC_NOWAIT))!=-1){
			msg->tipo=abrirPuertaBajar;
			msgsnd(idColaMensaje3,msg,tamanio,0);
			msgrcv(idColaMensaje3,msg,tamanio,cerrarPuertaBajar,0);
			printf("Bajo una persona..\n");
		}
		sleep(2);
		if(i!=0)
			printf("Continuo BAJANDO...\n");
		printf("\n");
	}
	free(msg);
}