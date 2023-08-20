#ifndef	SOCKET_H
#define	SOCKET_H

struct addrinfo* getAddressInfo(void (*onError)());
int createAndBindSocket(const struct addrinfo *serverAddressInfo, void (*onError)());

#endif