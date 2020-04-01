#include <sys/types.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	int validP; 
	if(argc != 3 || argv[2] == NULL){
		printf("Error. Ejecute el programa como\n\t./permisos [nombre_archivo][permisos]\n");
		return 0;
	}

	validP = atoi(argv[2]);
  
	if ((validP<778) && (validP > -1)){
		long permission;
		permission = strtol(argv[2],NULL,8); //string to long int
		
		if (chmod(argv[1],permission) != -1){
			printf("Permisos de %s actualizados.\n",argv[1]);
			char comando[100] = "ls -l ";
			strcat(comando, argv[1]);
			system(comando);
		}else
			printf("Imposible modificar los permisos.\n");
	}
	else
		printf("Ingrese un valor valido.\n");
	return(0);
}
