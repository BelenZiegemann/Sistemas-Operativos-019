#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define tamanio sizeof(mensaje)-sizeof(long)

#define CANT_PISOS 6
#define CANT_PERSONAS 2

#define hayPersonas 1
#define abrirPuertaSubir 2
#define cerrarPuertaSubir 3
#define abrirPuertaBajar 4
#define cerrarPuertaBajar 5

typedef struct mensaje {
	long tipo;
}mensaje;

key_t generateKey(){
	int projectId= 30;
	char path[200];
	getcwd(path,200*sizeof(char));
	key_t key= ftok(path,projectId);
	return key;
}

void msg_wait(int idCola, struct mensaje *msj,long tipo, int flag){
	int resul;
	if(flag==1)
		resul=msgrcv(idCola,msj,tamanio,tipo,IPC_NOWAIT);
	else
		resul=msgrcv(idCola,msj,tamanio,tipo,0);
	
	if(resul==-1){
		printf("Error al recibir el mensaje\n");
		exit(1);
	}
}

void msg_signal(int idCola, struct mensaje *msj,int flag){
	int resul;
	if(flag==1)
		resul=msgsnd(idCola,msj,tamanio,IPC_NOWAIT);
	else
		resul=resul=msgsnd(idCola,msj,tamanio,0);

	if(resul==-1){
		printf("Error al enviar el mensaje\n");
		exit(1);
	}

}