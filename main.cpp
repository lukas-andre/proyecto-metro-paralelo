#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#include "dataloader.h"
#include "metrored.h"

# include <mpi.h>

using namespace std;

std::string toString(recorrido ruta){
  std::string rutax;
  while(ruta){
    rutax.append(ruta->estacion->nombre);
    rutax.append(" - ");
    ruta = ruta->link;
  }
  return rutax;
}

int contadorEstaciones(std::string s){
    std::string delimiter = "-";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0, pos);
      s.erase(0, pos + delimiter.length());
      i++;
    }
    return i;
}

std::string separar_ruta_mas_corta(std::string s, std::string p_delimiter){
    std::string delimiter = p_delimiter;
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0, pos);
      s.erase(0, pos + delimiter.length());
      i++;
    }
    return "";
}
 
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
          if(p_size != 6) {
            std::cout << "El número de procesadores debe ser máximo 6" << std::endl;
            MPI::Finalize();
            return 0;
          }
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

            // cout << "Estación de inicio  : " << inicio->nombre << endl;
            // cout << "Estación destino    : " << destino->nombre << endl << endl;
            
            linea estacion_inicio;
            estacion_inicio = inicio;

            recorrido combinaciones = NULL; 
            recorrido p_ruta = NULL;
            recorridos rutas = NULL;
            // CREO UN STRING LARGO
            std::string _ruta_mas_corta(1000, 'l');
            std::string s_rutas;
            if (destinoEnLinea(estacion_inicio, destino)) {
                std::string direccion;
                direccion = buscarDireccion(estacion_inicio, destino);
                recorrerLinea(estacion_inicio, direccion, destino, &p_ruta);
                mostrarRecorrido(p_ruta);
                guardarRuta(&rutas, p_ruta);
            }
            else{
                obtenerCombinacionesLinea(&combinaciones, estacion_inicio);
                recorrido tmp = combinaciones;
                largo_combinaciones = obtenerLargoRecorrido(combinaciones);
                int send_data;
                MPI::Status stats; 
                if(p_size != 6) {
                  return 0;
                }
                for(int i = 0; i <= p_size; i++){
                  if(i == 1){
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    MPI_Send(&confirm, 1, MPI::BOOL, 1, 6, MPI_COMM_WORLD); 
                    if(confirm){
                      // ENVIO CODIGO INICIO
                      std::string codigo = tmp->estacion->combinacion->codigo;
                      MPI::COMM_WORLD.Send(codigo.c_str(), codigo.length(), MPI::CHAR, 1, 11); 
                      // ENVIO RUTA
                      recorrido ruta_pendiente = NULL;
                      recorrerLinea(
                          inicio,
                          buscarDireccion(inicio, tmp->estacion),
                          tmp->estacion,
                          &ruta_pendiente
                          );
                      std::string s_ruta = toString(ruta_pendiente);
                      MPI::COMM_WORLD.Send(s_ruta.c_str(), s_ruta.length(), MPI::CHAR, 1, 111); 
                      // ENVIO CODIGO DESTINO
                      std::string codigo_destino = destino->codigo;
                      MPI::COMM_WORLD.Send(codigo_destino.c_str(), codigo_destino.length(), MPI::CHAR, 1, 1111); 
                      // RECIBO RUTA MAS CORTA
                      MPI::COMM_WORLD.Probe(1, 99, stats);
                      int l = stats.Get_count(MPI::CHAR);
                      char *buf = new char[l];
                      MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 1, 99, stats);
                      std::string ruta_mas_corta(buf, l);
                      delete [] buf;
                      // ALMAZENAMOS RUTA         
                      s_rutas.append(ruta_mas_corta);             
                      s_rutas.append(" - ");  
                      std::cout<<"ruta mas corta encontrada P2: " << s_ruta << ruta_mas_corta <<std::endl;     
                      i++;
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
                      // ENVIO CODIGO INICIO                      
                      std::string nombre = tmp->estacion->combinacion->codigo;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 2, 12);    
                      // ENVIO RUTA
                      recorrido ruta_pendiente = NULL;
                      recorrerLinea(
                          inicio,
                          buscarDireccion(inicio, tmp->estacion),
                          tmp->estacion,
                          &ruta_pendiente
                          );
                      std::string s_ruta = toString(ruta_pendiente);
                      MPI::COMM_WORLD.Send(s_ruta.c_str(), s_ruta.length(), MPI::CHAR, 2, 122);    
                      // ENVIO CODIGO DESTINO
                      std::string codigo_destino = destino->codigo;
                      MPI::COMM_WORLD.Send(codigo_destino.c_str(), codigo_destino.length(), MPI::CHAR, 2, 1222);
                      // RECIBO RUTA MAS CORTA
                      MPI::COMM_WORLD.Probe(2, 88, stats);
                      int l = stats.Get_count(MPI::CHAR);
                      char *buf = new char[l];
                      MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 2, 88, stats);
                      std::string ruta_mas_corta(buf, l);
                      delete [] buf;
                      // ALMAZENAMOS RUTA      
                      s_rutas.append(ruta_mas_corta);             
                      s_rutas.append(" - "); 
                      std::cout<<"ruta mas corta encontrada P1: " << s_ruta << ruta_mas_corta <<std::endl;                           

                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);  
                    }
                    i++;
                    pop(&tmp);
                  }
                  if(i == 3){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    MPI_Send(&confirm, 1, MPI::BOOL, 3, 8, MPI_COMM_WORLD); 
                    if(confirm){
                      // ENVIO CODIGO INICIO 
                      std::string nombre = tmp->estacion->combinacion->codigo;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 3, 13);         
                      // ENVIO RUTA
                      recorrido ruta_pendiente = NULL;
                      recorrerLinea(
                          inicio,
                          buscarDireccion(inicio, tmp->estacion),
                          tmp->estacion,
                          &ruta_pendiente
                          );
                      std::string s_ruta = toString(ruta_pendiente);
                      MPI::COMM_WORLD.Send(s_ruta.c_str(), s_ruta.length(), MPI::CHAR, 3, 133);    
                      // ENVIO CODIGO DESTINO
                      std::string codigo_destino = destino->codigo;
                      MPI::COMM_WORLD.Send(codigo_destino.c_str(), codigo_destino.length(), MPI::CHAR, 3, 1333);
                      // RECIBO RUTA MAS CORTA
                      MPI::COMM_WORLD.Probe(3, 77, stats);
                      int l = stats.Get_count(MPI::CHAR);
                      char *buf = new char[l];
                      MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 3, 77, stats);
                      std::string ruta_mas_corta(buf, l);
                      delete [] buf;
                      // ALMAZENAMOS RUTA
                      s_rutas.append(ruta_mas_corta);   
                      s_rutas.append(" - ");   
                      std::cout<<"ruta mas corta encontrada P3: " << s_ruta << ruta_mas_corta <<std::endl;     
         

                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 3, 3, MPI_COMM_WORLD);  
                    }
                    if(tmp){
                      i++;                     
                      pop(&tmp);
                    }
                  }
                  if(i == 4){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    MPI_Send(&confirm, 1, MPI::BOOL, 4, 9, MPI_COMM_WORLD); 
                    if(confirm){
                      // ENVIO CODIGO INICIO 
                      std::string nombre = tmp->estacion->combinacion->codigo;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 4, 14);    
                      // ENVIO RUTA
                      recorrido ruta_pendiente = NULL;
                      recorrerLinea(
                          inicio,
                          buscarDireccion(inicio, tmp->estacion),
                          tmp->estacion,
                          &ruta_pendiente
                          );
                      std::string s_ruta = toString(ruta_pendiente);
                      MPI::COMM_WORLD.Send(s_ruta.c_str(), s_ruta.length(), MPI::CHAR, 4, 144);    
                      // ENVIO CODIGO DESTINO
                      std::string codigo_destino = destino->codigo;
                      MPI::COMM_WORLD.Send(codigo_destino.c_str(), codigo_destino.length(), MPI::CHAR, 4, 1444);
                      // RECIBO RUTA MAS CORTA
                      MPI::COMM_WORLD.Probe(4, 66, stats);
                      int l = stats.Get_count(MPI::CHAR);
                      char *buf = new char[l];
                      MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 4, 66, stats);
                      std::string ruta_mas_corta(buf, l);
                      delete [] buf;
                      // ALMAZENAMOS RUTA
                      s_rutas.append(ruta_mas_corta);             
                      s_rutas.append(" - ");             
                      std::cout<<"ruta mas corta encontrada P4: " << s_ruta << ruta_mas_corta <<std::endl;     
       
                      
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 4, 4, MPI_COMM_WORLD); 
                    }
                    if(tmp){
                      i++;
                      pop(&tmp);
                    }
                  }
                  if(i == 5){
                    largo_combinaciones--;
                    bool confirm = false;
                    if(largo_combinaciones > 0){
                      confirm = true;
                    }
                    MPI_Send(&confirm, 1, MPI::BOOL, 5, 10, MPI_COMM_WORLD); 
                    if(confirm){
                      // ENVIO CODIGO INICIO                       
                      std::string nombre = tmp->estacion->combinacion->codigo;
                      MPI::COMM_WORLD.Send(nombre.c_str(), nombre.length(), MPI::CHAR, 5, 15);  
                      // ENVIO RUTA
                      recorrido ruta_pendiente = NULL;
                      recorrerLinea(
                          inicio,
                          buscarDireccion(inicio, tmp->estacion),
                          tmp->estacion,
                          &ruta_pendiente
                          );
                      std::string s_ruta = toString(ruta_pendiente);
                      MPI::COMM_WORLD.Send(s_ruta.c_str(), s_ruta.length(), MPI::CHAR, 5, 155);                        
                      // ENVIO CODIGO DESTINO
                      std::string codigo_destino = destino->codigo;
                      MPI::COMM_WORLD.Send(codigo_destino.c_str(), codigo_destino.length(), MPI::CHAR, 5, 1555);
                      // RECIBO RUTA MAS CORTA
                      MPI::COMM_WORLD.Probe(5, 55, stats);
                      int l = stats.Get_count(MPI::CHAR);
                      char *buf = new char[l];
                      MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 5, 55, stats);
                      std::string ruta_mas_corta(buf, l);
                      delete [] buf;
                      // ALMAZENAMOS RUTA
                      s_rutas.append(ruta_mas_corta);             
                      std::cout<<"ruta mas corta encontrada P5: " << s_ruta << ruta_mas_corta <<std::endl;     
          
                      
                    }else{
                      send_data = -1;
                      MPI_Send(&send_data, 1, MPI_INT, 5, 5, MPI_COMM_WORLD);
                    }   
                  }
                }  
                // std::cout<< s_rutas <<std::endl;
                // separar_ruta_mas_corta(s_rutas, inicio->nombre);
            }
          } 
          if(p_rank == 1){
            int recv_data1;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 6, MPI_COMM_WORLD, &status);      
            if(confirm){
              // RECIBO CODIGO INICIO
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 11 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 11, stats);
              std::string codigo_recv(buf, l);
              delete [] buf;

              // RECIBO RUTA
              MPI::COMM_WORLD.Probe(0, 111, stats);
              int ll = stats.Get_count(MPI::CHAR);
              char *buff = new char[ll];
              MPI::COMM_WORLD.Recv(buff, ll, MPI::CHAR, 0, 111, stats);
              std::string ruta_recv(buff, ll);
              delete [] buff;
              
              // RECIBO CODIGO DESTINO
              MPI::COMM_WORLD.Probe(0, 1111, stats);
              int lll = stats.Get_count(MPI::CHAR);
              char *bufff = new char[lll];
              MPI::COMM_WORLD.Recv(bufff, lll, MPI::CHAR, 0, 1111, stats);
              std::string destino_recv(bufff, lll);
              delete [] bufff;

              // CODIGO QUE SE REPITE MEJORAR ESTO :S
              linea _l1, _l2, _l4, _l4a, _l5, _l6;
              _l1 = (linea) malloc(sizeof (nodo));
              _l2 = (linea) malloc(sizeof (nodo));
              _l4 = (linea) malloc(sizeof (nodo));
              _l4a = (linea) malloc(sizeof (nodo));
              _l5 = (linea) malloc(sizeof (nodo));
              _l6 = (linea) malloc(sizeof (nodo));

              generarLineasMetro(&_l1, &_l2, &_l4, &_l4a, &_l5, &_l6);

              linea inicio, destino;
              inicio = (linea) malloc(sizeof (nodo));
              destino = (linea) malloc(sizeof (nodo));

              // Tranformo string a char* codigo
              std::string str_codigo = codigo_recv ;
              char *c_codigo = new char[str_codigo.length() + 1];
              strcpy(c_codigo, str_codigo.c_str());
      
              // Tranformo string a char* destino
              std::string str_destino = destino_recv;
              char *c_destino = new char[destino_recv.length() + 1];
              strcpy(c_destino, str_destino.c_str());

              inicio = getUbicacion(c_codigo, _l1, _l2, _l4, _l4a, _l5, _l6);
              destino = getUbicacion(c_destino, _l1, _l2, _l4, _l4a, _l5, _l6);
              delete [] c_codigo;
              delete [] c_destino;

              std::string ruta_mas_corta = buscarRutaMasCorta(inicio, destino);

              MPI::COMM_WORLD.Send(ruta_mas_corta.c_str(), ruta_mas_corta.length(), MPI::CHAR, 0, 99);

            }else{
              MPI_Recv(&recv_data1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            }
          }
          if(p_rank == 2){
            int recv_data2;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 7, MPI_COMM_WORLD, &status);      
            if(confirm){
              // RECIBO CODIGO INICIO
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 12 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 12, stats);
              std::string codigo_recv(buf, l);
              delete [] buf;

              // RECIBO RUTA
              MPI::COMM_WORLD.Probe(0, 122, stats);
              int ll = stats.Get_count(MPI::CHAR);
              char *buff = new char[ll];
              MPI::COMM_WORLD.Recv(buff, ll, MPI::CHAR, 0, 122, stats);
              std:: string ruta_recv(buff, ll);
              delete [] buff;

              // RECIBO CODIGO DESTINO
              MPI::COMM_WORLD.Probe(0, 1222, stats);
              int lll = stats.Get_count(MPI::CHAR);
              char *bufff = new char[lll];
              MPI::COMM_WORLD.Recv(bufff, lll, MPI::CHAR, 0, 1222, stats);
              std::string destino_recv(bufff, lll);
              delete [] bufff;

              // CODIGO QUE SE REPITE MEJORAR ESTO :S
              linea _l1, _l2, _l4, _l4a, _l5, _l6;
              _l1 = (linea) malloc(sizeof (nodo));
              _l2 = (linea) malloc(sizeof (nodo));
              _l4 = (linea) malloc(sizeof (nodo));
              _l4a = (linea) malloc(sizeof (nodo));
              _l5 = (linea) malloc(sizeof (nodo));
              _l6 = (linea) malloc(sizeof (nodo));

              generarLineasMetro(&_l1, &_l2, &_l4, &_l4a, &_l5, &_l6);

              linea inicio, destino;
              inicio = (linea) malloc(sizeof (nodo));
              destino = (linea) malloc(sizeof (nodo));

              // Tranformo string a char* codigo
              std::string str_codigo = codigo_recv ;
              char *c_codigo = new char[str_codigo.length() + 1];
              strcpy(c_codigo, str_codigo.c_str());
      
              // Tranformo string a char* destino
              std::string str_destino = destino_recv;
              char *c_destino = new char[destino_recv.length() + 1];
              strcpy(c_destino, str_destino.c_str());

              inicio = getUbicacion(c_codigo, _l1, _l2, _l4, _l4a, _l5, _l6);
              destino = getUbicacion(c_destino, _l1, _l2, _l4, _l4a, _l5, _l6);
              delete [] c_codigo;
              delete [] c_destino;

              std::string ruta_mas_corta = buscarRutaMasCorta(inicio, destino);
              
              MPI::COMM_WORLD.Send(ruta_mas_corta.c_str(), ruta_mas_corta.length(), MPI::CHAR, 0, 88);
              
            }else{
              MPI_Recv(&recv_data2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
            }
          } 
          if(p_rank == 3){
            int recv_data3;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 8, MPI_COMM_WORLD, &status);
            if(confirm){  
              // RECIBO CODIGO INICIO
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 13 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 13, stats);
              std::string codigo_recv(buf, l);
              delete [] buf;
          
              // RECIBO RUTA
              MPI::COMM_WORLD.Probe(0, 133, stats);
              int ll = stats.Get_count(MPI::CHAR);
              char *buff = new char[ll];
              MPI::COMM_WORLD.Recv(buff, ll, MPI::CHAR, 0, 133, stats);
              std::string ruta_recv(buff, ll);
              delete [] buff;  
              
              // RECIBO CODIGO DESTINO
              MPI::COMM_WORLD.Probe(0, 1333, stats);
              int lll = stats.Get_count(MPI::CHAR);
              char *bufff = new char[lll];
              MPI::COMM_WORLD.Recv(bufff, lll, MPI::CHAR, 0, 1333, stats);
              std::string destino_recv(bufff, lll);
              delete [] bufff;

              // CODIGO QUE SE REPITE MEJORAR ESTO :S
              linea _l1, _l2, _l4, _l4a, _l5, _l6;
              _l1 = (linea) malloc(sizeof (nodo));
              _l2 = (linea) malloc(sizeof (nodo));
              _l4 = (linea) malloc(sizeof (nodo));
              _l4a = (linea) malloc(sizeof (nodo));
              _l5 = (linea) malloc(sizeof (nodo));
              _l6 = (linea) malloc(sizeof (nodo));

              generarLineasMetro(&_l1, &_l2, &_l4, &_l4a, &_l5, &_l6);

              linea inicio, destino;
              inicio = (linea) malloc(sizeof (nodo));
              destino = (linea) malloc(sizeof (nodo));

              // Tranformo string a char* codigo
              std::string str_codigo = codigo_recv ;
              char *c_codigo = new char[str_codigo.length() + 1];
              strcpy(c_codigo, str_codigo.c_str());

              // Tranformo string a char* destino
              std::string str_destino = destino_recv;
              char *c_destino = new char[destino_recv.length() + 1];
              strcpy(c_destino, str_destino.c_str());

              inicio = getUbicacion(c_codigo, _l1, _l2, _l4, _l4a, _l5, _l6);
              destino = getUbicacion(c_destino, _l1, _l2, _l4, _l4a, _l5, _l6);
              delete [] c_codigo;
              delete [] c_destino;

              std::string ruta_mas_corta = buscarRutaMasCorta(inicio, destino);

              MPI::COMM_WORLD.Send(ruta_mas_corta.c_str(), ruta_mas_corta.length(), MPI::CHAR, 0, 77);

            }else{
              MPI_Recv(&recv_data3, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
            }
          }  
          if(p_rank == 4){
            int recv_data4;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 9, MPI_COMM_WORLD, &status);
            if(confirm){
              // RECIBO CODIGO INICIO
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 14 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 14, stats);
              std::string codigo_recv(buf, l);
              delete [] buf;

              // RECIBO RUTA
              MPI::COMM_WORLD.Probe(0, 144, stats);
              int ll = stats.Get_count(MPI::CHAR);
              char *buff = new char[ll];
              MPI::COMM_WORLD.Recv(buff, ll, MPI::CHAR, 0, 144, stats);
              std::string ruta_recv(buff, ll);
              delete [] buff;
            
              // RECIBO CODIGO DESTINO
              MPI::COMM_WORLD.Probe(0, 1444, stats);
              int lll = stats.Get_count(MPI::CHAR);
              char *bufff = new char[lll];
              MPI::COMM_WORLD.Recv(bufff, lll, MPI::CHAR, 0, 1444, stats);
              std::string destino_recv(bufff, lll);
              delete [] bufff;

              // CODIGO QUE SE REPITE MEJORAR ESTO :S
              linea _l1, _l2, _l4, _l4a, _l5, _l6;
              _l1 = (linea) malloc(sizeof (nodo));
              _l2 = (linea) malloc(sizeof (nodo));
              _l4 = (linea) malloc(sizeof (nodo));
              _l4a = (linea) malloc(sizeof (nodo));
              _l5 = (linea) malloc(sizeof (nodo));
              _l6 = (linea) malloc(sizeof (nodo));

              generarLineasMetro(&_l1, &_l2, &_l4, &_l4a, &_l5, &_l6);

              linea inicio, destino;
              inicio = (linea) malloc(sizeof (nodo));
              destino = (linea) malloc(sizeof (nodo));

              // Tranformo string a char* codigo
              std::string str_codigo = codigo_recv ;
              char *c_codigo = new char[str_codigo.length() + 1];
              strcpy(c_codigo, str_codigo.c_str());
      
              // Tranformo string a char* destino
              std::string str_destino = destino_recv;
              char *c_destino = new char[destino_recv.length() + 1];
              strcpy(c_destino, str_destino.c_str());

              inicio = getUbicacion(c_codigo, _l1, _l2, _l4, _l4a, _l5, _l6);
              destino = getUbicacion(c_destino, _l1, _l2, _l4, _l4a, _l5, _l6);
              delete [] c_codigo;
              delete [] c_destino;

              std::string ruta_mas_corta = buscarRutaMasCorta(inicio, destino);

              MPI::COMM_WORLD.Send(ruta_mas_corta.c_str(), ruta_mas_corta.length(), MPI::CHAR, 0, 66);
            }else{
              MPI_Recv(&recv_data4, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
            }
          }
          if(p_rank == 5){
            int recv_data5;
            bool confirm;
            MPI_Recv(&confirm, 1, MPI::BOOL, 0, 10, MPI_COMM_WORLD, &status);
            if(confirm){           
              // RECIBO CODIGO INICIO
              MPI::Status stats;
              MPI::COMM_WORLD.Probe(0, 15 , stats);
              int l = stats.Get_count(MPI::CHAR);
              char *buf = new char[l];
              MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, 0, 15, stats);
              std::string codigo_recv(buf, l);
              delete [] buf;
              // RECIBO RUTA
              MPI::COMM_WORLD.Probe(0, 155, stats);
              int ll = stats.Get_count(MPI::CHAR);
              char *buff = new char[ll];
              MPI::COMM_WORLD.Recv(buff, ll, MPI::CHAR, 0, 155, stats);
              std::string ruta_recv(buff, ll);
              delete [] buff;  
              
              // RECIBO CODIGO DESTINO
              MPI::COMM_WORLD.Probe(0, 1555, stats);
              int lll = stats.Get_count(MPI::CHAR);
              char *bufff = new char[lll];
              MPI::COMM_WORLD.Recv(bufff, lll, MPI::CHAR, 0, 1555, stats);
              std::string destino_recv(bufff, lll);
              delete [] bufff;
              
              // CODIGO QUE SE REPITE MEJORAR ESTO :S
              linea _l1, _l2, _l4, _l4a, _l5, _l6;
              _l1 = (linea) malloc(sizeof (nodo));
              _l2 = (linea) malloc(sizeof (nodo));
              _l4 = (linea) malloc(sizeof (nodo));
              _l4a = (linea) malloc(sizeof (nodo));
              _l5 = (linea) malloc(sizeof (nodo));
              _l6 = (linea) malloc(sizeof (nodo));

              generarLineasMetro(&_l1, &_l2, &_l4, &_l4a, &_l5, &_l6);

              linea inicio, destino;
              inicio = (linea) malloc(sizeof (nodo));
              destino = (linea) malloc(sizeof (nodo));

              // Tranformo string a char* codigo
              std::string str_codigo = codigo_recv ;
              char *c_codigo = new char[str_codigo.length() + 1];
              strcpy(c_codigo, str_codigo.c_str());
      
              // Tranformo string a char* destino
              std::string str_destino = destino_recv;
              char *c_destino = new char[destino_recv.length() + 1];
              strcpy(c_destino, str_destino.c_str());

              inicio = getUbicacion(c_codigo, _l1, _l2, _l4, _l4a, _l5, _l6);
              destino = getUbicacion(c_destino, _l1, _l2, _l4, _l4a, _l5, _l6);
              delete [] c_codigo;
              delete [] c_destino;

              std::string ruta_mas_corta = buscarRutaMasCorta(inicio, destino);

              MPI::COMM_WORLD.Send(ruta_mas_corta.c_str(), ruta_mas_corta.length(), MPI::CHAR, 0, 55);
              
            }else{
              MPI_Recv(&recv_data5, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
            }
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

