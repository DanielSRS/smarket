#include <string.h> // strcmp
#include "Cstrings.h"
#include <stdlib.h> // malloc

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
