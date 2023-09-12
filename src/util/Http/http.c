#include "http.h"
#include <stdio.h> // printf
#include <stdlib.h> // free, calloc
#include "../JsonParser/JsonParser.h" // parseObject
#include "../TCP/TCP.h" // TCPServer
#include "../Logger/Logger.h"
#include <string.h> // strlen

RequestHeaderInfo getHeadersInfo(const char *requestBuffer, int length) {
  RequestHeaderInfo info = { 0 };
  info.tooManyFields = False;
  int substringIndex = 0;
  int substringStart = 0;
  int fildCount = 0;

  for (int index = 0; index < length; index++) {
    /**
     * não considera encoding da string, mas como só me interessa
     * os caracteres de controle talves não tenha problema??????
    */
    char character = requestBuffer[index];
    boolean isEndOfLine = character == '\n';
    boolean isEmptyLine = isEndOfLine && (requestBuffer[substringStart] == '\r' || requestBuffer[substringStart] == '\n');

    /**
     * Uma linha em branco indica o fim da header
    */
    if (isEmptyLine) {
      info.size = index;
      break;
    }

    if (isEndOfLine) {
      /** Verifica se a quantidade máxima de campos já não foi preenchida */
      if (info.numberOfFields == MAX_HEADER_FIELDS) {
        info.tooManyFields = True;
        break;
      }

      boolean thereWasAnCarrier = requestBuffer[index - 1] == '\r';   // se string termina em \r\n
      int substringEnd = thereWasAnCarrier ? index - 2 : index -1;    // fim da string (removendo \r e \n)

      info.fildLengths[substringIndex] = substringEnd - substringStart;         // tamanho da string
      info.fildSubstrings[substringIndex] = (char*) requestBuffer + substringStart;     // ponteiro pra a substring

      substringIndex++;
      substringStart = index + 1;   // o primeiro caractere da próxima linha
      info.numberOfFields++;        // Atualiza a contagem de campos da header
    }
  }

  return info;
}

void IOPrintRequestHeaderInfo(const RequestHeaderInfo info) {
  printf("{\n");
  printf("\tTamanho das headers: %d\n", info.size);
  printf("\tQuantidade de campos: %d\n", info.numberOfFields);
  printf("\tErro no máximo de compos: %s\n", info.tooManyFields ? "Sim" : "Não");
  printf("\tCampos da header: [\n");
  for (int i = 0; i < info.numberOfFields; i++) {
    char *substring = info.fildSubstrings[i];
    int length = info.fildLengths[i];
    printf("\t\t");
    for (int j = 0; j <= length; j++) {
      printf("%c", substring[j]);
    }
    printf(",\n");
  }
  printf("\t]\n");
  printf("}\n");
}

void destroyRequest(Request* request) {
  // request->data->destroy(&(request->data));
  // request->headers->destroy(&(request->headers));

  // Map *data = request->data;
  // Map *headers = request->headers;

  // data->destroy(&data);
  // headers->destroy(&headers);

  // free(request->path);
  // free(request->protocolVersion);

  // request->destroy = NULL;
  // request->thereWasErrors = False;
}

