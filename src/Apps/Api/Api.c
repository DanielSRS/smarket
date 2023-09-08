#include <stdlib.h> // exit
#include "../../util/Socket/socket.h" // freeaddrinfo, getAddressInfo, createAndBindSocket, listenForConnections, addrinfo

void onGetAddressInfoError() {
    exit(1);
};

int main(void) {

    int socketFileDescriptor = createAndBindSocket(onGetAddressInfoError);

    listenForConnections(socketFileDescriptor, onGetAddressInfoError);

    return 0;
}
