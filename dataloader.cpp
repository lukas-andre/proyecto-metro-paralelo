#include <cstring>

#include "dataloader.h"


void cargarLinea(char* fichero, linea *lx, char* nombreLinea){
    linea L = *lx;

    std::ifstream archivo(fichero);
    char line[35];
    int contador = 0;

    if(archivo.fail()) {
        std::cerr << "Error al abrir el archivo " << fichero << std::endl;
    } else {
        while(!archivo.eof())
        {
            archivo.getline(line, sizeof(line));
            std::string lineaArchivo = line;
            int largoLinea = lineaArchivo.length();

            linea nuevaEstacion;
            nuevaEstacion = (linea) malloc(sizeof(nodo));

            nuevaEstacion->id = contador;
            /* SIGLA DE LARGO 3 TRUE, LARGO 2 FALSE */
            nuevaEstacion->codigo = ( lineaArchivo[2] != ' ' ) ? lineaArchivo.substr(0,3) : lineaArchivo.substr(0,2);
            nuevaEstacion->nombre = ( lineaArchivo[2] != ' ' ) ? lineaArchivo.substr(4, largoLinea-4) : lineaArchivo.substr(3, largoLinea-3);
            nuevaEstacion->nombre_linea = nombreLinea;
            nuevaEstacion->combinacion = NULL;
            nuevaEstacion->der = NULL;

            if (L == NULL) {
                /* SI LA LISTA VIENE VACIA, NUEVA ESTACION ES EL PRIMER NODO */
                L = nuevaEstacion;
                nuevaEstacion->izq = NULL;
            } else {
                /* LINKEA LA LISTA QUE VIENE RECORRIENDO, A LA NUEVA ESTACION */
                nuevaEstacion->izq = L;
                L->der = nuevaEstacion;
            }
            L = L->der;
            contador++;

        }
    }
    archivo.close();

}


void imprimirLinea(linea lx) {
    linea L;
    L = lx;
    
    std::cout << L->nombre_linea << std::endl << std::endl;
    while(L != NULL) {
        std::cout << "id: " << L->id << "\tcod: " << L->codigo << "\t nombre: " << L->nombre << " \t linea: " << L->nombre_linea;

        /* verificar si hay nodo estacion para evitar core */
        if (L->combinacion != NULL) {
            std::cout << "\t\tCombinacion: " << L->combinacion->codigo;
        } else {
            std::cout << "\t\tNo hay combinacion";
        }

        /* verificar si existe nodo izquierdo para evitar core */
        if (L->izq != NULL) {
            std::cout << "\t\t\tAnterior: " << L->izq->nombre;
        } else {
            std::cout << "\t\t\tEs la primera estacion";
        }

        /* verificar si hay nodo derecho para evitar core */
        if (L->der != NULL) {
            std::cout << "\t\t\tSiguiente: " << L->der->nombre << std::endl;
        } else {
            std::cout << "\t\t\tEs la ultima estacion" << std::endl;
        }



        L = L->der;

    }

}


void cleanLinea(linea *lx) {
    linea primeraEstacion = *lx;
    linea borrar = *lx;

    primeraEstacion = primeraEstacion->der;

    primeraEstacion->izq = NULL;
    *lx = primeraEstacion;

    free(borrar);

}


void buscarCombinaciones(linea *lx, linea *ln) {
    linea aux = *lx;

    /* Buscar en Linea X para cada estación */
    while( aux != NULL ) {

        /* Buscar en Linea N para cada estación */
        linea search = *ln;

        while( search != NULL ) {

            if ( aux->nombre == search->nombre ) {
                /* Asignacion de valores a ambas estaciones con codigos de la otra linea */
                aux->combinacion = search;
                search->combinacion = aux;
            }

            search = search->der;
        }

        aux = aux->der;
    }

}


void generarTodasCombinaciones(linea *l1, linea *l2, linea *l4, linea *l4a, linea *l5, linea *l6) {
    linea metroRed[6] = { *l1, *l2, *l4, *l4a, *l5, *l6 };
    for(int i = 0; i<6; i++) {
        for(int j = 0; j<6; j++) {
            if ( i != j ) {
                buscarCombinaciones(&metroRed[i], &metroRed[j]);
            }
        }
    }
}


void generarLineasMetro(linea *l1, linea *l2, linea *l4, linea *l4a, linea *l5, linea *l6){
    /* Se declaran los nombres de los textos para abrir archivos (evitar warnings) */
    char* tl1 = NULL;
    char* tl2 = NULL;
    char* tl4 = NULL;
    char* tl4a = NULL;
    char* tl5 = NULL;
    char* tl6 = NULL;

    tl1 = (char *) calloc(20, sizeof (char));
    tl2 = (char *) calloc(20, sizeof (char));
    tl4 = (char *) calloc(20, sizeof (char));
    tl4a = (char *) calloc(20, sizeof (char));
    tl5 = (char *) calloc(20, sizeof (char));
    tl6 = (char *) calloc(20, sizeof (char));

    /* Setear el path de los ficheros */
    snprintf(tl1, 20, "./public/L1.txt");
    snprintf(tl2, 20, "./public/L2.txt");
    snprintf(tl4, 20, "./public/L4.txt");
    snprintf(tl4a, 20, "./public/L4a.txt");
    snprintf(tl5, 20, "./public/L5.txt");
    snprintf(tl6, 20, "./public/L6.txt");
    
    /* generar variables "L1", "L2", etc.. para evitar warnings */
    char* L1 = NULL;
    char* L2 = NULL;
    char* L4 = NULL;
    char* L4a = NULL;
    char* L5 = NULL;
    char* L6 = NULL;
    
    L1 = (char *) calloc(3, sizeof (char));
    L2 = (char *) calloc(3, sizeof (char));
    L4 = (char *) calloc(3, sizeof (char));
    L4a = (char *) calloc(4, sizeof (char));
    L5 = (char *) calloc(3, sizeof (char));
    L6 = (char *) calloc(3, sizeof (char));

    snprintf(L1, 3, "L1");
    snprintf(L2, 3, "L2");
    snprintf(L4, 3, "L4");
    snprintf(L4a, 4, "L4a");
    snprintf(L5, 3, "L5");
    snprintf(L6, 3, "L6");
    /* Fin L1, L2,... */

    /* Se cargan las lineas del metro con su correspondiente fichero */
    cargarLinea(tl1, l1, L1);
    cargarLinea(tl2, l2, L2);
    cargarLinea(tl4, l4, L4);
    cargarLinea(tl4a, l4a, L4a);
    cargarLinea(tl5, l5, L5);
    cargarLinea(tl6, l6, L6);
    
    /* Limpiar las lineas */
    cleanLinea(l1);
    cleanLinea(l2);
    cleanLinea(l4);
    cleanLinea(l4a);
    cleanLinea(l5);
    cleanLinea(l6);

    /* Cargar nodo*combinacion de cada estacion */
    generarTodasCombinaciones(l1,l2,l4,l4a,l5,l6);

    /* Ver por pantalla */
    
//    imprimirLinea(*l1);
//    imprimirLinea(*l2);
//    imprimirLinea(*l4);
//    imprimirLinea(*l4a);
//    imprimirLinea(*l5);
//    imprimirLinea(*l6);

}
