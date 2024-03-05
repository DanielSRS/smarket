#include "../../Cstrings/Cstrings.h" // alocatedCStrings, NULL, formatedCString

#ifndef MAP_ENTRY_H
#define MAP_ENTRY_H

/**
 * Tipo do dado guardado no MapEntry
*/
typedef enum EntryValueType {
  MAP_ENTRY_VALUE,
  STRING_ENTRY_VALUE,
  UNKNOWN_ENTRY_VALUE,
  NUMBER_ENTRY_VALUE,
  LIST_ENTRY_VALUE,
  NULL_ENTRY_VALUE,
} EntryValueType;

/**
 * Dados de um Map
*/
typedef struct MapEntry
{
  char *key;
  void *value;
  /**
   * Indica o tipo do dado guardado
   */
  EntryValueType type;
  /**
   * Retorna uma string com a chave e o valor
   * 
   * EX: key: value
   */
  char*(*toString)(struct MapEntry* self);
  /**
   * Retorna uma string com a chave e o valor no formato json
   * 
   * EX: "key": "value"
   */
  char*(*toJsonString)(struct MapEntry* self);
  /**
   * Destroi o registro liberando memória
   */
  void(*destroy)(struct MapEntry** self);

  struct MapEntry *sibling;
} MapEntry;

/**
 * MapEntry não aloca memoria para seus atributos.
 * 
 * antes do Destroy deve liberar a memória das referencias
 * manualmente
*/
MapEntry *newMapEntry(char *key, void *value, EntryValueType type);

#endif