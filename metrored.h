#include <string>
#include <iostream>
#include "dataloader.h"


#ifndef METRORED_H
#define METRORED_H

   
    typedef struct ruta{
        linea estacion;
        struct ruta*link;
    }ruta;

    typedef ruta*recorrido;

    typedef struct rutas{
        recorrido estaciones;
        struct rutas*link;
    }rutas;

    typedef rutas*recorridos;

    void agregarLineaRecorrido(recorrido *p_ruta, linea p_estacion);
    void mostrarRecorrido(recorrido p_ruta);
    void testingRecorrerLinea(linea inicio, linea destino);
    void pop(recorrido *combinaciones);
    int largoRecorrido(recorrido p_ruta);
    bool inLista(recorrido combinaciones, linea lx);
    void mostrarRutas(recorridos rutas);
    void guardarRuta(recorridos *ruta_ok, recorrido ruta);
    void eliminarCombinacionesConocidas2(recorrido *faltantes_combinaciones, recorrido stack_combinaciones, recorrido combinaciones_conocidas);
    void buscarDestinoDesde(recorrido inicio_variable, linea destino, recorrido *stack_combinaciones, recorrido *combinaciones_conocidas, recorridos *rutas_ok, recorrido *rutas_pendientes );
    void eliminarCombinacionesConocidas(recorrido *nuevas_combinaciones, recorrido stack_combinaciones);

    void agregarCombinacionesLimpias(recorrido *stack_combinaciones, recorrido nuevas_combinaciones);
    int obtenerLargoRecorrido(recorrido lista);
    /**
    * Funcion que valida si existe destino en la misma linea consultada
    * @param inicio Estacion inicio
    * @param destrino Estacion destino
    * @return
    */
    bool destinoEnLinea(linea inicio, linea destino);
    
    /**
    * Funcion que genera la direccion a recorrer
    * @param inicio 
    * @param destino
    * @return
    */
    std::string buscarDireccion(linea incio, linea destino);
    
    /**
    * Funcion que recibe la estacion del argumento del programa y determina ubicacion en la linea de la estacion
    * @param estacion String argumento recibido
    * @param l1 Linea 1
    * @param l2 Linea 2
    * @param l4 Linea 4
    * @param l4a Linea 4a
    * @param l5 Linea 5
    * @param l6 Linea 6
    * @return Retorna la estacion en forma de linea(lista) donde se tienen todos los campos necesarios
    */
    linea getUbicacion(char* estacion, linea l1, linea l2, linea l4, linea l4a, linea l5, linea l6);
    
    /**
     * Funcion que retorna la distancia entre dos estaciones
     * @param inicio
     * @param destino
     * @return 
     */
    int calcularDistanciaMismaLinea(linea inicio, linea destino);
    
    /**
     * Funcion que realiza combinacion cambiando al nodo siguiente nodo*combinacion
     * @param lx
     */
    void hacerCombinacion(linea *lx);
   
    /**
     * Funcion encargada de recorrer la linea X, con la direccion/sentido por parámetro hacia destino 
     * y depositar toda la ruta en la lista p_ruta de tipo recorrido
     * @param lx Linea a recorrer
     * @param sentido Sentido/Direccion a moverse por la linea
     * @param destino Estacion destino
     * @param *p_ruta lista de tipo recorrido para generar ruta/camino 
     */
    void recorrerLinea(linea lx, std::string sentido, linea destino, recorrido *p_ruta);
    
    
    
    

#endif /* METRORED_H */

