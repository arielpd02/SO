#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(void){
    printf("Ingrese un comando\n");

    char command[50];
    scanf("%s",command);
    printf("%s\n",command);

    system(command);

    exit(EXIT_SUCCESS);
}