#include <stdint.h> // uint16_t

#ifndef TCP_H
#define TCP_H

#define DEFAULT_BACKLOG_SIZE 10
#define DEFAULT_SERVER_PORT 3492

typedef struct _tcpServerConfig tcpServerConfig;


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
   * Destrói o objeto TCPServer
  */
  void (*destroy)(struct TCPServer** self);
} TCPServer;

TCPServer *createTCPServer();

#endif // TCP_H