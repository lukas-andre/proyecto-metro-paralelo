#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

    
    typedef struct nodo {
        int id;
        std::string codigo;
        std::string nombre;
        std::string nombre_linea;
        struct nodo*izq;
        struct nodo*der;
        struct nodo*combinacion;
    }nodo;

    typedef nodo*linea;
    
    

    /**
     * Funcion que carga una linea desde un fichero a una lista
     * @param fichero "./public/Lx.txt"
     * @param lx Linea vacia a la cual se le cargan las estaciones
     */
    void cargarLinea(char* fichero, linea *lx, char* nombreLinea);

    /**
    * Funcion para eliminar el primer nodo de la linea que aparecia en blanco
    * @param lx Linea a la cual se le limpia el nodo
    */
    void cleanLinea(linea *lx);

    /**
    * Funcion que imprime los datos de cada estacion de una linea por parametro
    * @param lx Linea que se de sea imprimir cada estacion
    */
    void imprimirLinea(linea lx);

    /**
     * Funcion que busca las combinaciones existentes entre dos lineas
     * @param lx Linea x
     * @param ln Linea n
     */
    void buscarCombinaciones(linea* lx, linea* ln);

    /*
    * Funcion que recibe todas las lineas del metro y comprueba si tiene combinacion con las demás para generar los link
    * @param l1 Linea 1
    * @param l2 Linea 2
    * @param l4 Linea 4
    * @param l4a Linea 4a
    * @param l5 Linea 5
    * @param l6 Linea 6
    */
    void generarTodasCombinaciones(linea *l1, linea *l2, linea *l4, linea *l4a, linea *l5, linea *l6);
    
    /**
     * Funcion que genera toda la red de metro recibiendo lineas vacias
     * @param l1 Linea 1
     * @param l2 Linea 2
     * @param l4 Linea 4
     * @param l4a Linea 4a
     * @param l5 Linea 5
     * @param l6 Linea 6
     */
    void generarLineasMetro(linea *l1, linea *l2, linea *l4, linea *l4a, linea *l5, linea *l6);
    
    

#endif /* FUNCIONES_H */
