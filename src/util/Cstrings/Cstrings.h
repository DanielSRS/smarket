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

/**
 * O usuário DEVE liberar a memória após o uso
*/
alocatedCString duplicateString(const char *stringToBeDuplicated);

alocatedCString duplicateStringWithSize(const char *stringToBeDuplicated, int length);

alocatedCString trimEnd(const char *stringToBeTrimmed, int length);

/** Calcula o tamanho de uma string formatada */
int formatedCStringSize(const char *__restrict__ __format, ...);

/** Cria uma CString formatada considerando */
alocatedCString formatedCString(const char *__restrict__ __format, ...);

/** Calcula o tamanho de uma string */
int cStringLenght(const char* string);

#endif //CSGSL_H