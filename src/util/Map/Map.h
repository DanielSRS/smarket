#include "../Boolean/Boolean.h"
#include "../Cstrings/Cstrings.h" // isEquals alocated cstrings
#include "MapEntry/MapEntry.h"

#ifndef MAP_H
#define MAP_H

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
   * Define um outro Map como valor para a chave no objeto Map. Retorna o objeto Map
   * 
   * IMPORTANTE: value é integrado ao Map! Na destruição do map, esse novo map aninhado
   * também vai ser destruído!
   */
  struct Map*(*setMap)(struct Map* self, char* key, struct Map* value);
  struct Map*(*setList)(struct Map* self, char* key, void* value);
  struct Map* (*setNumber)(struct Map* self, char* key, double value);
  /**
   * Adiciona uma nova chave cujo valor é um novo outro Map
   * 
   * @return A referencia do novo map
   */
  struct Map*(*nest)(struct Map* self, char* key);
  /**
   * Adiciona uma nova chave cujo valor é uma lista
   */
  struct Map*(*nestList)(struct Map* self, char* key);
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
   * Retorna uma string formatada como JSON com todos os valores do Map
   */
  char*(*toJsonString)(struct Map* self);
  /**
   * Retorna uma das headers http
   */
  char*(*toHtttpHeadersCString)(struct Map* self);
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
  int (*pushString)(struct List* self, char *value);
  int (*pushMap)(struct List* self, Map *value);
  /**
   * Retorna uma string com todos os valores da Lista
   */
  alocatedCString (*toString)(struct List* self);
  /**
   * Retorna uma json string com todos os valores da Lista
   */
  alocatedCString (*toJsonString)(struct List* self);
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