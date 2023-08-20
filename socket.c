#include <netdb.h> //addrinfo
#include <stdio.h> // null e stderr
#include <string.h> // memset

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
