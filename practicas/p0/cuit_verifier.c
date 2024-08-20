#include "cuit_verifier.h"

#define MAX_DIGITS 11

uint8_t productos[] = {5,4,3,2,7,6,5,4,3,2};



uint8_t cuit_verifier(char *cuil){


    //Calculamos el verificador , convertimos c/ char en un int
    int verifier = 0;

    for (int i = 0;i < MAX_DIGITS - 1; i++)
    {    
        int dig = cuil[i] - '0';
        verifier+=productos[i]* dig;
        printf("%d %d \n",productos[i],dig);
    }


    verifier=verifier/11;
    verifier-=11;

    // Si es distinto , lo corregimos y printeamos en formato.

    if(verifier != cuil[MAX_DIGITS-1] - '0'){
        char correct_cuil[14];
        int j=0;
        for (int i = 0; i < MAX_DIGITS+2; i++)
        {   
            
            if( i==2 || i == 11 ){
                correct_cuil[i]='-';
            }else if (i>2 && i<11){
                j=i-1;
                correct_cuil[i]=cuil[j];
            } else {
                correct_cuil[i]=cuil[j];
                j++;
            }
        }


        printf("%d\n",verifier);
        printf("%c \n",verifier + '0');

        correct_cuil[MAX_DIGITS + 1]=verifier + '0';

        printf("El CUIL correcto seria %s \n",correct_cuil);

        return 1;
    }else{
        printf("El CUIL ingresado es valido. \n");
    }
    
    return 0;
}





