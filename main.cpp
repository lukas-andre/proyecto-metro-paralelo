#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#include "dataloader.h"
#include "metrored.h"

# include <mpi.h>


using namespace std;

int cantidad_argumentos;
char** argumentos;

int id;
int ierr;
int rango_origen;
int rango_destino;
int etiqueta;
MPI_Status estado;



int main(int argc, char** argv) {
  int p_rank;
  int p_size;
  // Se inicializa MPI
  MPI_Init(0,0);
  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
  if( argc < 2 ){
      cout << "Debe ingresar al menos 1 argumento" << endl;
  }else {
    if(string(argv[1])  == "-f"){
        if(argc == 4){
          //Rank 0 se encarga de encontrar las posibles rutas hasta el destino e imprime la ruta con menor largo luego de recibir la respuesta de Rank 1
          if (p_rank==0){
            linea l1, l2, l4, l4a, l5, l6;

            l1 = (linea) malloc(sizeof (nodo));
            l2 = (linea) malloc(sizeof (nodo));
            l4 = (linea) malloc(sizeof (nodo));
            l4a = (linea) malloc(sizeof (nodo));
            l5 = (linea) malloc(sizeof (nodo));
            l6 = (linea) malloc(sizeof (nodo));

            generarLineasMetro(&l1, &l2, &l4, &l4a, &l5, &l6);

            linea inicio, destino;
            inicio = (linea) malloc(sizeof (nodo));
            destino = (linea) malloc(sizeof (nodo));

            inicio = getUbicacion(argv[2], l1, l2, l4, l4a, l5, l6);
            destino = getUbicacion(argv[3], l1, l2, l4, l4a, l5, l6);

            cout << "Estación de inicio  : " << inicio->nombre << endl;
            cout << "Estación destino    : " << destino->nombre << endl << endl;

            // testingRecorrerLinea(inicio, destino);
            linea estacion_inicio;
            estacion_inicio = inicio;
            
            recorrido combinaciones = NULL; 
            recorrido p_ruta = NULL;
            recorrido stack_combinaciones = NULL;
          

            if (destinoEnLinea(estacion_inicio, destino)) {
                std::string direccion;
                direccion = buscarDireccion(estacion_inicio, destino);
                recorrerLinea(estacion_inicio, direccion, destino, &p_ruta);
                mostrarRecorrido(p_ruta);
            }
            else{
                obtenerCombinacionesLinea(&combinaciones, estacion_inicio);
                obtenerCombinacionesLinea(&stack_combinaciones, estacion_inicio);
                recorrido combinaciones_conocidas = NULL;
                recorridos rutas_ok = NULL;
                recorrido ruta_pendiente = NULL;
                recorrido tmp = combinaciones;
                int largo_combinaciones = obtenerLargoRecorrido(combinaciones);
                std::cout<<"mostrando combinaciones de: "<< inicio->nombre <<std::endl;
                mostrarRecorrido(combinaciones);
                std::cout<<"Total Combinaciones: "<< largo_combinaciones <<std::endl;
                for(unsigned i = 0; i < largo_combinaciones; i++){
                  // recorrerLinea(
                  //     inicio,
                  //     buscarDireccion(inicio, tmp->estacion),
                  //     tmp->estacion,
                  //     &ruta_pendiente
                  //     );
                  
                  std::cout<<"P: "<< p_rank <<std::endl;
                  std::cout<<"cont: "<< i <<std::endl;
                  MPI_Send(&tmp->estacion->codigo.c_str(), 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);  
                  // MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
                  // MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
                  // MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
                    // switch(rank):
                    
                }
                // recorrido _stack_combinaciones_inicial = stack_combinaciones;
                // buscarDestinoDesde(tmp, destino, &stack_combinaciones, &combinaciones_conocidas, &rutas_ok, &ruta_pendiente);
                // ruta_pendiente = NULL;
                // combinaciones_conocidas = NULL;
                // stack_combinaciones = _stack_combinaciones_inicial;
                // pop(&tmp);
  
                std::cout << "Fin Busqueda Rutas"<<std::endl;
                mostrarRutas(rutas_ok);
            }

          }
          std::cout<<"rank: "<<p_rank<<std::endl;
        } else {
          cout << "Debe ingresar correctamente las estaciones" << endl;
      }
    }else {
      if(string(argv[1])  == "-v"){
        cout << "\nIntegrantes:\n\tLucas Henry\n\tEsteban Martínez\n\tJorge Maureira\n\n" << endl;
      }
    }
  }
  
  MPI_Finalize();
  return 0;
}

void testingRecorrerLinea(linea inicio, linea destino) {

}