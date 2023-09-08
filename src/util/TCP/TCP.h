#include <stdint.h> // uint16_t

#ifndef TCP_H
#define TCP_H

#define DEFAULT_BACKLOG_SIZE 10
#define DEFAULT_SERVER_PORT 3490

typedef struct _tcpServerConfig tcpServerConfig;

/** Informações de uma conexão tcp estabelecida */
typedef struct _TCPConnectionInfo TCPConnectionInfo;

/**
 * Uma conexão tcp estabelecida
*/
typedef struct TCPConnection
{
  TCPConnectionInfo* connectionInfo;
  /**
   * Recebe os dados enviados.
   * 
   * Escreve um máximo de bufferSize-1 dados no buffer e insere
   * null terminator at bufferSize index
   * 
   * @param buffer Buffer para guardar os dados recebidos
   * @param bufferSize Tamanho do buffer.
   * @returns Numero de bytes efetivamente escritos no buffer
  */
  int (*receive)(struct TCPConnection* self, char* buffer, int bufferSize);
  /**
   * Send numBytes of data from buffer
   * 
   * @param numBytes Number of bytes to sent throught the connection
   * @param buffer Data to be sent
   * @returns Numero de bytes enviados ou -1 (erro)
  */
  int (*send)(struct TCPConnection* self, int numBytes, char* buffer);
  /**
   * Fecha a conexão
  */
  void (*close)(struct TCPConnection* self);
  /**
   * Destrói o objeto TCPConnection.
   * 
   * Se a conexão ainda estiver aberta, é fechada antes da
   * destruição do objeto.
  */
  void (*destroy)(struct TCPConnection** self);
} TCPConnection;

/**
 * Servidor tcp
*/
typedef struct TCPServer
{
  /**
   * configurações do servidor
  */
  tcpServerConfig* serverConfiguration;
  /**
   * Servidor tcp começa a escutar por conexões
  */
  void (*serve)(struct TCPServer* self);
  /**
   * Define o tamanho da file no backlog
   * 
   * @returns O novo tamanho da fila
  */
  int (*setBacklogSize)(struct TCPServer* self, unsigned int newSize);
  /**
   * Define a porta que o servidor vai escutar por conexões
   * 
   * @returns A nova porta definida
  */
  uint16_t (*setPort)(struct TCPServer* self, uint16_t newPort);
  /**
   * Define uma função para lidar com uma nova conexão
  */
  void (*setNewConnectionHanddler)(struct TCPServer* self, void (*handdler)(TCPConnection* newConnection));
  /**
   * Destrói o objeto TCPServer
  */
  void (*destroy)(struct TCPServer** self);
} TCPServer;

/**
 * Cria um servidor TCP
*/
TCPServer *createTCPServer();

/**
 * Cria um novo objeto TCPConnection
 * 
 * @param socketDescriptor socket descriptor da nova conexão
*/
TCPConnection* newTCPConnection(int socketDescriptor);

#endif // TCP_H