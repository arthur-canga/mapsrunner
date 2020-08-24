//
// Por Arturo Canga. V-25.696.222
// y Luis Fernandez. V-28.002.235
// Para AyPII, creado el 20/7/20
//

#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//limpiador de place
void cleaner(place a){
    for (int i = 0; i < 30; i++) {
        a[i]='\0';
    }
}

//esto chequea si el lugar esta bien escrito
int checkplace_alphanumeric(const char *s){
    for (int i = 0; s[i]!='\n' ; ++i) {
        for (char j = 32; j <= 47 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 58; j <= 64 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 91; j <= 96 ; j++) {
            if (s[i]==j && j!=95)
                return 0;
        }
        for (char j = 123; j <= 126 ; j++) {
            if (s[i]==j)
                return 0;
        }
    }
    return 1;
}

//esto es pa chequea si la ruta tiene el formato
//HABLAME YANDEREDEV, TERMINA EL JUEGO Y DEJA DE DAR PENA EN TWITCH
int checkroute_alphanumeric(const char *s){
    //are ya coding son?
    int i, flagdot=0;
    for (i = 0; s[i]!='-' && i<FILENAME_MAX; i++) {
        for (char j = 32; j <= 47 ; j++) {
            //Yes dad
            if (s[i]==j && j!=45)
                return 0;
        }
        for (char j = 58; j <= 64 ; j++) {
            //Yes dad
            if (s[i]==j)
                return 0;
        }
        for (char j = 91; j <= 96 ; j++) {
            //Yes dad
            if (s[i]==j && j!=95)
                return 0;
        }
        for (char j = 123; j <= 126 ; j++) {
            //Yes dad
            if (s[i]==j)
                return 0;
        }
    }
    i++;
    if (s[i]!='>' || i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]==':' || i>=FILENAME_MAX)
        return 0;
    for (; s[i]!=':' && i<FILENAME_MAX; i++) {
        for (char j = 32; j <= 47 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 59; j <= 64 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 91; j <= 96 ; j++) {
            if (s[i]==j && j!=95)
                return 0;
        }
        for (char j = 123; j <= 126 ; j++) {
            if (s[i]==j)
                return 0;
        }
    }
    i++;
    if (s[i]=='=' || i>=FILENAME_MAX)
        return 0;
    for (; s[i]!='=' && i<FILENAME_MAX; i++) {
        for (char j = 32; j <= 47 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 58; j <= 64 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 91; j <= 96 ; j++) {
            if (s[i]==j && j!=95)
                return 0;
        }
        for (char j = 123; j <= 126 ; j++) {
            if (s[i]==j)
                return 0;
        }
    }
    i++;
    if (s[i]!='P' || i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]!=':' || i>=FILENAME_MAX)
        return 0;
    i++;
    for (; s[i]!=';' && i<FILENAME_MAX; i++) {
        if ((s[i]<'0' || s[i]>'9') && s[i]!='.')
            return 0;
        if (flagdot>0 && s[i]=='.')
            return 0;
        if (s[i]=='.') {
            flagdot==1;
        }
    }
    i++;
    if (s[i]!='B' || i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]!=':' || i>=FILENAME_MAX)
        return 0;
    i++;
    flagdot=0;
    for (; s[i]!=';' && i<FILENAME_MAX; i++) {
        if ((s[i]<'0' || s[i]>'9') && s[i]!='.')
            return 0;
        if (flagdot>0 && s[i]=='.')
            return 0;
        if (s[i]=='.') {
            flagdot==1;
        }
    }
    i++;
    if (s[i]!='C' || i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]!=':' || i>=FILENAME_MAX)
        return 0;
    i++;
    flagdot=0;
    for ( ;s[i]!='\n' && s[i]!='\0' && i<FILENAME_MAX ; i++){
        if ((s[i]<'0' || s[i]>'9') && s[i]!='.')
            return 0;
        if (flagdot>0 && s[i]=='.')
            return 0;
        if (s[i]=='.') {
            flagdot==1;
        }
    }
    return 1;
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
    //el título me marca el inicio de los lugares
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
        fprintf(stderr,"ERROR: No hay lugares en el archivo dado. Programa finalizado\n");
        exit(1);
    }
    //regresamos al inicio y empezamos a guardar
    fseek(f,aux,SEEK_SET);
    arrplace=(place *)malloc((*n)*sizeof(place));
    cleaner(buff);
    int i=0; int pos=0;
    char help;
    while(i < *n) {
        fgets(buff,FILENAME_MAX,f);
        if (!checkplace_alphanumeric(buff)) {
            fprintf(stderr, "ERROR: El lugar %d no cumple con el formato respectivo\n", i);
            fprintf(stderr,"Los lugares solo pueden tener caracteres alfanuméricos y pisos\n");
            fprintf(stderr, "Programa finalizado\n");
            exit(1);
        }
        int flagrep=0;
        if (i>0){
            for (int j = 0; j < pos; j++) {
                if (!strcmp(arrplace[j],buff))
                    flagrep++;
            }
        }
        if (!flagrep){
            strcpy(arrplace[pos],buff);
            arrplace[pos][strlen(buff)-1]='\0';
            help=arrplace[1][0];
            pos++;
        } else{
            fprintf(stderr,"ADVERTENCIA: Lugar %d repetido. No tomado\n",i);
        }
        help=arrplace[1][0];
        i++;
        cleaner(buff);
        help=arrplace[1][0];

    }
    help=arrplace[1][0];
    arrplace=(place *)realloc(arrplace,(pos+1)*sizeof(place));
    help=arrplace[1][0];
    fclose(f);
    return arrplace;
}
//a->a:a=P:n,B:n,C:n

