#ifndef SOCKETDATAGRAMA_H_
#define SOCKETDATAGRAMA_H_

#include "PaqueteDatagrama.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <netdb.h>
#include <strings.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <signal.h>
#define ld long double

class SocketDatagrama{
public:
   SocketDatagrama(int); //puerto local no se podrá cambiar para el proceso. Si es cero, lo asigna el sistema operativo
   ~SocketDatagrama();
   //Recibe un paquete tipo datagrama proveniente de este socket
   int recibe(PaqueteDatagrama & p);
   //Envía un paquete tipo datagrama desde este socket
   int envia(PaqueteDatagrama & p);
   void setTimeout();
   void unsetTimeout( );
   struct timeval getTimeout();
   int recibeTimeout( PaqueteDatagrama &p );    
   void setRTO();
   int recibeTimeoutRTO( PaqueteDatagrama &p );
   int enviaTimeoutRTO( PaqueteDatagrama &p );
   struct timeval getTiempoRecibe();
   int setBroadcast();
private:   
   struct sockaddr_in direccionLocal;
   struct sockaddr_in direccionForanea;
   struct timeval tiempoFuera;
   int s; //ID socket
   bool timeout;
   long double srtt;
   long double rttvar;
   long double RTO;
   long double RTT;
   long double RTTmedida;
   long double delta;
   void setAlarma( time_t segundos, suseconds_t microsegundos );
   //void activaAlarma( int sig );
   struct timeval tiempo1;
   struct timeval tiempo2;
   struct timeval tiempo3;
   struct sigaction alarma;
   struct timeval tiempoRecibido;
   void calculaRTO();
};

#endif
