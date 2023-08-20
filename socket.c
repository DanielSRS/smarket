#include <netdb.h> //addrinfo
#include <stdio.h> // null e stderr
#include <string.h> // memset
#include <unistd.h> // cloes

#define PORT "3492"  // A porta usada para outros usuários se conectarem. 

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
