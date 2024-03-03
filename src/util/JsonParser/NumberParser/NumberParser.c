#include "NumberParser.h"
#include "../../Logger/Logger.h"        // Logger, createLogger
#include "../../Cstrings/Cstrings.h"    // alocatedstirngs,
#include <stdlib.h>                     // exit
#include <stdio.h>                      // sscanf

#define _NULL 0

ParsedNumber parseNumber(int *offset, int max, const char *buffer) {
  // Status
  // 0 - não começou a analizar o numero
  // 1 - É negativo
  // 2 - É um numero menor ou igual a zero
  // 3 - primeiro digito de numero maior que zero
  // 4 - outros digitos de numero maior que zero
  // 5 -  inicio de fração
  // 6 - digitos da fraçaõ
  // 7 - Inicio do exponencial
  // 8 - sinal do exponencial
  // 9 - digitos do exponencial
  // 10 - terminou

  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "JsonParser[parseNumber]");
  // console->debug(console, "Iniciando parse do numero");

  int numberStart = *offset; // Inicia já tendo lido as " iniciais, então offset já é o primerio char do number

  int state = 0;
  boolean isFloat = False;
  boolean isScientific = False;
  alocatedCString string = _NULL;
  char input = ' ';
  ParsedNumber res = {
    .value = -1,
    .withErrors = True,
  };

  for (; *offset < max; *offset += 1) {
    input = buffer[*offset];

    // se olhando o primeiro caractere do numero
    if (state == 0) {

      if (input >= '1' && input <= '9') {
        state = 3;
      } else if (input == '-') {
        state = 1;
      } else if (input == '0') {
        state = 2;
      } else {
        console->error(console, "Estado 0; caractere invalido");
        return res;
      }
      continue;
    }

    // se inicio de numero negativo
    if (state == 1) {
      if (input >= '1' && input <= '9') {
        state = 3;
      } else if (input == '0') {
        state = 2; // zero negativo até então
      } else {
        console->error(console, "Estado 1; caractere invalido");
        return res;
      }
      continue;
    }

    if (state == 2) {
      // aqui já pode ser fim

      // espaço em brango
      if (input == ' ' || input == '\n' || input == '\r' || input == '\t') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == ',' || input == ']' || input == '}') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == '.') {
        state = 5;
        continue;
      }

      if (input == 'E' || input == 'e') {
        state = 7;
        continue;
      }

      console->error(console, "Estado 2; caractere invalido");
      return res;
    }

    if (state == 3) {
      // se o numero continua
      if (input >= '0' && input <= '9') {
        state = 4;
        continue;
      }

      // espaço em brango
      if (input == ' ' || input == '\n' || input == '\r' || input == '\t') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == ',' || input == ']' || input == '}') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == '.') {
        state = 5;
        continue;
      }

      if (input == 'E' || input == 'e') {
        state = 7;
        continue;
      }

      console->error(console, "Estado 3; caractere invalido");
      return res;
    }

    if (state == 4) {
      // se o numero continua
      if (input >= '0' && input <= '9') {
        continue;
      }

      // espaço em brango
      if (input == ' ' || input == '\n' || input == '\r' || input == '\t') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == ',' || input == ']' || input == '}') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == '.') {
        state = 5;
        continue;
      }

      if (input == 'E' || input == 'e') {
        state = 7;
        continue;
      }

      console->error(console, "Estado 4; caractere invalido");
      return res;
    }

    if (state == 5) {
      isFloat = True;
      // se o numero continua
      if (input >= '0' && input <= '9') {
        state = 6;
        continue;
      }

      console->error(console, "Estado 6; caractere invalido");
      return res;
    }

    if (state == 6) {
      // se o numero continua
      if (input >= '0' && input <= '9') {
        continue;
      }

      // espaço em brango
      if (input == ' ' || input == '\n' || input == '\r' || input == '\t') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == ',' || input == ']' || input == '}') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == 'E' || input == 'e') {
        state = 7;
        continue;
      }

      console->error(console, "Estado 6; caractere invalido");
      return res;
    }

    if (state == 7) {
      isScientific = True;
      if (input >= '0' && input <= '9') {
        state = 9;
        continue;
      }

      if (input == '-' || input == '+') {
        state = 8;
        continue;
      }

      console->error(console, "Estado 7; caractere invalido");
      return res;
    }

    if (state == 8) {
      if (input >= '0' && input <= '9') {
        state = 9;
        continue;
      }

      console->error(console, "Estado 8; caractere invalido");
      return res;
    }

    if (state == 9) {
      // se o numero continua
      if (input >= '0' && input <= '9') {
        continue;
      }

      // espaço em brango
      if (input == ' ' || input == '\n' || input == '\r' || input == '\t') {
        *offset -= 1;
        state = 10;
        continue;
      }

      if (input == ',' || input == ']' || input == '}') {
        *offset -= 1;
        state = 10;
        continue;
      }

      console->error(console, "Estado 9, caractere invalido");
      return res;
    }

    if (state == 10) {
      /** se a string é vazia */
      if (*offset == numberStart) {
        console->error(console, "Estado 10 - string vazia");
        exit(1);
      };                                                // erro! string não pode ser vazia
      int stringLength = *offset - numberStart;                                       // calula o tamanho da string
      // printf("stirng len: %d\n", stringLength);
      string = duplicateStringWithSize(buffer + numberStart, stringLength);
      // console->debug(console, "Estado 10 - numero achado: ");
      // console->debug(console, string);
      break;
    }

    console->error(console, "Sem Estado!!!");
  }

  if (isScientific) {
    sscanf(string, "%lf", &res.value);
  } else if (isFloat) {
    res.value = atof(string);
  } else {
    res.value = atoi(string);
  }
  res.withErrors = False;

  freeAlocatedCString(string);
  // console->debug(console, "numero encontrada");
  console->destroy(&console);
  return res;
}
