#include "../Boolean/Boolean.h"

#ifndef MAP_H
#define MAP_H

typedef struct MapEntry
{
  const char *key;
  const void *value;
  /**
   * Indica o tipo do dado guardado
   * 
   * pode ser: __Map__, __String__, __Any__
   */
  const char *type;
  /**
   * Retorna uma string com a chave e o valor
   */
  const char*(*toString)(struct MapEntry* self);
  /**
   * Destroi o registro liberando memória
   */
  const void(*destroy)(struct MapEntry* self);

  struct MapEntry *sibling;
} MapEntry;

typedef struct Map
{
  /** Remove todos os pares chave/valor do Map. */
  const void(*clear)(struct Map* self);
  /**
   * Retorna true se o elemento no objeto Map existia e tenha sido removido,
   * ou false se o elemento não existia.
   * 
   * Map.has(key) irá retornar false após isso.
   */
  const boolean(*del)(struct Map* self, char* key);
  /**
   * Retorna uma asserção booleana se o valor tenha sido associado à chave no objeto Map ou não.
   */
  const boolean(*has)(struct Map* self, char* key);
  /**
   * Define o valor para a chave no objeto Map. Retorna o objeto Map
   */
  const void(*set)(struct Map* self, char* key, void* value);
  /** 
   * Retorna o tamanho do map. */
  const int length;
  /**
   * Retorna o valor associado à chave, ou null se não há nada.
   */
  void*(*get)(struct Map* self, char* key);
  /**
   * Retorna uma string com todos os valores do Map
   */
  char*(*toString)(struct Map* self);
  /**
   * Itens inseridos no Map
   */
  const struct MapEntry *_items;
} Map;

/**
 * Cria um novo map
*/
Map* newMap();

/**
 * MapEntry não aloca memoria para seus atributos.
 * 
 * antes do Destroy deve liberar a memória das referencias
 * manualmente
*/
MapEntry *newMapEntry(char *key, void *value, char *type);


#endif // MAP_H