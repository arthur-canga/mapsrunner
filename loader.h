//
// Por Arturo Canga. V-25.696.222
// y Luis Fernandez. V-28.002.235
// Para AyPII, creado el 20/7/20
//

#include <stdio.h>
#define INF 1000000

typedef char place[30];

typedef struct route{
    place name;
    place origen;
    place destino;
    float p;
    float b;
    float c;
}route;

typedef struct casilla{
    place ruta;
    float p;
    float b;
    float c;
}casilla;

place *getplaces(char *, place *, int *);

route **getroutes(char *, route **, int *);

int placeposition(place, place *, int);

casilla ***matrix(place *, int, route **, int);

void applyweather(char *,route **,int);

void cleaner(place);