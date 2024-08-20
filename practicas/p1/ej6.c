//Me piden implementar una rutina/programa que haga lo mismo que la syscall system()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>



void flush_input_buffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

void system_aux(char const *args[]){
    sleep(1);
    printf("Ejecutando el comando...\n");

    //Llamamos a exec ; hardcodeando el path a los shell comands.
    int status = execv("/bin/ssh",args);

    printf("%d\n",status);

    //Finalizamos el proceso hijo
    exit(status);
}


int main(void){

    printf("Ingrese un comando Shell\n");
    char *command;
    scanf("%s",command);

    //Borramos stdin
    flush_input_buffer();
    int cant;
    printf("Ingrese numero de argumentos\n");
    scanf("%d",&cant);

    flush_input_buffer();

    const char *args[50]; // Cota teorica de argumentos de un shell command

    //Copiamos los argumentos en el array del comando
    args[0]=command;
    for (size_t i = 1; i < cant+1; i++)
    {   
        char *arg=(char*)malloc(10);
        printf("Ingrese argumento %d\n",i);
        scanf("%s",arg);
        args[i]=arg;
    }



    //Creamos un proceso hijo que invoque la syscall del comando
    __pid_t pid = fork();
    if(pid==0){
        system_aux(args);
    }else{
        if(wait(NULL) < 0){
            perror("Comando incorrecto.\n");
            exit(-1);
        };

        sleep(1);
        printf("Comando ejecutado\n");
    }

    exit(EXIT_SUCCESS);
}

