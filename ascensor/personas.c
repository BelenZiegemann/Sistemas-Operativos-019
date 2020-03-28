#include "common.h"

key_t clave;
int idColaMensaje1, idColaMensaje2,idColaMensaje3;

void personas();
void llamarAscensor(int piso_origen);
void entrarAscensor(int piso_destino);
void salirAscensor(int piso_destino);
void getRandom();

void main(){

	printf("Ejecutando personas...\n");
	
	/*Creo las colas de mensaje y obtengo un identificador.*/
	idColaMensaje1=msgget((key_t)1234,0777);//Para manejar los pisos origen
	idColaMensaje2=msgget((key_t)2341,0777);//Para manejar los pisos destino
	idColaMensaje3=msgget((key_t)3412,0777);//Para manejar los semaforos restantes
	/*Para eliminar las colas ejecutar desde la consola:
		ipcrm -Q 1234
		ipcrm -Q 2341
		ipcrm -Q 3412*/

	if(idColaMensaje1==-1 || idColaMensaje2==-1 || idColaMensaje3==-1){
		printf("No fue posible crear la cola de mensajes\n");
		exit(1);
	}

	
	for(int i=0; i<CANT_PERSONAS; i++){
		if(fork()==0)
			personas();
	}
	for(int j=0;j<CANT_PERSONAS;j++){
		wait(NULL);
	}
	
}

void personas(){

	printf("En personas..\n");
	while(1){

		int pOrigen=getRandom();
		int pDestino=getRandom();
		while(pOrigen==pDestino){
			pDestino=getRandom();
		}
		
		printf("origen: %i\n",pOrigen);
		printf("destino: %i\n",pDestino);
		llamarAscensor(pOrigen);
		entrarAscensor(pDestino);
		salirAscensor(pDestino);
		printf("\n");
	}
}

void llamarAscensor(int piso_origen){

	struct mensaje *msg= (struct mensaje*) malloc(sizeof(struct mensaje));
	//signal hayPersonas
	msg->tipo=hayPersonas;
	msgsnd(idColaMensaje3,msg,tamanio,0);
	printf("Una persona LLAMO al ascensor en %i\n", piso_origen);
	//signal origen[piso_origen]
	msg->tipo=piso_origen+1;
	msgsnd(idColaMensaje1,msg,tamanio,0);
	sleep(4);
	free(msg);
}

void entrarAscensor(int piso_destino){

	struct mensaje *msg= (struct mensaje*) malloc(sizeof(struct mensaje));

	//wait abrirPuertaSubir
	msg->tipo=abrirPuertaSubir;
	msgrcv(idColaMensaje3,msg, tamanio, abrirPuertaSubir,0);

	printf("Una persona ENTRO al ascensor y marco su piso destino: %i\n",piso_destino);
	sleep(4);
	
	msg->tipo=piso_destino+1;
	msgsnd(idColaMensaje2,msg,tamanio,0);

	msg->tipo=cerrarPuertaSubir;
	msgsnd(idColaMensaje3,msg,tamanio,0);

	free(msg);
}

void salirAscensor(int piso_destino){

	struct mensaje *msg= (struct mensaje*) malloc(sizeof(struct mensaje));
	
	msg->tipo=abrirPuertaBajar;
	msgrcv(idColaMensaje3,msg,tamanio,abrirPuertaBajar,0);

	printf("Una persona LLEGO a su piso destino: %i\n",piso_destino);
	sleep(4);

	msg->tipo=hayPersonas;
	msgrcv(idColaMensaje3, msg,tamanio, hayPersonas, IPC_NOWAIT);
	
	msg->tipo=cerrarPuertaBajar;
	msgsnd(idColaMensaje3,msg,tamanio,IPC_NOWAIT);
}

void getRandom(){
	int resul= (time(NULL)^getpid()%CANT_PISOS);
	return resul;
}
