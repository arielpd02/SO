#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>



void abraham(){
    __pid_t pid = fork();
    if(pid == 0){
        homero();
    }else{
        wait(NULL);
        printf("Soy Abraham\n");
        sleep(1);
    }
}

void homero(){
    __pid_t pid=fork();

    //Espero a que termine Bart
    wait(NULL);

    if( pid == 0){
        bart();
    }else{
        pid=fork();
        //Espero a que termine Lisa
        wait(NULL);
        if(pid==0){
            lisa();
        }else{
            pid=fork();
            //Espero a que termine Maggie
            wait(NULL);
            if(pid==0){
                maggie();
            }else{
                printf("Soy Homero\n");
                sleep(1);
                exit(0);    
            }
        }
    }
}

void bart(){
    printf("Soy Bart\n");
    sleep(1);
    exit(0);

}
void lisa(){
    printf("Soy Lisa\n");
    sleep(1);
    exit(0);
}
void maggie(){
    printf("Soy Maggie\n");
    sleep(1);
    exit(0);
}






int main(void){
    abraham();
    return 0;
}



