#include "SocketDatagrama.h"

int puerto = 7200;

int main(void){
   SocketDatagrama sock(7200);
   while(1) {
      PaqueteDatagrama aux(100000);
      sock.recibe(aux);
      int *datos =(int *) aux.obtieneDatos();
      std :: cout << datos[0] << " " << datos[1] << std :: endl; 
   }
}
