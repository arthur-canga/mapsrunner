//
// Por Arturo Canga. V-25.696.222
// Para AyPII, creado el 20/7/20
//
//

#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleaner(place a){
    for (int i = 0; i < 30; i++) {
        a[i]='\0';
    }
}

place *getplaces(char *filename, place *arrplace, int *n){
    FILE *f;
    *n=0;
    f=fopen(filename,"r");
    if (!f){
        fprintf(stderr,"ERROR: EL ARCHIVO NO EXISTE. Programa finalizado");
        exit(1);
    }
    place buff;
    fgets(buff,FILENAME_MAX,f);
    if (strcmp(buff,"Lugares\n")!=0){
        fprintf(stderr,"ERROR: NO FUERON ENCONTRADOS LOS LUGARES EN EL ARCHIVO\n");
        fprintf(stderr,"Nota: es necesario que el titulo de los lugares este escrito de la manera \"Lugares\\n\"\n");
        fprintf(stderr,"Programa finalizado");
        exit(1);
    }
    cleaner(buff);
    //guardamos el inicio del primer lugar
    long int aux=ftell(f);
    //contamos los lugares
    fgets(buff,FILENAME_MAX,f);
    while (!strstr(buff,"Rutas") && !feof(f)){
        (*n)++;
        cleaner(buff);
        fgets(buff,FILENAME_MAX,f);
    }
    //si no hay lugares hay un error
    if (!(*n)){
        fprintf(stderr,"ERROR: No hay lugares en el archivo dado. Programa finalizado");
        exit(1);
    }
    //regresamos al inicio y empezamos a guardar
    fseek(f,aux,SEEK_SET);
    arrplace=(place *)malloc((*n)*sizeof(place));
    cleaner(buff);
    int i=0;
    fgets(buff,FILENAME_MAX,f);
    while(i<*n){
        strcpy(arrplace[i],buff);
        arrplace[i][strlen(buff)-1]='\0';
        i++;
        cleaner(buff);
        fgets(buff,FILENAME_MAX,f);
    }
    fclose(f);
    return arrplace;
}

route *getroutes(char *filename, route *arr_routes, int *m){
    //Verificacion de apertura del archivo
    char buff[FILENAME_MAX];
    FILE *f;
    f=fopen(filename,"r");
    if (!f){
        fprintf(stderr,"El archivo no existe. Programa finalizado");
        exit(1);
    }
    //Búsqueda de las rutas
    while (!strstr(buff,"Rutas\n")) {
        cleaner(buff);
        fgets(buff, FILENAME_MAX, f);
    }
    cleaner(buff);
    if (feof(f)){
        fprintf(stderr,"ERROR: No se encontraron rutas en el archivo.\n");
        fprintf(stderr,"NOTA: El titulo de los lugares tiene que estar escrito de la forma \"Lugares\\n\"\n");
        fprintf(stderr,"Programa finalizado");
        exit(1);
    }
    //Posicionamiento de las rutas y conteo de las rutas
    *m=0;
    long int aux=ftell(f);
    //CONTEO
    fgets(buff,FILENAME_MAX,f);
    while (!feof(f)){
        (*m)++;
        cleaner(buff);
        fgets(buff,FILENAME_MAX,f);
    }
    (*m)++;
    //Si no hay rutas
    if (!(*m)){
        fprintf(stderr,"ERROR: No hay rutas en el archivo dado. Programa finalizado");
        exit(1);
    }
    //Regresamos y empezamos a guardar las rutas en su arreglo respectivo
    arr_routes=(route *)malloc((*m)*sizeof(route));
    fseek(f,aux,SEEK_SET);
    cleaner(buff);
    fgets(buff,FILENAME_MAX,f);
    for (int j = 0; j < *m; j++) {
        sscanf(buff,"%[a-zA-Z_0-9]->%[a-zA-Z_0-9]:%[a-zA-Z_0-9]=P:%f,B:%f,C:%f",arr_routes[j].name,
                arr_routes[j].origen,arr_routes[j].destino,&(arr_routes[j].p),&(arr_routes[j].b),
                &(arr_routes[j].c));
        cleaner(buff);
        fgets(buff,FILENAME_MAX,f);
    }
    fclose(f);
    return arr_routes;
}

int placeposition(place sitio, place *arr_places, int n){
    for (int i = 0; i < n; ++i) {
        //Si encuentra el sitio entonces retorna su posicion
        if (!strcmp(sitio,arr_places[i]))
            return i;
    }
    //Sino retorna un valor imposible para el arreglo
    return -1;
}

casilla **matrix(place *arr_places, int n, route *arr_routes, int m){
    casilla **matriz=(casilla **)malloc(n*sizeof(float));
    for (int i = 0; i < n; i++) {
        matriz[i]=(casilla *)malloc(n*sizeof(float));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i==j) {
                matriz[i][j].p=0;
                matriz[i][j].b=0;
                matriz[i][j].c=0;
            } else {
                matriz[i][j].p=INF;
                matriz[i][j].b=INF;
                matriz[i][j].c=INF;
            }
        }
    }

    for (int k = 0; k < m; k++) {
        int positionorigen,positiondestino;
        positionorigen=placeposition(arr_routes[k].origen,arr_places,n);
        positiondestino=placeposition(arr_routes[k].destino,arr_places,n);
        if (positionorigen<0 || positiondestino<0){
            fprintf(stderr,"ERROR: Una de las rutas tiene un lugar de origen o destino no designado anteriormente\n");
            fprintf(stderr,"Necesita indicar este sitio en el apartado de lugares del archivo\n");
            fprintf(stderr,"El/los sitios no definidos son los siguientes:\n");
            if (positionorigen<0)
                fprintf(stderr,"%s en la ruta %s\n",arr_routes[k].origen,arr_routes[k].name);
            if (positiondestino<0)
                fprintf(stderr,"%s en la ruta %s\n",arr_routes[k].destino,arr_routes[k].name);
            fprintf(stderr,"Programa finalizado");
            exit(1);
        } else {
            strcpy(matriz[positionorigen][positiondestino].ruta, arr_routes[k].name);
            matriz[positionorigen][positiondestino].p=arr_routes[k].p;
            matriz[positionorigen][positiondestino].b=arr_routes[k].b;
            matriz[positionorigen][positiondestino].c=arr_routes[k].c;
        }
    }
    return matriz;

}

void applyweather(char *weather,route *arr_routes,int n){
    FILE *w;
    w=fopen(weather,"r");
    char buff[FILENAME_MAX];
    fgets(buff,FILENAME_MAX,w);
    place name;
    float p,b,c;
    sscanf(buff,"%[a-zA-Z_0-9]=P:%f,B:%f,C:%f",name,&p,&b,&c);
    for (int i = 0; i < n; i++) {
        if (!strcmp(name,arr_routes[i].name)){
            arr_routes[i].p*=p;
            arr_routes[i].b*=b;
            arr_routes[i].c*=c;
            return;
        }
    }
    fprintf(stderr,"ADVERTENCIA: No se encontró el archivo %s, se procederá sin modificaciones climáticas\n",weather);
}