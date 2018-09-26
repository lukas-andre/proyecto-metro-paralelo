#include "dataloader.h"
#include "metrored.h"
#include <cstring>

/******************************************************************************************************************/
/*          OPERACIONES OPERACIONES SOBRE UNA LISTA DE TIPO RECORRIDO, AGREGAR, QUITAR, MOSTRAR, LARGO            */

// DOCUMENTAR
void agregarLineaRecorrido(recorrido *p_ruta, linea p_estacion) {
    recorrido auxiliar = *p_ruta;
    
    recorrido nuevo_nodo;
    nuevo_nodo = (recorrido) malloc(sizeof(ruta));
    nuevo_nodo->estacion = p_estacion;
    nuevo_nodo->link = NULL;
    
    if(auxiliar == NULL) {   
        *p_ruta = nuevo_nodo;        
    } else {
        while( auxiliar->link != NULL ) {
            auxiliar = auxiliar->link;
        }
        
        auxiliar->link = nuevo_nodo;
    }
}

// DOCUMENTAR
void pop(recorrido *combinaciones) {    
    recorrido aux = *combinaciones;
    recorrido tmp = *combinaciones;
    
    if ( tmp != NULL ) {
        aux = aux->link;
        *combinaciones = aux;
        free(tmp);
    }
    
}

// DOCUMENTAR
void mostrarRecorrido(recorrido p_ruta) {
    recorrido auxiliar = p_ruta;
    
    while( auxiliar != NULL ) {
        std::cout << auxiliar->estacion->nombre_linea << "\t"
                << auxiliar->estacion->codigo << "\t" 
                << auxiliar->estacion->nombre << std::endl;
        auxiliar = auxiliar->link;
    }
}

//DOCUMENTAR
int largoRecorrido(recorrido p_ruta) {
    int largo = 0;
    recorrido aux = p_ruta;
    
    while( aux != NULL ) {
        largo++;
        aux = aux->link;
    }
    
    return largo;
}

/*      FIN OPERACIONES OPERACIONES SOBRE UNA LISTA DE TIPO RECORRIDO, AGREGAR, QUITAR, MOSTRAR, LARGO            */
/******************************************************************************************************************/


/******************************************************************************************************************/
/*                                    INICIO OPERACIONES BASICAS                                                  */

bool destinoEnLinea(linea inicio, linea destino){
  return (inicio->nombre_linea == destino->nombre_linea) ? true : false;
}


std::string buscarDireccion(linea inicio, linea destino){
  return (inicio->id > destino->id) ? "izq" : "der";
}


linea getUbicacion(char* estacion, linea l1, linea l2, linea l4, linea l4a, linea l5, linea l6) {
    linea metroRed[6] = { l1, l2, l4, l4a, l5, l6 };

    for(int i = 0; i<6; i++) {
        linea recorre = metroRed[i];
        while( recorre != NULL ) {
            if ( estacion == recorre->codigo ) {
                return recorre;
            }
            recorre = recorre->der;
        }
    }
    
    return NULL;
}


int calcularDistanciaMismaLinea(linea inicio, linea destino){
    /* se calcula distancia si pertenecen a la misma linea previamente */
    return (inicio->id > destino->id) ? inicio->id - destino->id : destino->id - inicio->id;
}


void hacerCombinacion(linea *lx) {
    linea combinate = *lx;
    if(combinate->combinacion != NULL) {
        combinate = combinate->combinacion;
    }
    *lx = combinate;
}


void recorrerLinea(linea lx, std::string sentido, linea destino, recorrido *p_ruta) {
    linea estacion;
    estacion = lx;
    recorrido tmp = *p_ruta;
    /* se asume que esta en la misma linea */
    while ( estacion != destino ) {
        if (sentido.compare("der") == 0) {               
            agregarLineaRecorrido(&tmp, estacion);
            estacion = estacion->der;
        } else {
            agregarLineaRecorrido(&tmp, estacion);
            estacion = estacion->izq;
        }
    } 
    agregarLineaRecorrido(&tmp, estacion);
    
    
    *p_ruta = tmp;
}

