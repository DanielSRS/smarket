#include "../../util/Socket/socket.h" // connectTo, sendRequest
#include "../../util/ReadTags/ReadTags.h" //readtags
#include <unistd.h> // close
#include <stdio.h> // printf
#include <string.h> // strlen

void sendRequest(TCPClient* tcpClient);

int main() {
    TCPClient* tcpClient = newTCPClient("127.0.0.1", 3490);
    Map *tagsList = readTags();

    printf("\n\nTags lidas: \n%s", tagsList->toString(tagsList));
 
    // function for chat
    sendRequest(tcpClient);
    tcpClient->close(tcpClient);
}

void sendRequest(TCPClient* tcpClient) {
    char *request = "GET /dan/niel?dfg=dsfg HTTP/1.1\
                    \r\nHost: localhost:3492\
                    \r\nContent-Type: application/json\
                    \r\nUser-Agent: SensorManager/0.0.1\
                    \r\nContent-Length: 17\
                    \r\n\r\n{\"with\": \"tcpclient\"}";

    int numberOfBytesSent = tcpClient->send(tcpClient, strlen(request), request);
    if (numberOfBytesSent == -1)
        perror("send");

    printf("\nRequest sent\n\n");
}
