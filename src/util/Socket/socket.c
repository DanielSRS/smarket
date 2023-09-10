#include <stdio.h> // null e stderr
#include <string.h> // memset
#include <strings.h> // bzero
#include <unistd.h> // cloes
#include <stdlib.h> // exit
#include <arpa/inet.h> // inet_ntop
#include "../ChildProcess/childProcess.h" // handleChildProcessTermination
#include "socket.h" //addrinfo
#include "../Http/http.h" // RequestHeaderInfo, getHeadersInfo

#define PORT "3492"  // A porta usada para outros usuários se conectarem.
#define SERVER_PORT 3492  // A porta usada para outros usuários se conectarem.
#define BACKLOG 10   // Quantidade máxima de conexões pendentes  

/**
 * Retorna uma referencia de memoria. Libere após usar!!!
*/
struct addrinfo* getAddressInfo(void (*onError)()) {
    struct addrinfo configurationHints;                         // Parametros de confuguração para a função getaddrinfo
    struct addrinfo *addressInfo;                               // Resposta de chamada de getaddrinfo
    int configurationMemorySize = sizeof configurationHints;    // Tamanha da estrutura que gurda a configuração
    int THERE_WAS_NO_ERROR = 0;                                 // getaddrinfo executou sem nenhum erro.

    memset(&configurationHints, 0, configurationMemorySize);    // zerado a memória 

    /* Definindo os  parametros de configuração */
    configurationHints.ai_family = AF_UNSPEC;       // Escolha automática entre ipv4 e ipv6
    configurationHints.ai_socktype = SOCK_STREAM;   // Usa TCP como tipo da conexão
    configurationHints.ai_flags = AI_PASSIVE;       // Usar meu ip

    // Automaticamente preenche as informações da struct de endereço 
    int errorCode = getaddrinfo(NULL, PORT, &configurationHints, &addressInfo);

    /* Verifica se houve erro*/
    if (errorCode != THERE_WAS_NO_ERROR) {
        // const char *getaddrinfoErrorMessage = gai_strerror(errorCode);
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errorCode));
        onError();
        return NULL;
    }

    return addressInfo;
}

/**
 * Cria um socket e faz o bind com a porta
*/
int createAndBindSocket(uint16_t port, void (*onError)()) {
  struct sockaddr_in serverAddress;
  int INVALID_FILE_DESCRIPTOR = -1;
  int socketFileDescriptor;
   
  // socket create and verification
  socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFileDescriptor == INVALID_FILE_DESCRIPTOR) {
    printf("socket creation failed...\n");
    perror("server: socket");
    onError();
  }

  else
    printf("Socket successfully created..\n");
  bzero(&serverAddress, sizeof(serverAddress));
  
  // assign IP, PORT
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);
  
  /**
   * Binding newly created socket to given IP and verification
  */
  if ((bind(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) {
    fprintf(stderr, "server: failed to bind\n");
    onError();
  }

  else
    printf("Socket successfully binded..\n");

  return socketFileDescriptor;
}

void handleConnectionOnANewProcess(HTTPConnection* newConnection) {
  if (!fork()) { // Este é o processo filho

    newConnection->sendResponse(newConnection);
    newConnection->close(newConnection);
    exit(0);                                // Termina execução do processo filho
  }
  newConnection->close(newConnection);
}

// Abstrai o tipo do endereço do socket. De modo que funcione com IPs tanto de versão 4 como 6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void listenForConnections(uint16_t port, int socketFileDescriptor, void (*handdler)(TCPConnection* newConnection, void* context), void* context, void (*onError)()) {
    struct sockaddr_storage originConnectionAddress; // Informações do endereço da conexão de origem
    socklen_t sin_size = sizeof originConnectionAddress;
    int connectedSocketFileDescriptor;
    char originIpAddress[INET6_ADDRSTRLEN];

    handleChildProcessTermination();

    if (listen(socketFileDescriptor, BACKLOG) == -1) {
        perror("listen");
        onError();
    }

    printf("\n\n✅ server: waiting for connections on port: %d ...\n", port);

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

        TCPConnection* newConnection = newTCPConnection(connectedSocketFileDescriptor);

        // Imprime o IP de origem da conexão 
        inet_ntop(
            originConnectionAddress.ss_family,
            get_in_addr((struct sockaddr *)&originConnectionAddress),
            originIpAddress,
            sizeof originIpAddress);

        printf("\nserver: got connection from %s\n", originIpAddress);

        handdler(newConnection, context);
    }
}

int connectTo(char *host, int port) {
    int sockfd;
    struct sockaddr_in servaddr;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(host);
    servaddr.sin_port = htons(port);
 
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    return sockfd;
}
