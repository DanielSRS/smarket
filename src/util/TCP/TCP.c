#include "TCP.h"
#include <stdlib.h> // calloc, free, NULL
#include "../Socket/socket.h" //recv
#include <unistd.h> // close
#include "../Logger/Logger.h"

#define SOCKET_NOT_SETTED_YET -2

void onGetAddressInfoError() {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  console->error(console, "Houve um erro!! encerrando por onGetAddressInfoError\n");
  console->destroy(&console);

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
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  if(!(*self)) {
    console->error(console, "Destruindo refferencia nula TCPConnecitonInfo\n");
    console->destroy(&console);

    return;
  }

  free(*self);
  *self = NULL;

  console->debug(console, "Destruindo TCPConnectionInfo\n");
  console->destroy(&console);
}

/** Cria um novo objeto TCPConnectionInfo */
TCPConnectionInfo* newTCPConnectionInfo(int socketDescriptor) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  TCPConnectionInfo* newConnectionInfo = calloc(1, sizeof(TCPConnectionInfo));

  newConnectionInfo->connectionDescriptor = socketDescriptor;
  newConnectionInfo->destroy = destroyTCPConnectionInfo;

  console->debug(console, "Criado novo objeto TCPConnectionInfo\n");
  console->destroy(&console);

  return newConnectionInfo;
}

/**
 * Le os dados enviados em uma conexão socket tcp
*/
int receive(TCPConnection *self, char *buffer, int bufferSize) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  int numberOfReceivedBytes = 0;
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;

  numberOfReceivedBytes = recv(connectioSocketFileDescriptor, buffer, bufferSize - 1, 0);
  buffer[numberOfReceivedBytes] = '\0';

  console->debug(console, "TCPConnection received\n");
  console->destroy(&console);

  return numberOfReceivedBytes;
}

/**
 * Envia dados por uma conexão socket tcp
*/
int TCPConnectionSend(TCPConnection* self, int numBytes, char* buffer) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  int numberOfBytesSent = -1;
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;

  numberOfBytesSent = send(connectioSocketFileDescriptor, buffer, numBytes, 0);

  console->debug(console, "TCPConnection send\n");
  console->destroy(&console);

  return numberOfBytesSent;
}

/**
 * Encerra uma conexão socket tcp
*/
void TCPConnectionClose(struct TCPConnection* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  /** Encerrando conexão */
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;
  close(connectioSocketFileDescriptor);

  console->debug(console, "Fechando TCPConnection\n");
  console->destroy(&console);
}

/**
 * Destrói o objeto TCPConnection.
*/
void destroyTCPConnection(struct TCPConnection** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  if(!(*self)) {
    console->error(console, "Destruindo refenrencia invalida de TCPConnection\n");
    console->destroy(&console);
    return;
  }

  (*self)->close(*self);
  
  (*self)->connectionInfo->destroy(&((*self)->connectionInfo));
  
  free(*self);
  *self = NULL;

  console->debug(console, "Destroi objeto TCPConnection\n");
  console->destroy(&console);
}

TCPConnection* newTCPConnection(int socketDescriptor) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  TCPConnection* newConnection = calloc(1, sizeof(TCPConnection));

  newConnection->close = TCPConnectionClose;
  newConnection->connectionInfo = newTCPConnectionInfo(socketDescriptor);
  newConnection->destroy = destroyTCPConnection;
  newConnection->receive = receive;
  newConnection->send = TCPConnectionSend;

  console->debug(console, "Novo objeto TCPConnection\n");
  console->destroy(&console);

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
    console->destroy(&console);
    close(sd);
  }

  free(*self);
  *self = NULL;

  console->debug(console, "Destroi objeto tcpServerConfig\n");
  console->destroy(&console);
}

/** Cria um novo objeto tcpServerConfig */
tcpServerConfig* newTcpServerConfig() {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  tcpServerConfig* newConfig = calloc(1, sizeof(tcpServerConfig));
  newConfig->port = DEFAULT_SERVER_PORT;
  newConfig->backlog = DEFAULT_BACKLOG_SIZE;
  newConfig->socketDescriptor = SOCKET_NOT_SETTED_YET;
  newConfig->destroy = destroyTcpServerConfig;
  newConfig->newConnectionHanddler = NULL;

  console->debug(console, "Cria novo tcpServerConfig\n");
  console->destroy(&console);

  return newConfig;
}

/** Destrói um objeto TCPServer */
void destroyTCPServer(TCPServer** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  if(!(*self)) {
    console->error(console, "Tendo destruir TCPServer nulo\n");
    console->destroy(&console);
    return;
  };

  tcpServerConfig* config = (*self)->serverConfiguration;
  config->destroy(&config);

  (*self)->serverConfiguration = NULL;
  (*self)->destroy = NULL;
  (*self)->serve = NULL;
  (*self)->setBacklogSize = NULL;
  (*self)->setPort = NULL;

  free(*self);
  *self = NULL;

  console->debug(console, "Destruindo TCPServer");
  console->destroy(&console);
}

/**
 * Começa a escutar por connexões com as configurações definidas
*/
void serve(struct TCPServer* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  int port = self->serverConfiguration->port;
  int socketDescriptor = createAndBindSocket(port, onGetAddressInfoError);
  self->serverConfiguration->socketDescriptor = socketDescriptor;

  if  (!(self->serverConfiguration->newConnectionHanddler)) {
    console->error(console, "Cannot serve. Handdler is null!\n");
    console->destroy(&console);
    return;
  }

  console->debug(console, "Iniciando servidor TCP\n");
  console->destroy(&console);

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
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  /** Atualiza backlog */
  self->serverConfiguration->backlog = newSize;

  console->debug(console, "Atualiza tamanho do backlog do TCPServer\n");
  console->destroy(&console);

  return self->serverConfiguration->backlog;
}


/** Define a função de processamento de novas conexões */
void setNewConnectionHanddler(struct TCPServer* self, void (*handdler)(TCPConnection* newConnection)) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  /** Atualiza configuração */
  self->serverConfiguration->newConnectionHanddler = handdler;

  console->debug(console, "Atualizado TCPServer handler\n");
  console->destroy(&console);
}

/**
 * Altera a porta do servidor
 * 
 * Só pode ser executado antes de serve() NUNCA depois!
*/
uint16_t setPort(struct TCPServer* self, uint16_t newPort) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  /** Atualiza configuração */
  self->serverConfiguration->port = newPort;

  console->debug(console, "Atualizado porta do TCPServer\n");
  console->destroy(&console);

  return self->serverConfiguration->port;
}

TCPServer* createTCPServer() {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  TCPServer* newServer = calloc(1, sizeof(TCPServer));

  newServer->destroy = destroyTCPServer;
  newServer->serve = serve;
  newServer->serverConfiguration = newTcpServerConfig();
  newServer->setBacklogSize = setBacklogSize;
  newServer->setPort = setPort;
  newServer->setNewConnectionHanddler = setNewConnectionHanddler;

  console->debug(console, "Criado TCPServer\n");
  console->destroy(&console);

  return newServer;
}

TCPClient* newTCPClient(char *host, uint16_t port) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  int sockeDescriptor = connectTo(host, port);

  console->debug(console, "Criado TCPClient\n");
  console->destroy(&console);

  return newTCPConnection(sockeDescriptor);
}
