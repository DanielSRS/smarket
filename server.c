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
int createAndBindSocket(const struct addrinfo *serverAddressInfo, void (*onError)()) {
  const struct addrinfo *addressItem;
  int INVALID_FILE_DESCRIPTOR = -1, ERROR_ON_SET_SOCKET_OPTION = -1; // Ocoreu um ero na criação do socket
  int yes = 1;
  int socketFileDescriptor = -1;

  // Para cada item da lista de endereços, tenta criar e fazer bind.
  // para quando for concluido com sucesso em algum item da lista
  // Se houver erro, tentar com o proximo item até a lista acabar
  for(addressItem = serverAddressInfo; addressItem != NULL; addressItem = addressItem->ai_next) {

    // Cria o socket
    socketFileDescriptor = socket(
      addressItem->ai_family,
      addressItem->ai_socktype,
      addressItem->ai_protocol
    );

    if (socketFileDescriptor == INVALID_FILE_DESCRIPTOR) {
      perror("server: socket");
      continue;                   // Tenta cria o socket com o próximo item da lista
    }

    // Força o reuso do endereço pra evitar erros de porta ocupada
    int socketOptionRes = setsockopt(
      socketFileDescriptor,
      SOL_SOCKET,
      SO_REUSEADDR,
      &yes,
      sizeof(int)
    );

    if (socketOptionRes == ERROR_ON_SET_SOCKET_OPTION) {
        perror("setsockopt");
        onError();
    }

    // Faz o bind com a porta 
    int bindSocketResponse = bind(
      socketFileDescriptor,
      addressItem->ai_addr,
      addressItem->ai_addrlen
    );

    /** Se não for possivel fazer o bind*/
    if (bindSocketResponse == -1) {
        close(socketFileDescriptor);
        perror("server: bind");
        continue;
    }

    break; // Não houve erro. Encerra iteração
  }

  /**
   * Iterou a lista inteira e não foi possivel criar socket e fazer bind com 
   * nenum do itens
  */
  if (addressItem == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    onError();
  }

  return socketFileDescriptor;
}

int main(void)
{
    // File descriptor do socket. sockfd escuta por conexões. new_fd quando uma nova conexão é estabelecida. 
    int new_fd;
    struct addrinfo *servinfo;
    struct sockaddr_storage their_addr; // Informações do endereço de quem solicitou a conexão
    socklen_t sin_size;
    struct sigaction sa;
    char s[INET6_ADDRSTRLEN];

    servinfo = getAddressInfo(onGetAddressInfoError);

    int socketFileDescriptor = createAndBindSocket(servinfo, onGetAddressInfoError);
  
    freeaddrinfo(servinfo); // Libera a memória. Essa estrutura não vai ser mais usada. 

    if (listen(socketFileDescriptor, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("\n\n✅ server: waiting for connections on port: %s ...\n", PORT);

    // Loop principal para lidar com as solicitações de conexão 
    while(1) { 
        sin_size = sizeof their_addr;
        // Aceita uma conexão pendente 
        new_fd = accept(socketFileDescriptor, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) { // Se falha na conexão tenta a próxima 
            perror("accept");
            continue;
        }

        // Imprime o IP de origem da conexão 
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        // printando dados enviados pelo cliente 
        int numbytes;
        char buf[1000];
        if ((numbytes = recv(new_fd, buf, 1000 - 1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        printf("server: received data:\n\n '%s'",buf);

        // Cria um novo processo para responder a solicitação 

        if (!fork()) { // Este é o processo filho 
            close(socketFileDescriptor); // O processo filho não precisa dessa conexão

            // Resposta no formato definido pelo protocolo http 
            char *response = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nConnection: Keep-Alive\r\nContent-Type: text/plain\r\nContent-Length: 18\r\n\r\nHello, world!\r\n";
            
            // Envia a resposta 
            if (send(new_fd, response, strlen(response), 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}