Request parseRequest(const char *requestBuffer, int length) {
  RequestHeaderInfo info = getHeadersInfo(requestBuffer, length);
  Request request = { 0 };
  int charNaoLidos = info.fildLengths[0];

  /** Se houve algum erro apenas retorna sem processar o resto */
  if (info.size == 0 || info.tooManyFields || info.numberOfFields < 1 || info.fildLengths[0] < 14) {
    request.thereWasErrors = True;
    return request;
  }

  /** Descobre o método de requisição */
  int i = 0;
  int methodNameLength = 0;
  for (; i <= info.fildLengths[0]; i++) {
    char charactere = info.fildSubstrings[0][i];

    if (charactere == ' ') {
      methodNameLength = i;

      alocatedCString method = duplicateStringWithSize(info.fildSubstrings[0], methodNameLength);
      // printf("\nMetodo recebido: %s\n", method);
      if (isEquals("GET", method)){
        request.method = GET;
      } else if (isEquals("POST", method)) {
        request.method = POST;
      } else if (isEquals("PUT", method)) {
        request.method = PUT;
      } else if (isEquals("DELETE", method)) {
        request.method = DELETE;
      } else {
        request.thereWasErrors = True;
        free(method);
        return request;
      }

      //pathSubstring = info.fildSubstrings[0] + i + 1; // começa depois do methodo e do expaço em branco
      i = i + 1; // mesma coisa acima
      charNaoLidos = charNaoLidos - i;
      free(method);
      break;
    }
  }

  /** Descobre o path */
  char *pathSubstring = info.fildSubstrings[0] + i;
  for (i = 0; i <= charNaoLidos; i++) {
    char charactere = pathSubstring[i];

    if (charactere == ' ') {
      int pathLength = i;

      alocatedCString path = duplicateStringWithSize(pathSubstring, pathLength);
      // printf("\npath recebido: %s\n", path);

      request.path = path;

      //pathSubstring = info.fildSubstrings[0] + i + 1; // começa depois do methodo e do expaço em branco
      i = i + 1; // mesma coisa acima
      charNaoLidos = charNaoLidos - i;
      break;
    }
  }

  /** Descobre o path */
  char *versionSubstring = pathSubstring + i;
  alocatedCString version = duplicateStringWithSize(versionSubstring, charNaoLidos + 1);
  // printf("\nhttp version recebido: %s\n", version);

  request.protocolVersion = version;

  request.headers = newMap();

  // parsing das headers
  for (int n = 1; n < info.numberOfFields; n++) {
    char *fildSubstring = info.fildSubstrings[n];
    char substringLen = info.fildLengths[n];

    int m = 0;
    for (; m <= substringLen; m++) {
      char charactere = fildSubstring[m];

      if (charactere == ':') {
        alocatedCString key = duplicateStringWithSize(fildSubstring, m);
        // printf("\nkey lida: %s", key);

        char *valueSubS = fildSubstring + m + 1; // ignora a key e o espaço
        alocatedCString value = trimEnd(valueSubS, substringLen - m);
        // printf("\nvalue lida: %s", value);

        request.headers->setString(request.headers, key, value);
        break;
      }
    }
  }

  // body starts at info.size + 1
  int offset = info.size + 1;
  if (requestBuffer[offset] == '{') {
    offset++; // consome a abertura de chaves: "{";
    request.data = parseObject(&offset, length, requestBuffer);
  } else {
    request.data = newMap();
  }

  request.destroy = destroyRequest;

  return request;
}

void IOPrintRequest(Request request) {
  printf("{\n");
  printf("\tMethod: %s\n", request.method == GET ? "GET" : "Other");
  printf("\tPath: %s\n", request.path);
  printf("\tHttp version: %s\n", request.protocolVersion);
  printf("\tErrors?: %s\n", request.thereWasErrors ? "Yes" : "No");
  printf("\tHeaders: %s\n", request.headers->toString(request.headers));
  printf("\tData: %s\n", request.data->toString(request.data));
  printf("}\n");
}

/**** httpServerConfig *************************************************************************************************/

/** implementando opaque data type httpServerConfig */
typedef struct _httpServerConfig {
  /** Conexão tcp */
  TCPServer* tcpServer;
  /** Callback function */
  void (*handdler)(HTTPConnection *newConnection, void* context);
  /** Logger */
  Logger* logger;
  /** Contexto */
  void* context;
  /**
   * Destrói o objeto _httpServerConfig.
  */
  void (*destroy)(struct _httpServerConfig** self);
} httpServerConfig;