route **getroutes(char *filename, route **arr_routes, int *m){
    //verificacion de apertura del archivo
    char buff[FILENAME_MAX];
    FILE *f;
    f=fopen(filename,"r");
    if (!f){
        fprintf(stderr,"El archivo no existe. Programa finalizado\n");
        exit(1);
    }
    //búsqueda de las rutas
    while (!strstr(buff,"Rutas\n")) {
        cleaner(buff);
        fgets(buff, FILENAME_MAX, f);
    }
    cleaner(buff);
    if (feof(f)){
        fprintf(stderr,"ERROR: No se encontraron rutas en el archivo.\n");
        fprintf(stderr,"NOTA: El titulo de los lugares tiene que estar escrito de la forma \"Lugares\"\n");
        fprintf(stderr,"Programa finalizado\n");
        exit(1);
    }
    //posicionamiento de las rutas y conteo de las rutas
    *m=0;
    long int aux=ftell(f);
    //conteo
    fgets(buff,FILENAME_MAX,f);
    while (!feof(f)){
        (*m)++;
        cleaner(buff);
        fgets(buff,FILENAME_MAX,f);
    }
    (*m)++;
    //si no hay rutas
    if (!(*m)){
        fprintf(stderr,"ERROR: No hay rutas en el archivo dado. Programa finalizado\n");
        exit(1);
    }
    //Regresamos y empezamos a guardar las rutas en su arreglo respectivo
    arr_routes=(route **)malloc((*m)*sizeof(route *));
    for (int i = 0; i < (*m); ++i) {
        arr_routes[i]=(route *)malloc(sizeof(route));
    }
    fseek(f,aux,SEEK_SET);
    cleaner(buff);
    fgets(buff,FILENAME_MAX,f);
    int i=0, pos=0;
    while (i < *m) {
        if (!checkroute_alphanumeric(buff)) {
            fprintf(stderr, "ERROR: La ruta %d no cumple con el formato respectivo\n", i);
            fprintf(stderr, "Verifique que los nombres de la ruta sean todos alfanuméricos y la estructura cumpla con"
                            "la forma {$ROUTE}->{$ORIGIN}:{$DESTINY}=P:{$NUMBER},B:{$NUMBER},C:{$NUMBER}\n");
            fprintf(stderr, "Programa finalizado\n");
            exit(1);
        }
        int flagrep=0;
        place name,og,dest;
        float p,b,c;
        sscanf(buff,"%[a-zA-Z_0-9]->%[a-zA-Z_0-9]:%[a-zA-Z_0-9]=P:%f,B:%f,C:%f",name,og,dest,&p,&b,&c);
        if (pos>0){
            for (int j = 0; j < pos; j++) {
                if (!strcmp(arr_routes[j]->name,name))
                    flagrep++;
            }
        }
        if (flagrep>0) {
            fprintf(stderr, "ADVERTENCIA: Ruta %d repetida. No tomado\n", i);
        } else {
            if (og[0] == '\0' || dest[0] == '\0' || name[0] == '\0') {
                fprintf(stderr, "ERROR: Uno de los nombres de las rutas está en blanco\n");
                fprintf(stderr, "Programa finalizado\n");
                exit(1);
            }
            if (!p && !b && !c) {
                fprintf(stderr, "ADVERTENCIA: Una de las rutas introducidas tiene componentes P, B y C iguales a 0\n");
                fprintf(stderr, "Esta ruta no sera interpretada por el programa\n");
            }
            strcpy(arr_routes[pos]->name,name);
            strcpy(arr_routes[pos]->origen,og);
            strcpy(arr_routes[pos]->destino,dest);
            arr_routes[pos]->p=p;
            arr_routes[pos]->b=b;
            arr_routes[pos]->c=c;
            cleaner(buff);
            cleaner(name);
            cleaner(og);
            cleaner(dest);
            pos++;
            fgets(buff, FILENAME_MAX, f);
        }
        i++;
    }
    fclose(f);
    return arr_routes;
}

