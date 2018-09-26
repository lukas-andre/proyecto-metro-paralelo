#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#include "dataloader.h"
#include "metrored.h"

# include <mpi.h>


using namespace std;

 
int main(int argc, char** argv) {
  MPI_Status status;

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
          
          if (p_rank==0){
            int largo_combinaciones;
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
                largo_combinaciones = obtenerLargoRecorrido(combinaciones);
                std::cout<<"mostrando combinaciones de: "<< inicio->nombre <<std::endl;
                mostrarRecorrido(combinaciones);
                std::cout<<"Total Combinaciones: "<< largo_combinaciones <<std::endl;
        
                std::cout<<"P: "<< p_rank <<std::endl;
                int send_data;
                for(unsigned i = 0; i <= p_size; i++){
                  if(i == 1){
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    MPI_Send(&confirm, 1, MPI::BOOL, 1, 6, MPI_COMM_WORLD); 
                    if(confirm){
                      std::string nombre = tmp->estacion->combinacion->nombre;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 1, 11);                    
                      std::cout<<"Enviando data a P1 "<< nombre << " desde P"<< p_rank <<std::endl;
                      // MPI_Send(&send_data, 1, MPI_INT, 1, 1, MPI_COMM_WORLD); 
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 1, 1, MPI_COMM_WORLD); 
                    }
                    pop(&tmp);
                  }
                  if(i == 2){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    MPI_Send(&confirm, 1, MPI::BOOL, 2, 7, MPI_COMM_WORLD); 
                    if(confirm){
                      std::string nombre = tmp->estacion->combinacion->nombre;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 2, 12);                    

                      std::cout<<"Enviando data "<< nombre <<" a P2 desde P"<< p_rank <<std::endl;            
                      // MPI_Send(&send_data, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);  
                    }
                    pop(&tmp);
                  }
                  if(i == 3){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    // std::cout<<"Confirm3: "<< confirm <<std::endl;
                    MPI_Send(&confirm, 1, MPI::BOOL, 3, 8, MPI_COMM_WORLD); 
                    if(confirm){
                      std::string nombre = tmp->estacion->combinacion->nombre;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 3, 13);                    

                      std::cout<<"Enviando data a P3 " <<nombre<<" desde P"<< p_rank <<std::endl;
                      // MPI_Send(&send_data, 1, MPI_INT, 3, 3, MPI_COMM_WORLD);  
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 3, 3, MPI_COMM_WORLD);  
                    }
                    if(tmp){
                      pop(&tmp);
                    }
                  }
                  if(i == 4){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    // std::cout<<"Confirm4: "<< confirm <<std::endl;
                    MPI_Send(&confirm, 1, MPI::BOOL, 4, 9, MPI_COMM_WORLD); 
                    if(confirm){
                      std::string nombre = tmp->estacion->combinacion->nombre;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 4, 14);                    

                      std::cout<<"Enviando data a P4 " << nombre <<" desde P"<< p_rank <<std::endl;
                      // MPI_Send(&send_data, 1, MPI_INT, 4, 4, MPI_COMM_WORLD); 
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 4, 4, MPI_COMM_WORLD); 
                    }
                    if(tmp){
                      pop(&tmp);
                    }
                  }
                  if(i == 5){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    // std::cout<<"Confirm5: "<< confirm <<std::endl;
                    MPI_Send(&confirm, 1, MPI::BOOL, 5, 10, MPI_COMM_WORLD); 
                    if(confirm){
                      std::string nombre = tmp->estacion->combinacion->nombre;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 5, 15);                    

                      std::cout<<"Enviando data a P5 "<< nombre << " desde P"<< p_rank <<std::endl;
                      // MPI_Send(&send_data, 1, MPI_INT, 5, 5, MPI_COMM_WORLD);
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 5, 5, MPI_COMM_WORLD);
                    }                 
                  }
                }
              // recorrido _stack_combinaciones_inicial = stack_combinaciones;
                // buscarDestinoDesde(tmp, destino, &stack_combinaciones, &combinaciones_conocidas, &rutas_ok, &ruta_pendiente);
                // ruta_pendiente = NULL;
                // combinaciones_conocidas = NULL;
                // stack_combinaciones = _stack_combinaciones_inicial;
                // pop(&tmp);
  
                std::cout << "Fin Busqueda Rutas"<<std::endl;
                // mostrarRutas(rutas_ok);
            }
          } 
          // linea _estacion_inicio;
          // recorrido _combinaciones;
          // _estacion_inicio = getUbicacion(argv[2], l1, l2, l4, l4a, l5, l6);          
          // obtenerCombinacionesLinea(&_combinaciones, _estacion_inicio);
          if(p_rank == 1){
            int recv_data1;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 6, MPI_COMM_WORLD, &status);
            // std::cout<<"confirm1: "<<confirm<<std::endl;         
            if(confirm){
              // MPI_Recv(&recv_data1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
              // std::cout<<"recv_data1: "<<recv_data1<<std::endl;
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 11 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 11, stats);
              std::string nombre_recv(buf, l);
              delete [] buf;
              std::cout<<"nombre_recv: "<<nombre_recv<<std::endl;
            }else{
              MPI_Recv(&recv_data1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            }
          }
          if(p_rank == 2){
            int recv_data2;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 7, MPI_COMM_WORLD, &status);
            // std::cout<<"confirm2: "<<confirm<<std::endl;         
            if(confirm){
              // MPI_Recv(&recv_data2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
              // std::cout<<"recv_data2: "<<recv_data2<<std::endl;  
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 12 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 12, stats);
              std::string nombre_recv(buf, l);
              delete [] buf;
              std::cout<<"nombre_recv2: "<<nombre_recv<<std::endl;       
            }else{
              MPI_Recv(&recv_data2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
            }
          } 
          if(p_rank == 3){
            int recv_data3;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 8, MPI_COMM_WORLD, &status);
            if(confirm){
              // MPI_Recv(&recv_data3, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
              // std::cout<<"recv_data3: "<<recv_data3<<std::endl;  
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 13 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 13, stats);
              std::string nombre_recv(buf, l);
              delete [] buf;
              std::cout<<"nombre_recv3: "<<nombre_recv<<std::endl;                      
            }else{
              MPI_Recv(&recv_data3, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
            }
          }  
          if(p_rank == 4){
            int recv_data4;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 9, MPI_COMM_WORLD, &status);
            if(confirm){
              // MPI_Recv(&recv_data4, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
              // std::cout<<"recv_data4: "<<recv_data4<<std::endl;           

              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 14 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 14, stats);
              std::string nombre_recv(buf, l);
              delete [] buf;
              std::cout<<"nombre_recv4: "<<nombre_recv<<std::endl;             

            }else{
              MPI_Recv(&recv_data4, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
            }
          }
          if(p_rank == 5){
            int recv_data5;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 10, MPI_COMM_WORLD, &status);
            if(confirm){
              // MPI_Recv(&recv_data5, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
              // std::cout<<"recv_data5: "<<recv_data5<<std::endl;           
              
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 15 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 15, stats);
              std::string nombre_recv(buf, l);
              delete [] buf;
              std::cout<<"nombre_recv5: "<<nombre_recv<<std::endl;        
            }else{
              MPI_Recv(&recv_data5, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
            }
          }
          // std::cout<<"rank: "<<p_rank<<std::endl;
          // MPI_Finalize();

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