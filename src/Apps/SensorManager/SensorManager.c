#include "../../socket.h"
#include <unistd.h>

int main() {
    int sockfd = connectTo("127.0.0.1", 3492);
 
    // function for chat
    sendRequest(sockfd);
 
    // close the socket
    close(sockfd);
}
