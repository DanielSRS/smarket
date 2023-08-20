#include <stdlib.h>
#include "socket.h"

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
