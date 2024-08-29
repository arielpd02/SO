#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()

enum{READ,WRITE};
enum{PADRE,HIJO1,HIJO2};
int pipes[3][2];

void hijo1(){
    int valor=0;
    while(valor<50){
        read(pipes[PADRE][READ],&valor,sizeof(valor));
        valor++;
        printf("Hijo1 envia el valor %d\n",valor);
        write(pipes[HIJO1][WRITE],&valor,sizeof(valor));
    }

    exit(EXIT_SUCCESS);

}

void hijo2(){

    int valor=0;
    while(valor<50){
        read(pipes[HIJO1][READ],&valor,sizeof(valor));
        valor++;
        printf("Hijo2 envia el valor %d\n",valor);
        write(pipes[HIJO2][WRITE],&valor,sizeof(valor));
    }

    exit(EXIT_SUCCESS);

}

/*Mejor implementacion -> while(1) , que la guarda sea los valores de 'valor'. De esta manera nos abstraemos de a quien le toca el valor final.*/


int main(void){
    //Creamos los pipes de comunicacion
    pipe(pipes[PADRE]);
    pipe(pipes[HIJO1]);
    pipe(pipes[HIJO2]);

    //Creamos los hijos
    pid_t pid_1=fork();
    if(pid_1==0){
        hijo1();
    }

    pid_t pid_2=fork();
    if(pid_2==0){
        hijo2();
    }

    //Rearmamos las conexiones
    int valor=0;
    write(pipes[PADRE][WRITE],&valor,sizeof(valor));

    while(valor <50){
        read(pipes[HIJO2][READ],&valor,sizeof(valor));
        if(valor>=50) break;
        valor++;
        printf("Padre envia el valor %d\n",valor);
        write(pipes[PADRE][WRITE],&valor,sizeof(valor));
    }

    exit(EXIT_SUCCESS);


}