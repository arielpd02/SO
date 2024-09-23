#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

void closeForStart(int pipes[][2],int start,int n){
	for (size_t i = 0; i < n; i++)
	{
		if(i!= start && i!=((start+1)%n)){
			close(pipes[i][PIPE_WRITE]);
		}
	}
	
}

void closeForOthers(int pipes[][2],int i,int n){
	for (size_t k = 0; k < n; k++)
	{
		if( k!= (i+1)%n){
			close(pipes[k][PIPE_WRITE]);
		}
	}
}

int generate_random_number(){
	return (rand() % 50);
}

void proceso_hijo(int i,int start,int n,int pipes[][2]){
	int msg=0;
	int secret = generate_random_number();

	//Primero cerramos pipes que no vamos utilizar, segun tipo de proceso.
	if(i==start){ 		
		closeForStart(pipes,start,n);
	}else{
		closeForOthers(pipes,i,n);
	}

	//Mientras mi antecesor me envie algun dato.Sino sucede , read devuelve 0.
	while(read(pipes[i][PIPE_READ],&msg,sizeof(msg))>0){
		if( i == start && msg>=secret){
			// Fue >=, le enviamos al padre el resultado y salimos.
			write(pipes[i][PIPE_WRITE],&msg,sizeof(msg));
			printf("El mensaje fue %d y el numero secreto es %d\n",msg,secret);
			break;
		}
		printf("Proceso %d envia mensaje: %d\n",i,msg);
		msg++;
		write(pipes[(i+1)%n][PIPE_WRITE],&msg,sizeof(msg));
	}

	//Cerramos escritura del pipe i+1 para desencadenar los EOFs
	close(pipes[(i+1)%n][PIPE_WRITE]);
	exit(0);
}






int main(int argc, char **argv){	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, start, initial_value;
	n = atoi(argv[1]);
	initial_value = atoi(argv[2]);
	start = atoi(argv[3]);

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, initial_value, start);
    
    /* COMPLETAR */

	//Creamos los pipes : el pipe 0 corresponde al del padre.
	int pipes[n][2];

	for (size_t i = 0; i < n; i++)
	{
		pipe(pipes[i]);
	}

	pid_t pid_inicial;

	//Creamos los hijos y cerramos pipes correspondientes.
	for (size_t i = 0; i < n; i++)
	{
		pid_t pid=fork();

		if(pid==0){
			proceso_hijo(i,start,n,pipes);
		}
	}

	//Enviamos valor inicial al hijo inicial.
	write(pipes[start][PIPE_WRITE],&initial_value,sizeof(initial_value));

	//Cerramos Write ends de los procesos.
	for (int i = 0; i < n; i++)
	{
		close(pipes[i][PIPE_WRITE]);
		
	}

	//Leemos resultado del hijo, espero a que finalicen.
	for (int i = 0; i < n; i++)
	{
		pid=wait(NULL);
	}

	int final_value=0;
	read(pipes[start][PIPE_READ],&final_value,sizeof(final_value));
	printf("El numero que finalizo el anillo es %d\n",final_value);

	return 0;
	
}