// DOCUMEMTAR
void obtenerCombinacionesLinea(recorrido *combinaciones, linea lx) {
    recorrido r_comb = *combinaciones;
    linea l_right, l_left;
    l_right = lx;
    l_left = lx;
    
    
    while ( l_right != NULL ) {
        if ( l_right->combinacion && !inLista(r_comb, l_right)) {
            agregarLineaRecorrido(&r_comb, l_right);
        }
        l_right = l_right->der;
    }
    while ( l_left != NULL ) {
        if ( l_left->combinacion && !inLista(r_comb, l_left)) {
            agregarLineaRecorrido(&r_comb, l_left);
        }
        l_left = l_left->izq;
    }    
    *combinaciones = r_comb;
}

bool inLista(recorrido combinaciones, linea lx) {
    recorrido aux = combinaciones;
    bool state = false;
    while(aux != NULL) {
        if ( aux->estacion->nombre == lx->nombre ) {
            return true;
        }
        aux = aux->link;
    }
    return state;
}



/*                                  FIN INICIO OPERACIONES BASICAS                                                */
/******************************************************************************************************************/



/******************************************************************************************************************/
/*                                    INICIO CALCULAR RUTA                                                        */



void mostrarRutas(recorridos rutas){
    int cont = 0;
    while(rutas){
        std::cout<<"Mostrando Recorridos Encontrados"<<std::endl;
        std::cout<<"Recorrido Numero: "<< cont <<std::endl;
        mostrarRecorrido(rutas->estaciones);
        cont++;
        rutas = rutas->link;
    }
}


void guardarRuta(recorridos *ruta_ok, recorrido ruta){
    recorridos aux_ruta;
    recorridos nueva_ruta;
    aux_ruta = *ruta_ok;
    nueva_ruta = (recorridos) malloc(sizeof(rutas));
    nueva_ruta->estaciones = ruta;
    nueva_ruta->link = NULL;

    if (aux_ruta == NULL){
        *ruta_ok = nueva_ruta;
    } else {
        while ( aux_ruta->link != NULL){
            aux_ruta = aux_ruta->link;
        }
        aux_ruta->link = nueva_ruta;
    }
}

bool quedanCombinacionesARecorrer(recorrido combinaciones){
    int largo = obtenerLargoRecorrido(combinaciones);
    if (largo <= 10 ){
        return true;
    }
    return false;
}

int obtenerLargoRecorrido(recorrido lista){
    int contador = 0;
    while(lista){
        contador++;
        lista = lista->link;
    }
    return contador;
}

void agregarCombinacionesLimpias(recorrido *stack_combinaciones, recorrido nuevas_combinaciones){
    recorrido _stack_combinaciones = *stack_combinaciones;
    recorrido _stack_combinaciones_limpias = NULL;
    
    while(_stack_combinaciones){
        if(!inLista(_stack_combinaciones, nuevas_combinaciones->estacion)){
            agregarLineaRecorrido(&_stack_combinaciones_limpias, _stack_combinaciones->estacion);
        }
        _stack_combinaciones = _stack_combinaciones->link;
    }
    while(nuevas_combinaciones){
        if(!inLista(_stack_combinaciones, nuevas_combinaciones->estacion)){
            agregarLineaRecorrido(&_stack_combinaciones_limpias, nuevas_combinaciones->estacion);
        }
        nuevas_combinaciones = nuevas_combinaciones->link;
    }        
    *stack_combinaciones = _stack_combinaciones_limpias;
}

void eliminarCombinacionesConocidas(recorrido *nuevas_combinaciones, recorrido stack_combinaciones){
    recorrido combinaciones_limpias = NULL;
    recorrido combinaciones_entrantes = *nuevas_combinaciones;
    while (combinaciones_entrantes){
        if (!inLista(stack_combinaciones, combinaciones_entrantes->estacion)){
            agregarLineaRecorrido(&combinaciones_limpias, combinaciones_entrantes->estacion);
        } else {
            // std::cout<<"Combinacion ya conocida: "<< combinaciones_entrantes->estacion->nombre <<std::endl;
        }
        combinaciones_entrantes = combinaciones_entrantes->link;
    }
    *nuevas_combinaciones = combinaciones_limpias;
}

/*                                          FIN CALCULAR RUTA                                                     */
/******************************************************************************************************************/