int placeposition(place sitio, place *arr_places, int n){
    for (int i = 0; i < n; ++i) {
        place help="\0";
        strcpy(help,arr_places[i]);
        //si encuentra el sitio entonces retorna su posicion
        if (strcmp(sitio,arr_places[i])==0)
            return i;
    }
    //si no retorna un valor imposible para el arreglo
    return -1;
}

//Me gustó esta función
casilla ***matrix(place *arr_places, int n, route **arr_routes, int m){
    casilla ***matriz=(casilla ***)malloc(n*sizeof(casilla **));
    for (int i = 0; i < n; i++) {
        matriz[i]=(casilla **)malloc(n*sizeof(casilla *));
        for (int j = 0; j < n; ++j) {
            matriz[i][j]=(casilla *)malloc(n*sizeof(casilla));
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            strcpy(matriz[i][j]->ruta,"\0");
            matriz[i][j]->p=0;
            matriz[i][j]->b=0;
            matriz[i][j]->c=0;
        }
    }

    for (int k = 0; k < m; k++) {
        int positionorigen,positiondestino;
        positionorigen=placeposition(arr_routes[k]->origen,arr_places,n);
        positiondestino=placeposition(arr_routes[k]->destino,arr_places,n);
        if (positionorigen<0 || positiondestino<0){
            fprintf(stderr,"ERROR: Una de las rutas tiene un lugar de origen o destino no designado anteriormente\n");
            fprintf(stderr,"Necesita indicar este sitio en el apartado de lugares del archivo\n");
            fprintf(stderr,"El/los sitios no definidos son los siguientes:\n");
            if (positionorigen<0)
                fprintf(stderr,"%s en la ruta %s\n",arr_routes[k]->origen,arr_routes[k]->name);
            if (positiondestino<0)
                fprintf(stderr,"%s en la ruta %s\n",arr_routes[k]->destino,arr_routes[k]->name);
            fprintf(stderr,"Programa finalizado\n");
            exit(1);
        } else {


            if (matriz[positionorigen][positiondestino]->ruta[0]!='\0'){
                fprintf(stderr,"ADVERTENCIA: Se detectó que hay rutas con los mismos lugares de origen y destino\n");
                fprintf(stderr,"Estas van a ser sobreescritas\n");
            }


            strcpy(matriz[positionorigen][positiondestino]->ruta, arr_routes[k]->name);
            matriz[positionorigen][positiondestino]->p=arr_routes[k]->p;
            matriz[positionorigen][positiondestino]->b=arr_routes[k]->b;
            matriz[positionorigen][positiondestino]->c=arr_routes[k]->c;

        }
    }

    return matriz;

}

