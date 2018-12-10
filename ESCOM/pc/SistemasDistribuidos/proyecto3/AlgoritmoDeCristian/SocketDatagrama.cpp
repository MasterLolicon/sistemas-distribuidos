#include "SocketDatagrama.h"

SocketDatagrama :: SocketDatagrama(int puerto){
	timeout = false;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	bzero((char *)&direccionLocal, sizeof(direccionLocal));
   	direccionLocal.sin_family = AF_INET;
   	direccionLocal.sin_addr.s_addr = INADDR_ANY;
   	direccionLocal.sin_port = htons(puerto);
   	bind(s, (struct sockaddr *)&direccionLocal, sizeof(direccionLocal));
}

SocketDatagrama :: ~SocketDatagrama(){
	bzero((char *)&direccionLocal, sizeof(direccionLocal));
	close(s);
}

void SocketDatagrama :: unsetTimeout(){
	timeout = false;
	tiempo.tv_sec = 0;
	tiempo.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tiempo, sizeof(tiempo));
}

void SocketDatagrama :: setTimeout(time_t segundos, suseconds_t microsegundos){
	tiempo.tv_sec = segundos;
	tiempo.tv_usec = microsegundos;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tiempo, sizeof(tiempo));
	timeout = true;
}

int SocketDatagrama :: recibeTimeout(PaqueteDatagrama &p){
	struct timeval t1, t2, tr;

	int retorno;
	socklen_t clilen;
	clilen = sizeof(direccionForanea);
	gettimeofday(&t1, NULL);
	retorno = recvfrom(s, (char *)p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &clilen);
	gettimeofday(&t2, NULL);
	if(retorno < 0){
		std :: cout << "Tiempo Excedido\n" << std :: endl;
		if(errno == EWOULDBLOCK)
			retorno = retorno;
		else
			std :: cout << "Error en recvfrom" << std :: endl;
	}

	unsetTimeout();
	if(retorno > 0){
		timersub(&t2, &t1, &tr);
		std :: cout << "Tiempo de recepcion" << std :: endl;
		std :: cout << "Segundos: " << tr.tv_sec << std :: endl;
		std :: cout << "Microsegundos: " << tr.tv_usec << std :: endl << std :: endl;

	}
	p.inicializaPuerto(ntohs(direccionForanea.sin_port));
	inet_ntop(AF_INET, &(direccionForanea.sin_addr), p.obtieneDireccion(), INET_ADDRSTRLEN);
	return retorno;
}

int SocketDatagrama :: recibe(PaqueteDatagrama &p){
	socklen_t clilen =  sizeof(direccionForanea);
	int retorno = recvfrom(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &clilen);
	p.inicializaPuerto(ntohs(direccionForanea.sin_port));
	inet_ntop(AF_INET, &(direccionForanea.sin_addr), p.obtieneDireccion(), INET_ADDRSTRLEN);
	
	/*std :: cout << "Direccion IP del paquete RECIBIDO:" << p.obtieneDireccion() << std :: endl;
	std :: cout << "Puerto del paquete RECIBIDO:" << p.obtienePuerto() << std :: endl << std :: endl;
	*/
	return retorno; 
}

int SocketDatagrama :: envia(PaqueteDatagrama &p){
	socklen_t clilen =  sizeof(direccionForanea);
	bzero((char *)&direccionForanea, sizeof(direccionForanea));
   	direccionForanea.sin_family = AF_INET;
   	direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
   	direccionForanea.sin_port = htons(p.obtienePuerto());
	int retorno = sendto(s,(char *) p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, sizeof(direccionForanea));
	
/*
	std :: cout << "Direccion IP del paquete ENVIADO:" << p.obtieneDireccion() << std :: endl;
	std :: cout << "Puerto del paquete ENVIADO:" << p.obtienePuerto() << std :: endl << std :: endl;
*/	
	return retorno; 
}

void SocketDatagrama :: setBroadcast(){
	int yes = 1;
	int broad = setsockopt( s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
	if( broad < 0 ){
		std :: cout << "Error en el Broadcast" << std :: endl;
		//exit(0);
	}

}

void SocketDatagrama :: unsetBroadcast(){
	int yes = 0;
	int broad = setsockopt( s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
	if( broad < 0 ){
		std :: cout << "Error en el Broadcast" << std :: endl;
		//exit(0);
	}
}

