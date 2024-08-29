#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()
#include <signal.h>


void handler_SIGINT_hijo(int){
    printf("Dejame pensarlo...\n");
    sleep(5);
    printf("Ya sé el significado de la vida.\n");
    write(4,"42",2);
    kill(getppid(),SIGINT);
}

void handler_SIGINT_padre(int){
    char numero;
    read(3,&numero,sizeof(numero));
    printf("Mirá vos. El significado de la vida es %c\n",numero);
    printf("¡Bang Bang, estás liquidado!\n");
    kill(,SIGHUP);      //Como sabe el padre el pid del hijo desde el handler? Sino la envia desde aca, porque no aparece que volvio de este en strace?
}
void subrutina_hijo(){
    close(3);
    pid_t pid_padre=getppid();
    signal(SIGINT,handler_SIGINT_hijo);
    signal(SIGHUP,handler_SIGHUP);

    while(1){
        //Nada
    }
}

void handler_SIGHUP(){  
    printf("Me voy a mirar crecer las flores desde abajo.\n");
    close(4);
    exit(EXIT_SUCCESS);
}




int main(void){
    //Creamos pipe de los procesos
    int pipes[2];
    pipe(pipes);

    pid_t pid_hijo=fork();
    if(pid_hijo==0){
        subrutina_hijo();
    }
    close(4);
    signal(SIGINT,handler_SIGINT_padre);
    signal(SIGCHLD,NULL);
    sleep(1);
    printf("¿Cuál es el significado de la vida?\n");
    kill(pid_hijo,SIGINT);

    sleep(10);

    printf("Te voy a buscar en la oscuridad.\n");
    close(3);

    exit(EXIT_SUCCESS);

}