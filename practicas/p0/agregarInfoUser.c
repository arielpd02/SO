#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cuit_verifier.h"

typedef struct user
{
    char *nombre;
    uint8_t edad;
    char *cuit;

}user_t;
 

int main(void){

    //Pedimos memoria para el nuevo usuario y cargamos sus datos ingresados 
    user_t *new_user=(user_t*)malloc(sizeof(user_t));
    new_user->nombre=malloc(20);
    new_user->cuit=malloc(12);


    printf("Ingrese nombre de persona\n");
    scanf("%s",new_user->nombre);

    printf("Ingrese edad de persona\n");
    scanf("%d",&new_user->edad);

    printf("Ingrese Cuit de persona\n");
    scanf("%s",new_user->cuit);

    //LLamamos a ver si el cuit es correcto.
    uint8_t verificado = cuit_verifier(new_user->cuit);




    if(new_user->edad > 99 || new_user->edad <18 || verificado==1){
        printf("La información cargada es invalida, por favor, intente de nuevo\n");
    }else{
        printf("¡Su información fue cargada con éxito!\n");
    }

    free(new_user);


    return 0;
}