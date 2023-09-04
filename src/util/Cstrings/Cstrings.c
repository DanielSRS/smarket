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