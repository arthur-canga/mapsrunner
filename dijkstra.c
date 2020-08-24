//
// Por Arturo Canga. V-25.696.222
// y Luis Fernandez. V-28.002.235
// Para AyPII, creado el 15/7/20
//

#include <stdlib.h>
#include <string.h>
#include "dijkstra.h"

float dijkstra_p(int n, casilla ***ady,int inicio, int fin, int *next){

    float **costo_p=(float **)malloc(sizeof(float *)*n);
    for (int i = 0; i < n; i++) {
        costo_p[i]=(float *)malloc(sizeof(float)*n);
    }
    float *distance=(float *)malloc(sizeof(float)*n);
    float min;
    //aux es el arreglo de marcado auxiliar
    int *aux=(int *)malloc(sizeof(int)*n);
    //prev es el arreglo de los nodos previos
    int *prev=(int *)malloc(sizeof(int)*n);

    //INICIALICÉ ESTO PORQUE SI TOCA UN NODO HUÉRFANO QUEDA SIN INICIALIZAR, EMBASURADO Y VIOLA SEGMENTO
    int sig=-1;

    //Inicialización de arreglos
    for(int i=0;i<n;i++) {
        for (int j = 0; j < n; j++) {
            if (ady[i][j]->p == 0)
                costo_p[i][j] = INF;
            else
                costo_p[i][j] = ady[i][j]->p;
        }
    }

    for(int i=0;i<n;i++){
        distance[i]=costo_p[inicio][i];
        prev[i]=inicio;
        aux[i]=0;
    }


    distance[inicio]=0;
    aux[inicio]=1;
    //Dijkstra itera n-2 veces
    for (int i = 1; i < n-1; ++i) {
        min = INF;
        //Chequeo de mínimos sin visitar
        for (int j = 0; j < n; j++) {
            if (distance[j] < min && !aux[j]) {
                min = distance[j];
                sig = j;
            }
        }
        if (sig==-1)
            return INF;

        aux[sig] = 1;
        for (int j = 0; j < n; j++) {
            if (!aux[j]) {
                //Comparo toda los posibles recorridos, si el recorrido es menor escribo
                if (min + costo_p[sig][j] < distance[j]) {
                    distance[j] = min + costo_p[sig][j];
                    prev[j] = sig;
                }
            }

        }
    }
    float final=distance[fin];
    free(distance);
    free(costo_p);
    free(aux);
    int j=fin;

    //Creo que para imprimir es más comodo hacerlos de siguientes en vez de previos, no?
    do {
        next[prev[j]]=j;
        j=prev[j];
    } while (j!=inicio);
    free(prev);
    return final;
}

//
float yen_p(int n, casilla ***ady, int inicio, int fin, const int *optimo, int *segundo){
    float yen=INF, seg=INF;
    //CHECK ES UN ARREGLO PARA CADA VEZ QUE LLAMO DIJKSTRA
    //SEGUNDO ES EL ARREGLO DE RESPUESTA
    int *check=(int *)malloc(sizeof(int)*n);
    //Inicializo
    for (int i = 0; i < n; i++) {
        segundo[i]=0;
    }
    for (int i = 0; i < n; i++) {
        check[i]=0;
    }
    int i=inicio;

    //Recorro TODO EL RECORRIDO ORIGINAL DE DIJKSTRA
    //Y por cada ruta, anulo su existencia temporalmente si existen y le paso Dijkstra de nuevo
    do {
        float restore=ady[i][optimo[i]]->p;
        ady[i][optimo[i]]->p=0;
        seg=dijkstra_p(n,ady,inicio,fin,check);
        ady[i][optimo[i]]->p=restore;
        //Si el Dijkstra es menor que el anterior, guardamos
        if (seg<yen){
            yen=seg;
            //Copia de arreglos
            for (int k = 0; k < n; ++k) {
                segundo[k]=check[k];
            }
        }
        //Avanzamos al siguiente
        i=optimo[i];
    }while (i!=fin);
    free(check);

    return yen;
}