int checkweather_alphanumeric(const char *s){
    int i,flagdot=0;
    for (i = 0; s[i]!='=' && i<FILENAME_MAX; i++) {
        for (char j = 32; j <= 47 ; j++) {
            if (s[i]==j && j!=45)
                return 0;
        }
        for (char j = 58; j <= 64 ; j++) {
            if (s[i]==j)
                return 0;
        }
        for (char j = 91; j <= 96 ; j++) {
            if (s[i]==j && j!=95)
                return 0;
        }
        for (char j = 123; j <= 126 ; j++) {
            if (s[i]==j)
                return 0;
        }
    }
    i++;
    if (s[i]!='P'|| i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]!=':'|| i>=FILENAME_MAX)
        return 0;
    i++;
    for (;s[i]!=';' && i<FILENAME_MAX ; i++){
        if ((s[i]<'0' || s[i]>'9') && s[i]!='.')
            return 0;
        if (flagdot>0 && s[i]=='.')
            return 0;
        if (s[i]=='.') {
            flagdot==1;
        }
    }
    i++;
    if (s[i]!='B'|| i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]!=':'|| i>=FILENAME_MAX)
        return 0;
    i++;
    flagdot=0;
    for (;s[i]!=';' && i<FILENAME_MAX ; i++){
        if ((s[i]<'0' || s[i]>'9') && s[i]!='.')
            return 0;
        if (flagdot>0 && s[i]=='.')
            return 0;
        if (s[i]=='.')
            flagdot==1;
    }
    i++;
    if (s[i]!='C'|| i>=FILENAME_MAX)
        return 0;
    i++;
    if (s[i]!=':'|| i>=FILENAME_MAX)
        return 0;
    i++;
    flagdot=0;
    for (;s[i]!='\n' && s[i]!='\0' && i<FILENAME_MAX ; i++){
        if ((s[i]<'0' || s[i]>'9') && s[i]!='.')
            return 0;
        if (flagdot>0 && s[i]=='.')
            return 0;
        if (s[i]=='.')
            flagdot==1;
    }
    return 1;
}

//Función del clima, mismo principio que las otras funciones de lectura de archivos
void applyweather(char *weather,route **arr_routes,int n){
    FILE *w;
    w=fopen(weather,"r");
    if (!w){
        fprintf(stderr,"ADVERTENCIA: No se encontró el archivo %s, se procederá sin modificaciones climáticas\n",weather);
        return;
    }
    char buff[FILENAME_MAX];
    place name;
    float p,b,c;
    while (!feof(w)){
        fgets(buff,FILENAME_MAX,w);
        if (!checkweather_alphanumeric(buff)){
            fprintf(stderr,"ERROR: Una de las líneas del archivo de clima no cumple con la estructura requerida\n");
            fprintf(stderr,"Programa finalizado\n");
            exit(1);
        }
        sscanf(buff,"%[a-zA-Z_0-9]=P:%f,B:%f,C:%f",name,&p,&b,&c);
        for (int i = 0; i < n; i++) {
            if (!strcmp(name,arr_routes[i]->name)){
                arr_routes[i]->p*=p;
                arr_routes[i]->b*=b;
                arr_routes[i]->c*=c;
                break;
            }
            if (i==n-1) {
                fprintf(stderr, "ERROR: Una de las rutas en el archivo de clima no existe. Programa finalizado\n");
                exit(1);
            }
        }
    }
}
