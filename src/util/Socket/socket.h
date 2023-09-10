#include <netdb.h> //addrinfo
#include "../TCP/TCP.h" // newTCPConnection, TCPConnection
#include "../Http/http.h"

#ifndef	SOCKET_H
#define	SOCKET_H

struct addrinfo* getAddressInfo(void (*onError)());
int createAndBindSocket(uint16_t port, void (*onError)());
void handleConnectionOnANewProcess(HTTPConnection* newConnection);
void listenForConnections(
  uint16_t port,
  int socketFileDescriptor,
  void (*handdler)(TCPConnection* newConnection, void* context),
  void* context,
  void (*onError)()
);
int connectTo(char *host, int port);

#endif