//Esta funcion se explica sola, si no lo entiendes me escribes al priv
void printer_p(int n, int inicio, int fin, place *arr_places, casilla ***ady){
    int *arr_dij=(int *)malloc(n*sizeof(int));
    int *arr_yen=(int *)malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        arr_dij[i]=arr_yen[i]=-1;
    }
    float dij=dijkstra_p(n,ady,inicio,fin,arr_dij);
    float yen=yen_p(n,ady,inicio,fin,arr_dij,arr_yen);
    place guardar;
    strcpy(guardar,ady[1][0]->ruta);


    if (dij==INF){
        printf("No hay un camino para ir de %s a %s a pie\n",arr_places[inicio],arr_places[fin]);
        return;
    } else{
        printf("Costo del mejor camino para ir de %s a %s a pie: %.2f\n",arr_places[inicio],arr_places[fin],dij);
        printf("Ruta: %s",arr_places[inicio]);

        int i=inicio;
        do{
            printf(" --(por ruta %s)--> ",ady[i][arr_dij[i]]->ruta);
            printf("%s",arr_places[arr_dij[i]]);
            i=arr_dij[i];
        } while(i!=fin);
    }
    printf("\n");
    if (yen==INF){
        printf("No hay una segunda ruta óptima para ir de %s a %s a pie\n",arr_places[inicio],arr_places[fin]);
        return;
    } else{
        printf("Costo del camino alternativo para ir de %s a %s a pie: %.2f\n",arr_places[inicio],arr_places[fin],yen);
        printf("Ruta: %s",arr_places[inicio]);

        int i=inicio;
        do{
            printf(" --(por ruta %s)--> ",ady[i][arr_yen[i]]->ruta);
            printf("%s",arr_places[arr_yen[i]]);
            i=arr_yen[i];
        } while(i!=fin);
    }
    printf("\n\n");
    free(arr_dij);
    free(arr_yen);
}

//Ahora es que me vengo percatando que tu estructura es un asco y me obliga a repetir 3 funciones 3 veces
// pero ya estamos terminando así que no voy a cambiarla
float dijkstra_b(int n, casilla ***ady,int inicio, int fin, int *next){

    float **costo_p=(float **)malloc(sizeof(float *)*n);
    for (int i = 0; i < n; i++) {
        costo_p[i]=(float *)malloc(sizeof(float)*n);
    }
    float *distance=(float *)malloc(sizeof(float)*n);
    float min;
    int *aux=(int *)malloc(sizeof(int)*n);
    int *prev=(int *)malloc(sizeof(int)*n);
    int sig=-1;

    for(int i=0;i<n;i++) {
        for (int j = 0; j < n; j++) {
            if (ady[i][j]->b == 0)
                costo_p[i][j] = INF;
            else
                costo_p[i][j] = ady[i][j]->b;
        }
    }

    for(int i=0;i<n;i++){
        distance[i]=costo_p[inicio][i];
        prev[i]=inicio;
        aux[i]=0;
    }

    distance[inicio]=0;
    aux[inicio]=1;

    for (int i = 1; i < n-1; i++) {
        min = INF;
        for (int j = 0; j < n; j++) {
            if (distance[j] < min && !aux[j]) {
                min = distance[j];
                sig = j;
            }
        }
        if (sig==-1)
            return INF;

        aux[sig] = 1;
        for (int j = 0; j < n; j++) {
            if (!aux[j]) {
                if (min + costo_p[sig][j] < distance[j]) {
                    distance[j] = min + costo_p[sig][j];
                    prev[j] = sig;
                }
            }
        }
    }
    float final=distance[fin];
    free(distance);
    free(costo_p);
    free(aux);
    int j=fin;
    do {
        next[prev[j]]=j;
        j=prev[j];
    } while (j!=inicio);
    free(prev);
    return final;
}

float yen_b(int n, casilla ***ady, int inicio, int fin, const int *optimo, int *segundo){
    float yen=INF, seg=INF;
    int *check=(int *)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++) {
        segundo[i]=0;
    }
    for (int i = 0; i < n; i++) {
        check[i]=0;
    }
    int i=inicio;
    do {
        float restore=ady[i][optimo[i]]->b;
        ady[i][optimo[i]]->b=0;
        seg=dijkstra_b(n,ady,inicio,fin,check);
        ady[i][optimo[i]]->b=restore;
        if (seg<yen){
            yen=seg;
            for (int k = 0; k < n; ++k) {
                segundo[k]=check[k];
            }
        }
        i=optimo[i];
    }while (i!=fin);
    free(check);

    return yen;
}

void printer_b (int n, int inicio, int fin, place *arr_places, casilla ***ady){
    int *arr_dij=(int *)malloc(n*sizeof(int));
    int *arr_yen=(int *)malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        arr_dij[i]=arr_yen[i]=-1;
    }
    float dij=dijkstra_b(n,ady,inicio,fin,arr_dij);
    float yen=yen_b(n,ady,inicio,fin,arr_dij,arr_yen);


    if (dij==INF){
        printf("No hay un camino para ir de %s a %s en bus\n",arr_places[inicio],arr_places[fin]);
        return;
    } else{

        printf("Costo del mejor camino para ir de %s a %s en bus: %.2f\n",arr_places[inicio],arr_places[fin],dij);
        printf("Ruta: %s",arr_places[inicio]);

        int i=inicio;
        do{
            printf(" --(por ruta %s)--> ",ady[i][arr_dij[i]]->ruta);
            printf("%s",arr_places[arr_dij[i]]);
            i=arr_dij[i];
        } while(i!=fin);
    }
    printf("\n");
    if (yen==INF){
        printf("No hay una segunda ruta óptima para ir de %s a %s en bus\n",arr_places[inicio],arr_places[fin]);
        return;
    } else{
        printf("Costo del camino alternativo para ir de %s a %s en bus: %.2f\n",arr_places[inicio],arr_places[fin],yen);
        printf("Ruta: %s",arr_places[inicio]);

        int i=inicio;
        do{
            printf(" --(por ruta %s)--> ",ady[i][arr_yen[i]]->ruta);
            printf("%s",arr_places[arr_yen[i]]);
            i=arr_yen[i];
        } while(i!=fin);
    }
    printf("\n\n");
    free(arr_dij);
    free(arr_yen);
}

