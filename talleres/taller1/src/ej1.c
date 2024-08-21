#include <signal.h> /* constantes como SIGINT*/
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int N,J,K;

void jugar(int){   

    printf("Saco numero al azar..\n");

    //Saca un numero aleatorio entre 0 y N
    int random = rand() % N;

    if(random == J){
        printf("He sido asesinado L.P.M\n");
        exit(EXIT_SUCCESS);
    }
    printf("UF, safe..!\n");
}



void juego_hijo(int N,int J){

    signal(SIGTERM,jugar);

    while (1)
    {
        //sleep(1);
    }
        
}



int main(int argc, char const *argv[]){

    //Guardo los parametros
    N=atoi(argv[1]);
    K=atoi(argv[2]);
    J=atoi(argv[3]);

    //Cuento perdidos
    int muertos=0;

    //Inicializo vector de PID
    pid_t pid_hijos[N];

    //Creamos los N procesos hijos
    for (size_t i = 0; i < N; i++)
    {   
        //Creo un hijo y me guardo su PID, Su id es el index en el array
        pid_t pid=fork();
        if(pid==0){
            juego_hijo(N,J);
        }
        //Soy padre , me guardo el PID
        pid_hijos[i]=pid;
        
    }

    //Jugamos K rondas
    for (size_t i = 0; i < K; i++)
    {
        printf("Comienzo ronda %d ...\n",i);

        for (size_t j = 0; j < N; j++)
        {   
            if(pid_hijos[j]==0){
                continue;
            }
            //Enviamos SIGTERM a c/hijo
            kill(pid_hijos[j],SIGTERM);
            sleep(1);
        }

        //Hay problema con el wait..
            
        int status=0;
        for (size_t k = 0; k < N; k++)
        {
            if(pid_hijos[k]==0){
                continue;
            }
            int pid = waitpid(pid_hijos[k],&status,WNOHANG); //Devuelve 0 si no termino. El PID en cc.

            if(pid<0){perror("wait");exit(EXIT_FAILURE);
            }else if(pid == pid_hijos[k]){
                //Marco el hijo como muerto
                pid_hijos[k]=0;
                muertos++;
            } //CC -> el hijo sobrevivio.
        }
        
    }

    if(muertos==N){
        printf("Perdieron todos :(\n");
        exit(EXIT_SUCCESS);
    }

    //Declaramos los ganadores si los hay
    for (size_t i = 0; i < N; i++)
    {
        if(pid_hijos[i]!=0){
            printf("Gano proceso %d de PID %d!\n",i,pid_hijos[i]);
            //Terminamos el proceso hijo
            kill(pid_hijos[i],SIGKILL);
        }
    }

    exit(EXIT_SUCCESS);
    

}