//
// Por Arturo Canga. V-25.696.222
// y Luis Fernandez. V-28.002.235
// Para AyPII, creado el 15/7/20
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dijkstra.h"

//Con el menú, ahora pasé todo lo que guarda lo principal a este archivo
//btw aprendí a pasar por referencia los arreglos
casilla ***loader(place **arr_places, int *n){
    char lugares[FILENAME_MAX];
    char clima[FILENAME_MAX];
    char c='\0';
    int m;
    printf("Introduzca el nombre del archivo de lugares y rutas: ");
    scanf("%s",lugares);
    *arr_places=getplaces(lugares,*arr_places,n);
    route **arr_routes=getroutes(lugares,arr_routes,&m);
    while ((c!='y' && c!='Y') && (c!='n' && c!='N')){
        system("clear");
        printf("Desea añadir un archivo de clima? (Y/N): ");
        c=getchar();
    }
    if (c=='y' || c=='Y'){

        do{
            printf("Añadir un archivo de clima modificará los valores de las rutas durante toda la "
                   "ejecución de este archivo. Está seguro de que desea continuar? (Y/N): ");
            c=getchar();
            c=getchar();
            if ((c!='y' && c!='Y') && (c!='n' && c!='N')) {
                printf("Por favor introduzca Y para sí o N para no");
                usleep(1000000);
            }
        } while ((c!='y' && c!='Y') && (c!='n' && c!='N'));

        if (c=='y' || c=='Y') {
            printf("Introduzca el nombre del archivo de clima: ");
            scanf("%s", clima);
            applyweather(clima,arr_routes,m);
        }
    }
    casilla ***returner=matrix(*arr_places,*n,arr_routes,m);
    free(arr_routes);
    return returner;
}

void calculator(casilla ***ady, place *arr_places, int n){
    char c;
    place inicio,llegada;
    int i,f;
    system("clear");
    cleaner(inicio);
    cleaner(llegada);
    do {
        printf("Listado de lugares:\n");
        for (int j = 0; j < n; ++j) {
            if (j==n-1)
                printf("%s\n\n",arr_places[j]);
            else
                printf("%s, ",arr_places[j]);
        }
        printf("Introduzca el lugar de partida: ");
        scanf("%s", inicio);
        i = placeposition(inicio, arr_places, n);
        if (i < 0) {
            fprintf(stderr, "ERROR: Dicho lugar no existe\n");
            usleep(1000000);
        }
        system("clear");
    } while (i<0);

    do {
        printf("Listado de lugares:\n");
        for (int j = 0; j < n; ++j) {
            if (j==n-1)
                printf("%s\n",arr_places[j]);
            else
                printf("%s, ",arr_places[j]);
        }
        printf("Lugar de inicio: %s\n",arr_places[i]);
        printf("Introduzca el lugar de llegada: ");
        scanf("%s", llegada);
        f = placeposition(llegada, arr_places, n);
        if (f < 0) {
            fprintf(stderr, "ERROR: Dicho lugar no existe\n");
            usleep(1000000);
        }
        //Añadí esto, por si ponen mismo lugar de inicio y llegada
        if (f==i){
            fprintf(stderr,"ERROR: El lugar de inicio y el de llegada son el mismo\nRegresando al menú...");
            usleep(1000000);
            return;
        }
        system("clear");
    } while (f<0);
    printer_p(n,i,f,arr_places,ady);
    printer_b(n,i,f,arr_places,ady);
    printer_c(n,i,f,arr_places,ady);
    printf("Pulse cualquier tecla para continuar...");
    getchar();
    getchar();
    //Cambié la función, ahora con menú es mejor que retorne en vez de preguntar infinitamente si quiere
    //volver a calcular
    system("clear");
}
