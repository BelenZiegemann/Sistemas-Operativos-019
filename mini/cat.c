#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void leerArchivo();

int main(int argc, char *argv[]){
	
	if(argc<1)
		printf("No ingreso los argumentos suficientes. \n");
	else
		leerArchivo(argv[1]);

	return 0;
}

void leerArchivo(char *direccion){

	FILE *fd;
	fd=fopen(direccion, "rt");
	if(fd== NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}	
	else{
		char c;
		int fin= fscanf(fd, "%c", &c);
		while(fin!=EOF){
			if(c== '\n')
				printf("\n");
			else
				printf("%c", c);
			
			fin=fscanf(fd,"%c", &c);
		}
		
	}
	printf("\n");
	fclose(fd);
}