#include "../Boolean/Boolean.h" // boolean

#ifndef CSGSL_H
#define CSGSL_H

/**
 * Explicita que um char* teve memória alocada
*/
typedef char* alocatedCString;

boolean isEquals(const char * first, const char *second);

/**
 * Converte um inteiro em string
*/
alocatedCString intToCString(int value);

#endif //CSGSL_H