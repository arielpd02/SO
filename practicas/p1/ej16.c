#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()

enum{READ,WRITE};

void proceso_ls(int pipes[]){
    //Modificamos las referencias de los descriptores: STDOUT->pipes[W]
    dup2(pipes[WRITE],STDOUT_FILENO);

    //Eliminamos referencia a punta de lectura
    close(pipes[READ]);

    //Cargamos programa ls -al
    execl("/usr/bin/ls","ls","-al",(char*)NULL);
}

void proceso_wc(int pipes[]){
    //Modificamos referencias de los descriptores : STDIN->pipes[R]
    dup2(pipes[READ],STDIN_FILENO);

    //Eliminamos referencia a punta de escritura del pipes
    close(pipes[WRITE]);

    //Cargamos programa 'wc -l'
    execl("/usr/bin/wc","wc","-l",(char*)NULL);

}


//Proceso shell-padre
int main(int argc,const char *argv[]){
    //Iniciamos un pipe para los hijos subprocesos
    int pipes[2];
    pipe(pipes);

    //Creamos los subprocesos "ls y wc"
    pid_t pid_ls=fork();
    if(pid_ls==0){
        proceso_ls(pipes);
    }

    pid_t pid_wc=fork();
    if(pid_wc==0){
        proceso_wc(pipes);
    }
    
    //Eliminamos referencia del padre a los fd del pipes para que wc tenga EOF
    close(pipes[READ]);
    close(pipes[WRITE]);


    //Fin
    exit(EXIT_SUCCESS);

}