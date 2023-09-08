#include <string.h> // strcmp
#include "Cstrings.h"
#include <stdlib.h> // malloc
#include <stdio.h> // sprintf

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