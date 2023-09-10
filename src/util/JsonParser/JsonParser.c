#include "JsonParser.h"
#include <stdio.h>

alocatedCString parseString(int *offset, int max, const char *buffer) {
  int stringStart = *offset; // Inicia já tendo lido as " iniciais, então offset já é o primerio char da string
  alocatedCString string = NULL;
  boolean readingAControl = False;


  for (; *offset < max; *offset += 1) {
    char input = buffer[*offset];

    if (readingAControl) {
      boolean a = input == '"'
               || input == '\\'
               || input == '/'
               || input == 'b'
               || input == 'f'
               || input == 'n'
               || input == 'r'
               || input == 't';
      if (a) {
        readingAControl = False;
        continue;
      };
      exit(1);                       // Erro!!!
    }

    /** fechamento da string */
    if (input == '\"') {
      /** se a string é vazia */
      if (*offset == stringStart) exit(1);                                                // erro! string não pode ser vazia
      int stringLength = *offset - stringStart;                                       // calula o tamanho da string
      // printf("stirng len: %d\n", stringLength);
      string = duplicateStringWithSize(buffer + stringStart, stringLength);       // salva a string
      if (string == NULL) {
        printf("string invalida\n");
        exit(1);
      }
      break;                                                                              // termina o loop
    }
    
    /** contrabarra */
    if (input == '\\') {
      readingAControl = True;
      continue;
    }

  }

  return string;
}


ParsedValue parseValue(int *offset, int max, const char *buffer) {
  int stringStart = *offset;
  // void *value = NULL;
  boolean FoundValue = False;
  ParsedValue value = { 0 };
  value.type = unknow;


  for (; *offset <= max; *offset += 1) {
    char input = buffer[*offset];

    /** Ignora espaços em branco */
    if (input == ' ') continue;
    if (input == '\n') continue;
    if (input == '\r') continue;
    if (input == '\t') continue;

    /** Value is string */
    if (input == '"') {
      if (FoundValue) {
        printf("as-Econtrado valor mais de uma vez\n");
        exit(1);        // valor ja encontrado!! erro
      }
      *offset += 1;      // consome a primeiras aspas
      FoundValue = True;
      value.stringVAlue = parseString(offset, max, buffer);
      value.type = String;
      continue;
    }

    /** Value is object */
    if (input == '{') {
      if (FoundValue) exit(1);        // valor ja encontrado!! erro
      *offset += 1;      // consome a abertura de chaves
      FoundValue = True;
      value.objValue = parseObject(offset, max, buffer);
      value.type = Obj;
      continue;                       // continua senão chega no exit();
    }

    /** Value is true */
    if (input == 't') {
      if (FoundValue) {
        printf("t-Econtrado valor mais de uma vez\n");
        exit(1);        // valor ja encontrado!! erro
      }
      boolean isTrue = buffer[*offset] == 't'
                    && buffer[*offset + 1] == 'r'
                    && buffer[*offset + 2] == 'u'
                    && buffer[*offset + 3] == 'e';
      if (isTrue) {
        FoundValue = True;
        *offset += 3;                   // avança os caracteres de true. o terceiro é incrementado pelo for
        // salva valor;
        value.type = Boolean;
        value.boolValue = True;
        continue;
      }
    }

    /** Value is false */
    if (input == 'f') {
      if (FoundValue) {
        printf("f-Econtrado valor mais de uma vez\n");
        exit(1);        // valor ja encontrado!! erro
      }
      boolean isFalse = buffer[*offset] == 'f'
                    && buffer[*offset + 1] == 'a'
                    && buffer[*offset + 2] == 'l'
                    && buffer[*offset + 3] == 's'
                    && buffer[*offset + 4] == 'e';
      if (isFalse) {
        FoundValue = True;
        *offset += 4;                   // avança os caracteres de true. o terceiro é incrementado pelo for
        // salva valor;
        value.type = Boolean;
        value.boolValue = False;
        continue;
      }
    }

    /** Value is null */
    if (input == 'n') {
      if (FoundValue) {
        printf("n-Econtrado valor mais de uma vez\n");
        exit(1);        // valor ja encontrado!! erro
      }
      boolean isNull = buffer[*offset] == 'n'
                    && buffer[*offset + 1] == 'u'
                    && buffer[*offset + 2] == 'l'
                    && buffer[*offset + 3] == 'l';
      if (isNull) {
        FoundValue = True;
        *offset += 3;                   // avança os caracteres de true. o terceiro é incrementado pelo for
        // salva valor;
        value.type = null;
        continue;
      }
    }

    if (input == ',' || input == ']' || input == '}') {
      if (!FoundValue || value.type == unknow) {
        printf("Valor não encontrado\n");
        exit(1);          // valor não encontrado. erro
      }
      *offset -= 1;                                          // decreemnta o offset pq: ',' ']' '}' dever ser lidos novamente fora de parsevalue
      break;
    }

    exit(1); // encontrado qualquer valor não esperado. erro
  }

  return value;
}

Map* parseObject(int *offset, int max, const char *buffer) {
  Map* obj = newMap();
  alocatedCString key = NULL;
  ParsedValue value = { 0 };
  value.type = unknow;

  for (; *offset <= max; *offset += 1) {
    char input = buffer[*offset];

    /** Ignora espaços em branco */
    if (input == ' ') continue;
    if (input == '\n') continue;
    if (input == '\r') continue;
    if (input == '\t') continue;

    /** Chave */
    if (input == '"') {
      if (key != NULL) exit(1);                 // key econtrada mais de uma vez!!!
      *offset += 1; // consome as aspas atuais
      key = parseString(offset, max, buffer);
    }
    
    /** Valor */
    if (input == ':') {
      if (key == NULL) exit(1);                   // começando a ler o valor mas a chave não existe
      *offset += 1; // consome os dois pontos atuais
      value = parseValue(offset, max, buffer);
    }

    // tem proximo
    if (input == ',') {
      if (value.type == unknow) {
        // valor não encontrado
        exit(1);
      }
      
      if (value.type == String) { // insere uma string
        obj->setString(obj, key, value.stringVAlue);
        key != NULL ? free(key) : 0; // libera a memria da chave
        value.stringVAlue != NULL ? free(value.stringVAlue) : 0; //libera a stirng
        key = NULL;
        value.type = unknow;
      }

      if (value.type == Obj) { // insere nested object
        obj->setMap(obj, key, value.objValue);
        key != NULL ? free(key) : 0; // libera a memria da chave
        key = NULL;
        value.type = unknow;
      }
    }

    // tem proximo
    if (input == '}') {
      if (value.type == unknow) {
        // valor não encontrado
        exit(1);
      }
      
      if (value.type == String) { // insere uma string
        obj->setString(obj, key, value.stringVAlue);
        key != NULL ? free(key) : 0; // libera a memria da chave
        value.stringVAlue != NULL ? free(value.stringVAlue) : 0; //libera a stirng
      }

      if (value.type == Obj) { // insere nested object
        obj->setMap(obj, key, value.objValue);
        key != NULL ? free(key) : 0; // libera a memria da chave
        key = NULL;
        value.type = unknow;
      }

      break; // todo o obejto foi reconhecido
    }
  }

  return obj;
}

