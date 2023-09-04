#include "http.h"
#include <stdio.h>

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