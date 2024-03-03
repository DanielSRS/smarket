#include "../Cstrings/Cstrings.h" // alocatedstirngs,
#include "../Boolean/Boolean.h" // boolean
#include "../Map/Map.h" // newMap(), Map
#include <stdlib.h> // null

#ifndef JSON_PARSER_H
#define JSON_PARSER_H


typedef enum ValueType { Obj, String, Array, Boolean, null, unknow, number } ValueType;

typedef struct ParsedValue
{
  ValueType type;
  Map *objValue;
  char *stringVAlue;
  // list value
  boolean boolValue;
  double numberValue;
} ParsedValue;

/**
 * Faz o parse de uma string no formato:
 * 
 * something in the string"
 * 
 * IMPORTANTE: Note que a string fecha aspas, mas não abre.
 * Isso pq a priori as aspas iniciais ja foram lidas.
 * 
 * parseString faz parte de um parser maior, então processa apenas
 * dados parciais
*/
alocatedCString parseString(int *offset, int max, const char *buffer);

ParsedValue parseValue(int *offset, int max, const char *buffer);

/** Reconehce um objeto */
Map* parseObject(int *offset, int max, const char *buffer);

typedef struct ParsedString
{
  alocatedCString string;
  boolean withErrors;
} ParsedString;


#endif // JSON_PARSER_H