#include <string.h> // strcmp
#include "Cstrings.h"
#include <stdlib.h> // malloc
#include <stdio.h> // sprintf
#include <stdarg.h> // va_list, va_arg, va_end

/** Compara se duas strings são iguais */
boolean isEquals(const char * first, const char *second) {
  if (strcmp(first, second) == 0) return True;
  return False;
}

alocatedCString intToCString(int value) {
  int maxStringSize = 12;                 // suficiente para representar um inteiro
  char *intString = (char*) malloc(maxStringSize * sizeof(char));
  sprintf(intString, "%d", value);
  return intString;
}

alocatedCString duplicateString(const char *stringToBeDuplicated) {
    char *destinationString = malloc(strlen (stringToBeDuplicated) + 1);  // Space for length plus nul
    if (destinationString == NULL) return NULL;                           // No memory
    strcpy(destinationString, stringToBeDuplicated);                      // Copy the characters
    return destinationString;                                             // Return the new string
}

alocatedCString duplicateStringWithSize(const char *stringToBeDuplicated, int length) {
    char *destinationString = malloc(length + 1);  // Space for length plus nul
    if (destinationString == NULL) return NULL;                           // No memory
    snprintf(destinationString, length + 1, "%s", stringToBeDuplicated);                      // Copy the characters
    return destinationString;                                             // Return the new string
}

alocatedCString trimEnd(const char *stringToBeTrimmed, int length) {
  int maxEnd = length;
  if (stringToBeTrimmed == NULL) return NULL;
  for (; maxEnd >= 0; maxEnd--) {
    /** Não sobrou string */
    if (maxEnd == 0) {
      return NULL;
    }

    char charactere = stringToBeTrimmed[maxEnd];
    boolean isNotAWhiteSpace = charactere != ' ' && charactere != '\0' && charactere != '\t' && charactere != '\r';

    if (isNotAWhiteSpace) {
      maxEnd++; // o indice atual já é um caractere valido, então o tamanho é indice + 1
      break;
    }
  }

  return duplicateStringWithSize(stringToBeTrimmed, maxEnd);
}

/** Calcula o tamanho da string formatada considerando o null terminator */
int vFormatedCStringSize(const char *__restrict__ __format, va_list args) {
  int size = vsnprintf(NULL, 0, __format, args) + 1;
  return size;
}

/** Formated CStrings size */
int formatedCStringSize(const char *__restrict__ __format, ...) {
  va_list args;
  va_start(args, __format);
  int size = vFormatedCStringSize(__format, args);
  va_end(args);

  return size;
}

/** Formated CStrings */
alocatedCString formatedCString(const char *__restrict__ __format, ...) {
  va_list args;
  /** Obtem o tamanho da nova string */
  va_start(args, __format);
  int newStringSize = vFormatedCStringSize(__format, args);
  va_end(args);

  /** Aloca memória */
  alocatedCString newString = calloc(newStringSize, sizeof(char));
  
  /** Cria a nova string */
  va_start(args, __format);
  int size = vsnprintf(newString, newStringSize, __format, args);
  va_end(args);

  return newString;
}
