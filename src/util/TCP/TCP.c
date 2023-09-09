#include "TCP.h"
#include <stdlib.h> // calloc, free, NULL
#include "../Socket/socket.h" //recv
#include <unistd.h> // close
#include "../Logger/Logger.h"

#define SOCKET_NOT_SETTED_YET -2

void onGetAddressInfoError() {
    exit(1);
};

typedef struct _TCPConnectionInfo
{
  /** Socket descriptor of the (new) connection */
  int connectionDescriptor;
  /** Destrói o objeto TCPConnectionInfo */
  void (*destroy)(struct _TCPConnectionInfo** self);
} TCPConnectionInfo;

/** Destrói um objeto TCPConnectionInfo */
void destroyTCPConnectionInfo(TCPConnectionInfo** self) {
  if(!(*self)) return;

  free(*self);
  *self = NULL;
}

/** Cria um novo objeto TCPConnectionInfo */
TCPConnectionInfo* newTCPConnectionInfo(int socketDescriptor) {
  TCPConnectionInfo* newConnectionInfo = calloc(1, sizeof(TCPConnectionInfo));

  newConnectionInfo->connectionDescriptor = socketDescriptor;
  newConnectionInfo->destroy = destroyTCPConnectionInfo;

  return newConnectionInfo;
}

/**
 * Le os dados enviados em uma conexão socket tcp
*/
int receive(TCPConnection *self, char *buffer, int bufferSize) {
  int numberOfReceivedBytes = 0;
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;

  numberOfReceivedBytes = recv(connectioSocketFileDescriptor, buffer, bufferSize - 1, 0);
  buffer[numberOfReceivedBytes] = '\0';

  return numberOfReceivedBytes;
}

/**
 * Envia dados por uma conexão socket tcp
*/
int TCPConnectionSend(TCPConnection* self, int numBytes, char* buffer) {
  int numberOfBytesSent = -1;
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;

  numberOfBytesSent = send(connectioSocketFileDescriptor, buffer, numBytes, 0);

  return numberOfBytesSent;
}

/**
 * Encerra uma conexão socket tcp
*/
void TCPConnectionClose(struct TCPConnection* self) {
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;

  close(connectioSocketFileDescriptor);
}

/**
 * Destrói o objeto TCPConnection.
*/
void destroyTCPConnection(struct TCPConnection** self) {
  if(!(*self)) return;

  (*self)->close(*self);
  
  (*self)->connectionInfo->destroy(&((*self)->connectionInfo));
  
  free(*self);
  *self = NULL;
}

TCPConnection* newTCPConnection(int socketDescriptor) {
  TCPConnection* newConnection = calloc(1, sizeof(TCPConnection));

  newConnection->close = TCPConnectionClose;
  newConnection->connectionInfo = newTCPConnectionInfo(socketDescriptor);
  newConnection->destroy = destroyTCPConnection;
  newConnection->receive = receive;
  newConnection->send = TCPConnectionSend;

  return newConnection;
}

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
   * Processa novas conexões
  */
  void (*newConnectionHanddler)(TCPConnection* newConnection);
  /**
   * Destrói o objeto _tcpServerConfig
  */
  void (*destroy)(struct _tcpServerConfig** self);
} tcpServerConfig;

/** Destrói um objeto tcpServerConfig */
void destroyTcpServerConfig(tcpServerConfig** self) {
  Logger* console = createLogger();
  console->extend(console, "TCP");

  if(!(*self)) return;

  int sd = (*self)->socketDescriptor;
  if (sd != SOCKET_NOT_SETTED_YET) {
    console->debug(console, "Close tcpServerConfig socket\n");
    close(sd);
  }

  free(*self);
  *self = NULL;
}

/** Cria um novo objeto tcpServerConfig */
tcpServerConfig* newTcpServerConfig() {
  tcpServerConfig* newConfig = calloc(1, sizeof(tcpServerConfig));
  newConfig->port = DEFAULT_SERVER_PORT;
  newConfig->backlog = DEFAULT_BACKLOG_SIZE;
  newConfig->socketDescriptor = SOCKET_NOT_SETTED_YET;
  newConfig->destroy = destroyTcpServerConfig;
  newConfig->newConnectionHanddler = NULL;

  return newConfig;
}

/** Destrói um objeto TCPServer */
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
  Logger* console = createLogger();
  console->extend(console, "TCP");

  int port = self->serverConfiguration->port;
  int socketDescriptor = createAndBindSocket(port, onGetAddressInfoError);
  self->serverConfiguration->socketDescriptor = socketDescriptor;

  if  (!(self->serverConfiguration->newConnectionHanddler)) {
    console->error(console, "Cannot serve. Handdler is null!\n");
    return;
  }

  listenForConnections(
    port,
    self->serverConfiguration->socketDescriptor,
    self->serverConfiguration->newConnectionHanddler,
    onGetAddressInfoError
  );
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


/** Define a função de processamento de novas conexões */
void setNewConnectionHanddler(struct TCPServer* self, void (*handdler)(TCPConnection* newConnection)) {
  self->serverConfiguration->newConnectionHanddler = handdler;
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
  newServer->setNewConnectionHanddler = setNewConnectionHanddler;

  return newServer;
}

TCPClient* newTCPClient(char *host, uint16_t port) {
  int sockeDescriptor = connectTo(host, port);

  return newTCPConnection(sockeDescriptor);
}
