#include "http.h"
#include <stdio.h> // printf
#include <stdlib.h> // free

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
  if (info.size == 0 || info.tooManyFields, info.numberOfFields < 1 || info.fildLengths[0] < 14) {
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
  request.data = newMap();

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
        alocatedCString value = duplicateStringWithSize(valueSubS, substringLen - m);
        // printf("\nvalue lida: %s", value);

        request.headers->setString(request.headers, key, value);
        break;
      }
    }
  }

  // body starts at info.size + 1

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