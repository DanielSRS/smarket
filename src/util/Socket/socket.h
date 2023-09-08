#include <netdb.h> //addrinfo
#include "../TCP/TCP.h" // newTCPConnection, TCPConnection

#ifndef	SOCKET_H
#define	SOCKET_H

struct addrinfo* getAddressInfo(void (*onError)());
int createAndBindSocket(uint16_t port, void (*onError)());
void handleConnectionOnANewProcess(TCPConnection* newConnection);
void listenForConnections(uint16_t port, int socketFileDescriptor, void (*handdler)(TCPConnection* newConnection), void (*onError)());
void sendRequest(int sockfd);
int connectTo(char *host, int port);

#endif