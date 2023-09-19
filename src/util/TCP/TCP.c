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

  console->error(console, "Houve um erro!! encerrando por onGetAddressInfoError");
  console->destroy(&console);

  exit(1);
};

typedef struct _TCPConnectionInfo
{
  /** Socket descriptor of the (new) connection */
  int connectionDescriptor;
  /** Ip da conexão de origem */
  alocatedCString ip;
  /** Porta da conexão de origem */
  alocatedCString port;
  /** Destrói o objeto TCPConnectionInfo */
  void (*destroy)(struct _TCPConnectionInfo** self);
} TCPConnectionInfo;

/** Destrói um objeto TCPConnectionInfo */
void destroyTCPConnectionInfo(TCPConnectionInfo** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  if(!(*self)) {
    console->error(console, "Destruindo refferencia nula TCPConnecitonInfo");
    console->destroy(&console);

    return;
  }

  freeAlocatedCString((*self)->ip);
  freeAlocatedCString((*self)->port);
  free(*self);
  *self = NULL;

  console->debug(console, "Destruindo TCPConnectionInfo");
  console->destroy(&console);
}

/** Cria um novo objeto TCPConnectionInfo */
TCPConnectionInfo* newTCPConnectionInfo(int socketDescriptor, char* ip, char* port) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  TCPConnectionInfo* newConnectionInfo = calloc(1, sizeof(TCPConnectionInfo));

  newConnectionInfo->connectionDescriptor = socketDescriptor;
  newConnectionInfo->destroy = destroyTCPConnectionInfo;
  newConnectionInfo->ip = duplicateString(ip);
  newConnectionInfo->port = duplicateString(port);

  console->debug(console, "Criado novo objeto TCPConnectionInfo");
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

  console->debug(console, "TCPConnection received");
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

  console->debug(console, "TCPConnection send");
  console->destroy(&console);

  return numberOfBytesSent;
}

/**
 * Encerra uma conexão socket tcp
*/
void TCPConnectionClose(struct TCPConnection* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP[TCPConnectionClose]");

  /** Encerrando conexão */
  int connectioSocketFileDescriptor = self->connectionInfo->connectionDescriptor;
  if (connectioSocketFileDescriptor == -1) {
    console->debug(console, "Conexão já fechada");
    console->destroy(&console);
    return;
  }
  int result = close(connectioSocketFileDescriptor);

  if (result == 0) {
    console->debug(console, "Fechado TCPConnection");
    self->connectionInfo->connectionDescriptor = -1;
  } else {
    console->error(console, "Erro ao encerrar TCPConnection");
  }

  console->destroy(&console);
}

/**
 * Destrói o objeto TCPConnection.
*/
void destroyTCPConnection(struct TCPConnection** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP[destroyTCPConnection]");

  if(!(*self)) {
    console->error(console, "Destruindo refenrencia invalida de TCPConnection");
    console->destroy(&console);
    return;
  }

  console->debug(console, "Fechando conexão");
  (*self)->close(*self);
  
  console->debug(console, "Destruindo connectionInfo");
  (*self)->connectionInfo->destroy(&((*self)->connectionInfo));
  
  free(*self);
  *self = NULL;

  console->debug(console, "Destroi objeto TCPConnection");
  console->destroy(&console);
}

TCPConnection* newTCPConnection(int socketDescriptor, char* ip, char* port) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  TCPConnection* newConnection = calloc(1, sizeof(TCPConnection));

  newConnection->close = TCPConnectionClose;
  newConnection->connectionInfo = newTCPConnectionInfo(socketDescriptor, ip, port);
  newConnection->destroy = destroyTCPConnection;
  newConnection->receive = receive;
  newConnection->send = TCPConnectionSend;

  console->debug(console, "Novo objeto TCPConnection");
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
  void (*newConnectionHanddler)(TCPConnection* newConnection, void* context);
  /** Contexto para ser passado para newConnectionHanddler */
  void* context;
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
    console->debug(console, "Close tcpServerConfig socket");
    console->destroy(&console);
    close(sd);
  }

  free(*self);
  *self = NULL;

  console->debug(console, "Destroi objeto tcpServerConfig");
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
  newConfig->context = NULL;

  console->debug(console, "Cria novo tcpServerConfig");
  console->destroy(&console);

  return newConfig;
}

/** Destrói um objeto TCPServer */
void destroyTCPServer(TCPServer** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  if(!(*self)) {
    console->error(console, "Tendo destruir TCPServer nulo");
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
    console->error(console, "Cannot serve. Handdler is null!");
    console->destroy(&console);
    return;
  }

  console->debug(console, "Iniciando servidor TCP");
  console->destroy(&console);

  listenForConnections(
    port,
    self->serverConfiguration->socketDescriptor,
    self->serverConfiguration->newConnectionHanddler,
    self->serverConfiguration->context,
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

  console->debug(console, "Atualiza tamanho do backlog do TCPServer");
  console->destroy(&console);

  return self->serverConfiguration->backlog;
}


/** Define a função de processamento de novas conexões */
TCPServer* setNewConnectionHanddler(struct TCPServer* self, void (*handdler)(TCPConnection* newConnection, void* context)) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  /** Atualiza configuração */
  self->serverConfiguration->newConnectionHanddler = handdler;

  console->debug(console, "Atualizado TCPServer handler");
  console->destroy(&console);

  return self;
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

  console->debug(console, "Atualizado porta do TCPServer");
  console->destroy(&console);

  return self->serverConfiguration->port;
}

/**
 * Altera o contexto a ser passado para o handle
*/
TCPServer* setContext(struct TCPServer* self, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  /** Atualiza o contexto */
  self->serverConfiguration->context = context;

  console->debug(console, "Atualizado o contexto do TCPServer");
  console->destroy(&console);

  return self;
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
  newServer->setContext = setContext;

  console->debug(console, "Criado TCPServer");
  console->destroy(&console);

  return newServer;
}

TCPClient* newTCPClient(char *host, uint16_t port) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "TCP");

  int sockeDescriptor = connectTo(host, port);

  console->debug(console, "Criado TCPClient");
  console->destroy(&console);

  alocatedCString strPort = intToCString((int) port);

  TCPConnection* newConnection = newTCPConnection(sockeDescriptor, "-1.-1.-1.-1", strPort);

  freeAlocatedCString(strPort);

  return newConnection;
}
