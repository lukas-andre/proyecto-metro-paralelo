#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

# include <mpi.h>

#include "dataloader.h"
#include "metrored.h"

using namespace std;

// VARIABLES PARA USAR LOS ARGUMENTOS FUERA DEL MAIN
int cantidad_argumentos;
char** argumentos;

// VARIABLES PARA MPI

int id;
int ierr;
int rango;
int procesos;
int rango_origen;
int rango_destino;
int etiqueta;
MPI_Status estado;

int main(int argc, char** argv) {
  int rank,size;
  // Se inicializa MPI
  MPI_Init(0,0);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if( argc < 2 ){
      cout << "Debe ingresar al menos 1 argumento" << endl;
  }else {
    if(string(argv[1])  == "-f"){
        if(argc == 4){
          //Rank 0 se encarga de encontrar las posibles rutas hasta el destino e imprime la ruta con menor largo luego de recibir la respuesta de Rank 1
          if (rank==0){
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
          }
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