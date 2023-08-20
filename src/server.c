#include <stdlib.h> // exit
#include "socket.h" // freeaddrinfo, getAddressInfo, createAndBindSocket, listenForConnections, addrinfo

void onGetAddressInfoError() {
    exit(1);
};

int main(void) {
    struct addrinfo *servinfo;

    servinfo = getAddressInfo(onGetAddressInfoError);

    int socketFileDescriptor = createAndBindSocket(servinfo, onGetAddressInfoError);
  
    freeaddrinfo(servinfo); // Libera a memória. Essa estrutura não vai ser mais usada. 

    listenForConnections(socketFileDescriptor, onGetAddressInfoError);

    return 0;
}
