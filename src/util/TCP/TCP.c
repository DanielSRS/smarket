#include "TCP.h"
#include <stdlib.h> // calloc, free, NULL
#include "../Socket/socket.h"

#define SOCKET_NOT_SETTED_YET -2

void onGetAddressInfoError() {
    exit(1);
};

/**
 * Configuraçõea de um servidor TCP
*/
typedef struct _tcpServerConfig
{
  /**
   * Porta que o servidor vai ficar escutando
  */
  uint16_t port;
  /**
   * Tamanho máximo da fila de solicitações de conexão.
   * Se a fila estiver cheia, todas as novas solicitações
   * são recusadas
  */
  int backlog;
  /**
   * Socket descriptor que o servidor vai ficar escutando
   * por conexões
  */
  int socketDescriptor;
  /**
   * Destrói o objeto _tcpServerConfig
  */
  void (*destroy)(struct _tcpServerConfig** self);
} tcpServerConfig;

void destroyTcpServerConfig(tcpServerConfig** self) {
  if(!(*self)) return;

  free(*self);
  *self = NULL;
}

tcpServerConfig* newTcpServerConfig() {
  tcpServerConfig* newConfig = calloc(1, sizeof(tcpServerConfig));
  newConfig->port = DEFAULT_SERVER_PORT;
  newConfig->backlog = DEFAULT_BACKLOG_SIZE;
  newConfig->socketDescriptor = SOCKET_NOT_SETTED_YET;
  newConfig->destroy = destroyTcpServerConfig;

  return newConfig;
}

void destroyTCPServer(TCPServer** self) {
  if(!(*self)) return;

  tcpServerConfig* config = (*self)->serverConfiguration;
  config->destroy(&config);

  (*self)->serverConfiguration = NULL;
  (*self)->destroy = NULL;
  (*self)->serve = NULL;
  (*self)->setBacklogSize = NULL;
  (*self)->setPort = NULL;

  free(*self);
  *self = NULL;
}

/**
 * Começa a escutar por connexões com as configurações definidas
*/
void serve(struct TCPServer* self) {
  int socketDescriptor = createAndBindSocket(onGetAddressInfoError);
  listenForConnections(socketDescriptor, onGetAddressInfoError);
}

/**
 * Altera o tamanho do backlog
 * 
 * Só pode ser executado antes de serve() NUNCA depois!
*/
int setBacklogSize(struct TCPServer* self, unsigned int newSize) {
  self->serverConfiguration->backlog = newSize;
  return self->serverConfiguration->backlog;
}

/**
 * Altera a porta do servidor
 * 
 * Só pode ser executado antes de serve() NUNCA depois!
*/
uint16_t setPort(struct TCPServer* self, uint16_t newPort) {
  self->serverConfiguration->port = newPort;
  return self->serverConfiguration->port;
}

TCPServer* createTCPServer() {
  TCPServer* newServer = calloc(1, sizeof(TCPServer));

  newServer->destroy = destroyTCPServer;
  newServer->serve = serve;
  newServer->serverConfiguration = newTcpServerConfig();
  newServer->setBacklogSize = setBacklogSize;
  newServer->setPort = setPort;

  return newServer;
}