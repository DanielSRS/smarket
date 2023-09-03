#include "../../util/Socket/socket.h" // connectTo, sendRequest
#include "../../util/ReadTags/ReadTags.h" //readtags
#include <unistd.h> // close
#include <stdio.h> // printf

int main() {
    int sockfd = connectTo("127.0.0.1", 3492);
    Map *tagsList = readTags();

    printf("\n\nTags lidas: \n%s", tagsList->toString(tagsList));
 
    // function for chat
    sendRequest(sockfd);
 
    // close the socket
    close(sockfd);
}
