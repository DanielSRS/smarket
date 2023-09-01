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
  char*(*toString)(struct MapEntry* self);
  /**
   * Destroi o registro liberando memória
   */
  const void(*destroy)(struct MapEntry** self);

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
  struct Map*(*setAny)(struct Map* self, char* key, void* value);
  /**
   * Define uma string como valor para a chave no objeto Map. Retorna o objeto Map
   * 
   * value é copiado e salvo no map
   */
  struct Map*(*setString)(struct Map* self, char* key, char* value);
  /**
   * Adiciona uma nova chave cujo valor é um novo outro Map
   */
  struct Map*(*nest)(struct Map* self, char* key);
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
   * Retorna uma lista de strings com todas as chaves do Map
   */
  char**(*getKeys)(struct Map* self);
  /**
   * Destroi o registro liberando memória
   */
  const void(*destroy)(struct Map** self);
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

/**
 * Uma lista
*/
typedef struct List
{
  /**
   * Retorna a quantidade de itens inseridos na lista
   */
  int (*length)(struct List* self);
  /**
   * Insere um item de qualquer tipo da lista.
   * 
   * O item é salvo como uma referencia. ao destruir o map o
   * dado não é destruido junto
   * 
   * retorna o novo tamanho da lista
   */
  int (*pushAny)(struct List* self, void *value);
  /**
   * Insere um item de tipo string na lsita.
   * 
   * O item é salvo como uma cópia do valor passado como parametro 
   * e é destruído junto com a destruição da lista.
   * 
   * modificar value após a inserção na lista não causa nenhum efeito
   * no dado salvo.
   */
  int (*pushString)(struct List* self, void *value);
  /**
   * Destroi a lista liberando memória. Todos os itens são destruídos junto
   * com a lista, exceto os itens salvos como referencia.
   */
  const void(*destroy)(struct List** self);
  /**
   * Dados da lista.
   * 
   * A lista é um envolucro ao redor de um Map
   */
  const struct Map *_map;
} List;

/**
 * Cria um nova lista
*/
List* newList();

#endif // MAP_H