/** Destrói o objeto httpServerConfig. */
void destroyHttpServerConfig(httpServerConfig** self) {
  if (!(*self)) return;

  /** libera o servidor tcp */
  (*self)->tcpServer->destroy(&((*self)->tcpServer));

  (*self)->handdler = NULL;
  (*self)->logger->destroy(&((*self)->logger));

  free(*self);        // libera a estrutura
  *self = NULL;       // aponta para null
}

/** Cria novo objeto httpServerConfig */
static httpServerConfig* newHttpServerConfig() {
  httpServerConfig* newConfig = calloc(1, sizeof(httpServerConfig));

  newConfig->destroy = destroyHttpServerConfig;
  newConfig->tcpServer = createTCPServer();
  newConfig->handdler = NULL;
  newConfig->logger = createLogger();
  newConfig->context = NULL;

  return newConfig;
}

/**** end ****/

/**** HTTPConnectionInfo ***********************************************************************************************/

typedef struct _HTTPConnectionInfo
{
  /** Conexão tcp */
  TCPConnection* tcpConnection;
  /**
   * Destrói o objeto _HTTPConnectionInfo.
  */
  void (*destroy)(struct _HTTPConnectionInfo** self);
} HTTPConnectionInfo;

/** Destrói o objeto HTTPConnectionInfo. */
void destroyHTTPConnectionInfo(HTTPConnectionInfo** self) {
  if (!(*self)) return;

  (*self)->tcpConnection->close((*self)->tcpConnection); // termina a conexão tcp
  (*self)->tcpConnection->destroy(&((*self)->tcpConnection)); // destroi a conexão

  free(*self);
  *self = NULL;
}

HTTPConnectionInfo* newHTTPConnectionInfo(TCPConnection* connection) {
  HTTPConnectionInfo* new = calloc(1, sizeof(HTTPConnectionInfo));

  new->destroy = destroyHTTPConnectionInfo;
  new->tcpConnection = connection;

  return new;
}

/**** end ****/

/**** HTTPConnection ***************************************************************************************************/

/** Destroy objeto HTTPConnection */
void destroyHTTPConnection(HTTPConnection** self) {
  if (!(*self)) return;

  (*self)->close(*self);                                            // Encerra a conexão
  (*self)->connectionInfo->destroy(&((*self)->connectionInfo));     // libera conection info

  free(*self);
  *self = NULL;
}

void closeHTTPConnection(HTTPConnection* self) {
  self->connectionInfo->tcpConnection->close(self->connectionInfo->tcpConnection);
}

void responseHTTPConnection(HTTPConnection* self) {
  TCPConnection* tcpConnection = self->connectionInfo->tcpConnection;

  alocatedCString response = self->response->toString(self->response);
  int responseLength = cStringLenght(response);

  tcpConnection->send(tcpConnection, responseLength, response);
}

/** Cria objeto HTTPConnection */
HTTPConnection* newHTTPConnection(TCPConnection* connection, RequestHeaderInfo headerInfo, Request request) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  HTTPConnection* new = calloc(1, sizeof(HTTPConnection));

  new->destroy = destroyHTTPConnection;
  new->close = closeHTTPConnection;
  new->connectionInfo = newHTTPConnectionInfo(connection);
  new->headerInfo = headerInfo;
  new->request = request;
  new->sendResponse = responseHTTPConnection;
  new->response = newResponse();

  console->debug(console, "Criado HTTPConnection");
  console->destroy(&console);

  return new;
}

/**** end ****/

/**** HTTPServer *******************************************************************************************************/

/** destroi o objeto HTTPServer */
void destroyHTTPServer(HTTPServer** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  if(!(*self)) {
    console->error(console, "Destrindo uma referencia invalida de HTTPServer");
    console->destroy(&console);
    return;
  };

  console->error(console, "Destrindo objeto HTTPServer");
  console->destroy(&console);

  /** Destroi as configurações do servidor */
  (*self)->serverConfiguration->destroy(&((*self)->serverConfiguration));

  free(*self);
  *self = NULL;
}

