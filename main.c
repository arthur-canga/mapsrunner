#include <stdio.h>
#include <stdlib.h>
#include "loader.h"

int main() {
    char lugares[FILENAME_MAX];
    char clima[FILENAME_MAX];
    char c='\0';
    int n,m;
    printf("Introduzca el nombre del archivo de lugares y rutas: \n");
    scanf("%s",lugares);
    place *arr_places=getplaces(lugares,arr_places,&n);
    route *arr_routes=getroutes(lugares,arr_routes,&m);
    while ((c!='y' && c!='Y') && (c!='n' && c!='N')){
        system("clear");
        printf("Desea añadir un archivo de clima? (Y/N): \n");
        c=getchar();
    }
    if (c=='y' || c=='Y'){
        printf("Está seguro? Añadir un archivo de clima modificará los valores de las rutas durante toda la "
               "ejecución del programa (Y/N)\n");
        c=getchar();
        if (c=='y' || c=='Y') {
            printf("Introduzca el nombre del archivo de clima");
            scanf("%s", clima);
        }
    }
    casilla **adyacencia=matrix(arr_places,n,arr_routes,m);
    c='Y';
    place inicio,llegada;
    while (c=='y' || c=='Y'){
        system("clear");
        cleaner(inicio);
        cleaner(llegada);
        printf("Introduzca el lugar de partida: ");
        scanf("%s",inicio);
        printf("Introduzca el lugar de llegada: ");
        scanf("%s",llegada);
        /*AQUÍ VAN LAS FUNCIONES DE BÚSQUEDA, MENSAJES DE ERROR, IMPRESIÓN DE RUTA, ETC.*/
        printf("Desea realizar nuevos cálculos? (Y/N)");
        c=getchar();
    }
    return 0;
}