float dijkstra_c(int n, casilla ***ady,int inicio, int fin, int *next){

    float **costo_p=(float **)malloc(sizeof(float *)*n);
    for (int i = 0; i < n; i++) {
        costo_p[i]=(float *)malloc(sizeof(float)*n);
    }
    float *distance=(float *)malloc(sizeof(float)*n);
    float min;
    int *aux=(int *)malloc(sizeof(int)*n);
    int *prev=(int *)malloc(sizeof(int)*n);
    int sig=-1;

    for(int i=0;i<n;i++) {
        for (int j = 0; j < n; j++) {
            if (ady[i][j]->c == 0)
                costo_p[i][j] = INF;
            else
                costo_p[i][j] = ady[i][j]->c;
        }
    }

    for(int i=0;i<n;i++){
        distance[i]=costo_p[inicio][i];
        prev[i]=inicio;
        aux[i]=0;
    }

    distance[inicio]=0;
    aux[inicio]=1;

    for (int i = 1; i < n-1; i++) {
        min = INF;
        for (int j = 0; j < n; j++) {
            if (distance[j] < min && !aux[j]) {
                min = distance[j];
                sig = j;
            }
        }
        if (sig==-1)
            return INF;

        aux[sig] = 1;
        for (int j = 0; j < n; j++) {
            if (!aux[j]) {
                if (min + costo_p[sig][j] < distance[j]) {
                    distance[j] = min + costo_p[sig][j];
                    prev[j] = sig;
                }
            }
        }
    }
    float final=distance[fin];
    free(distance);
    free(costo_p);
    free(aux);
    int j=fin;
    do {
        next[prev[j]]=j;
        j=prev[j];
    } while (j!=inicio);
    free(prev);
    return final;
}

float yen_c(int n, casilla ***ady, int inicio, int fin, const int *optimo, int *segundo){
    float yen=INF, seg=INF;
    int *check=(int *)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++) {
        segundo[i]=0;
    }
    for (int i = 0; i < n; i++) {
        check[i]=0;
    }
    int i=inicio;
    do {
        float restore=ady[i][optimo[i]]->c;
        ady[i][optimo[i]]->c=0;
        seg=dijkstra_c(n,ady,inicio,fin,check);
        ady[i][optimo[i]]->c=restore;
        if (seg<yen){
            yen=seg;
            for (int k = 0; k < n; ++k) {
                segundo[k]=check[k];
            }
        }
        i=optimo[i];
    }while (i!=fin);
    free(check);

    return yen;
}

void printer_c(int n, int inicio, int fin, place *arr_places, casilla ***ady){
    int *arr_dij=(int *)malloc(n*sizeof(int));
    int *arr_yen=(int *)malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        arr_dij[i]=arr_yen[i]=-1;
    }
    float dij=dijkstra_c(n,ady,inicio,fin,arr_dij);
    float yen=yen_c(n,ady,inicio,fin,arr_dij,arr_yen);


    if (dij==INF){
        printf("No hay un camino para ir de %s a %s en carro\n",arr_places[inicio],arr_places[fin]);
        return;
    } else{
        printf("Costo del mejor camino para ir de %s a %s en carro: %.2f\n",arr_places[inicio],arr_places[fin],dij);
        printf("Ruta: %s ",arr_places[inicio]);

        int i=inicio;
        do{
            printf(" --(por ruta %s)--> ",ady[i][arr_dij[i]]->ruta);
            printf("%s",arr_places[arr_dij[i]]);
            i=arr_dij[i];
        } while(i!=fin);
    }
    printf("\n");
    if (yen==INF){
        printf("No hay una segunda ruta óptima para ir de %s a %s en carro\n",arr_places[inicio],arr_places[fin]);
        return;
    } else{
        printf("Costo del camino alternativo para ir de %s a %s en carro: %.2f\n",arr_places[inicio],arr_places[fin],yen);
        printf("Ruta: %s ",arr_places[inicio]);

        int i=inicio;
        do{
            printf(" --(por ruta %s)--> ",ady[i][arr_yen[i]]->ruta);
            printf("%s",arr_places[arr_yen[i]]);
            i=arr_yen[i];
        } while(i!=fin);
    }
    printf("\n\n");
    free(arr_dij);
    free(arr_yen);
}