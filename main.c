//
// Por Arturo Canga. V-25.696.222
// y Luis Fernandez. V-28.002.235
// Para AyPII, creado el 7/8/20
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "menu.h"

void main(){
    int option=0;
    casilla ***adyacencia=NULL;
    place *arr_place=NULL;
    int size=0;
    while (1){
        if (!adyacencia)
            printf("ADVERTENCIA: No tiene archivos cargados\n");
        printf("Introduzca que desea hacer: \n");
        printf("1. Cambiar los archivos a manejar (Esto limpiará los sistemas del mapa e iniciará uno nuevo)\n");
        printf("2. Calcular una nueva ruta\n");
        printf("3. Salir del programa\n");
        scanf("%d",&option);
        if (option>4 || option<0){
            printf("Introduzca un número de las opciones dadas\n");
            usleep(1000000);
        }
        if (option==1){
            free(adyacencia);
            adyacencia=NULL;
            free(arr_place);
            arr_place=NULL;
            adyacencia=loader(&arr_place,&size);
        }
        if (option==2){
            if (!adyacencia){
                fprintf(stderr,"LA MATRIZ DE ADYACENCIA ESTÁ VACÍA!");
                fprintf(stderr,"Cargue primero unos archivos en la primera opción antes de ejecutar esta");
                usleep(1000000);
            } else
                calculator(adyacencia,arr_place,size);
        }
        if (option==3) {
            free(adyacencia);
            free(arr_place);
            return;
        }
    }
}
