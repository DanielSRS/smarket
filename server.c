#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "socket.h"

#define PORT "3492"  // A porta usada para outros usuários se conectarem. 

#define BACKLOG 10   // Quantidade máxima de conexões pendentes 

/**
 * Lida da o sinal que o sistema envia quando os processos filhos param ou terminal
*/
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    /** Aguarda até que o processo filho morra */
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// Abstrai o tipo do endereço do socket. De modo que funcione com IPs tanto de versão 4 como 6
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void onGetAddressInfoError() {
    exit(1);
};

/**
 * Lida com os sinais recebidos pelo sistema operacional quando um processo filho encerra
*/
void handleChildProcessTermination() {
    struct sigaction signalAction;

    signalAction.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&signalAction.sa_mask);
    signalAction.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &signalAction, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void handleConnectionOnANewProcess(int parentSocketFileDescriptor, int connectedSocketFileDescriptor) {
    if (!fork()) { // Este é o processo filho 
        close(parentSocketFileDescriptor); // O processo filho não precisa dessa conexão

        // Resposta no formato definido pelo protocolo http 
        char *response = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\n\
        Connection: Keep-Alive\r\n\
        Content-Type: application/json\r\n\
        Content-Length: 14\r\n\r\n\
        {\"dan\": \"iel\"}";

        // Envia a resposta 
        if (send(connectedSocketFileDescriptor, response, strlen(response), 0) == -1)
            perror("send");
        close(connectedSocketFileDescriptor);
        exit(0);                                // Termina execução do processo filho
    }
}

void listenForConnections(int socketFileDescriptor, void (*onError)()) {
    struct sockaddr_storage originConnectionAddress; // Informações do endereço da conexão de origem
    socklen_t sin_size = sizeof originConnectionAddress;
    int connectedSocketFileDescriptor;
    char originIpAddress[INET6_ADDRSTRLEN];

    handleChildProcessTermination();

    if (listen(socketFileDescriptor, BACKLOG) == -1) {
        perror("listen");
        onError();
    }

    printf("\n\n✅ server: waiting for connections on port: %s ...\n", PORT);

    // Loop principal para lidar com as solicitações de conexão 
    while(1) {
        // Aceita uma conexão pendente 
        connectedSocketFileDescriptor = accept(
            socketFileDescriptor,
            (struct sockaddr *)
            &originConnectionAddress,
            &sin_size
        );

        if (connectedSocketFileDescriptor == -1) { // Se falha na conexão tenta a próxima 
            perror("accept");
            continue;
        }

        // Imprime o IP de origem da conexão 
        inet_ntop(
            originConnectionAddress.ss_family,
            get_in_addr((struct sockaddr *)&originConnectionAddress),
            originIpAddress,
            sizeof originIpAddress);

        printf("server: got connection from %s\n", originIpAddress);

        // printando dados enviados pelo cliente 
        int numbytes;
        char buf[1000];
        if ((numbytes = recv(connectedSocketFileDescriptor, buf, 1000 - 1, 0)) == -1) {
            perror("recv");
            onError();
        }

        buf[numbytes] = '\0';

        printf("server: received data:\n\n '%s'",buf);

        // Cria um novo processo para responder a solicitação 
        handleConnectionOnANewProcess(socketFileDescriptor, connectedSocketFileDescriptor);

        close(connectedSocketFileDescriptor);  // parent doesn't need this
    }
}

int main(void) {
    struct addrinfo *servinfo;

    servinfo = getAddressInfo(onGetAddressInfoError);

    int socketFileDescriptor = createAndBindSocket(servinfo, onGetAddressInfoError);
  
    freeaddrinfo(servinfo); // Libera a memória. Essa estrutura não vai ser mais usada. 

    listenForConnections(socketFileDescriptor, onGetAddressInfoError);

    return 0;
}
