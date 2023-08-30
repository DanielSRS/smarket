#include <string.h>
#include "Cstrings.h"

#define True 1
#define False 0

/** Compara se duas strings são iguais */
boolean isEquals(const char * first, const char *second) {
  if (strcmp(first, second) == 0) return True;
  return False;
}