#include <netdb.h> //addrinfo

#ifndef	SOCKET_H
#define	SOCKET_H

struct addrinfo* getAddressInfo(void (*onError)());
int createAndBindSocket(void (*onError)());
void handleConnectionOnANewProcess(int parentSocketFileDescriptor, int connectedSocketFileDescriptor);
void listenForConnections(int socketFileDescriptor, void (*onError)());
void sendRequest(int sockfd);
int connectTo(char *host, int port);

#endif