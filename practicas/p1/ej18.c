#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()
#include <signal.h>
#include <types.h>



void informarResultado(int num,int res){

}

void ejecutar_nieto(int pipes[]){
    int numero;
    read(pipes[READ],&numero,sizeof(numero));
    int resultado=calcular(numero);

    //Una vez hecho el calculo, se lo mandamos al padre y le avisamos
    write(pipes[WRITE],&resultado,sizeof(resultado));
    exit(EXIT_SUCCESS);
}


void ejecutarHijo (int i, int pipes[][2]) {
    //Creamos el pipe con el nieto y leemos el numero secreto.
    int pipes_hijo[2]={0};
    pipe(pipes_hijo);

    int numero=0;
    read(pipes[i][READ],&numero,sizeof(numero));


    pid_t pid_hijo=fork();
    if(pid_hijo==0){
        ejecutar_nieto(pipes_hijo);
    }
    write(pipes_hijo[WRITE],&numero,sizeof(numero));

    //Respondemos al polling del padre hasta que este finalice
    char done=0;
    while(1){
        int pid=waitpid(pid_hijo,NULL,WNOHANG);
        if(pid!=0){break;}                  //Si devuelve 0 -> el hijo no cambio de estado
        write(pipes[N+i][WRITE],&done,sizeof(done));
    }

    //Terminado el hijo , leemos el resultado e informamos a padre
    int resultado;
    read(pid_hijo[READ],&resultado,sizeof(resultado));
    done=1;
    write(pipes[N+i][WRITE],&done,sizeof(done));


    //Enviamos numero y res al padre
    write(pipes[N+i][WRITE],&numero,sizeof(numero)); //Sup tenemos N
    write(pipes[N+i][WRITE],&resultado,sizeof(resultado)); //Sup tenemos N



}




int main(int argc, char* argv[]){
    if (argc< 2) {
        printf ("Debe ejecutar con la cantidad de hijos como parametro\n");
        return 0; }
    int N = atoi(argv[1]);
    int pipes[N*2][2];

    for ( int i=0; i< N*2; i++){
        pipe(pipes[i]); }

    for (int i=0; i< N; i++) {
        int pid = fork () ;
        if (pid==0) {
            ejecutarHijo(i,pipes);  //No deberia pasarle N tambien??
        return 0;
        } else {    
            int numero = dameNumero(pid) ;
            write(pipes[i][1], &numero, sizeof(numero)); } }
            int cantidadTerminados = 0;
            char hijoTermino [N] = {0};
            while (cantidadTerminados < N) {
                for ( int i=0; i< N; i++) {
                    if (hijoTermino[i]) {
                    continue; }
                    char termino = 0;
                    write(pipes[i][1], &termino, sizeof(termino));  //Por que es necesario?
                    read(pipes[N+i][0], &termino, sizeof(termino));
                    if (termino) {
                        int numero;
                        int resultado ;
                        read(pipes[N+i][0], &numero, sizeof(numero));
                        read(pipes[N+i][0], &resultado, sizeof(resultado));
                        informarResultado(numero, resultado);
                        hijoTermino[i] = 1;
                        cantidadTerminados++; 
                    } 
                } 
            }
    wait(NULL);
    return 0; 
}