/** Executa o servidor http */
void serveHTTPServer(HTTPServer *self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  /** Verifica se foi definido uma função de callback */
  void* handler = self->serverConfiguration->handdler;
  if (handler == NULL) {
    console->error(console, "Cannot serve. Handler was not set!");
    return;
  }

  console->debug(console, "Iniciando servidor");
  console->destroy(&console);

  /** Inicia servidor */
  TCPServer* tcpServer = self->serverConfiguration->tcpServer;
  tcpServer->serve(tcpServer);
}

/** Configura o tamanho do backlog do servidor */
HTTPServer* setHTTPServerBacklogSize(HTTPServer *self, unsigned int newSize) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  /** Atualiza backlog size */
  TCPServer* tcpServer = self->serverConfiguration->tcpServer;
  tcpServer->setBacklogSize(tcpServer, newSize);

  console->debug(console, "Atualiza HTTPServer backlog");
  console->destroy(&console);

  return self;
}

/** Configura a porta que o servidor escuta */
HTTPServer* setHTTPServerPort(HTTPServer *self, uint16_t newPort) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  /** Atualiza porta */
  TCPServer* tcpServer = self->serverConfiguration->tcpServer;
  tcpServer->setPort(tcpServer, newPort);

  console->debug(console, "Atualiza HTTPServer porta");
  console->destroy(&console);

  return self;
}

/** Configura a função de callback */
HTTPServer* setHTTPServerHandler(struct HTTPServer *self, void (*handdler)(HTTPConnection *newConnection, void* context)) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  /** atualiza handler */
  self->serverConfiguration->handdler = handdler;

  console->debug(console, "Atualiza HTTPServer handdler");
  console->destroy(&console);

  return self;
}

/** dispacher */
void dispatcher(TCPConnection* newConnection, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  /** Obtem referencia do servidor */
  HTTPServer* server = (HTTPServer*) context;
  
  int numberOfBytesWritten = 0;
  char buffer[1000];

  /** Lê os dados enviados pelo cliente */
  numberOfBytesWritten = newConnection->receive(newConnection, buffer, 1000);

  /** Verifica se houve erro */
  if (numberOfBytesWritten == -1) {
    console->error(console, "Erro ao ler request!!");
  }

  /** Garante que existe um null terminator na string */
  buffer[numberOfBytesWritten] = '\0';

  /** Processa a requisição */
  RequestHeaderInfo info = getHeadersInfo(buffer, numberOfBytesWritten);
  // IOPrintRequestHeaderInfo(info);
  Request request = parseRequest(buffer, numberOfBytesWritten);
  IOPrintRequest(request);

  /** Cria uma nova conexão http */
  HTTPConnection* httpcon = newHTTPConnection(newConnection, info, request);

  /** Chama o handler para lidar com a solicitação */
  server->serverConfiguration->handdler(httpcon, server->serverConfiguration->context);

  console->destroy(&console);
}

/** Define o contexto */
HTTPServer* setHTTPServerContext(void* context, struct HTTPServer* self) {
  self->serverConfiguration->context = context;
}

HTTPServer* createHTTPServer() {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "HTTP");

  HTTPServer* new = calloc(1, sizeof(HTTPServer));

  new->destroy = destroyHTTPServer;
  new->serve = serveHTTPServer;
  new->serverConfiguration = newHttpServerConfig();

  TCPServer* tcpServer = new->serverConfiguration->tcpServer;
  tcpServer->setNewConnectionHanddler(tcpServer, dispatcher);
  tcpServer->setContext(tcpServer, new); // define o próprio servidor http com contexto

  new->setBacklogSize = setHTTPServerBacklogSize;
  new->setNewConnectionHanddler = setHTTPServerHandler;
  new->setPort = setHTTPServerPort;
  new->setContext = setHTTPServerContext;

  console->debug(console, "Criado HTTPServer");
  console->destroy(&console);

  return new;
}

/**** end ****/
