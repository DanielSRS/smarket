#include "Map.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAP_OBJECT "__Map__"
#define STRING_OBJECT "__String__"
#define ANY_OBJECT "__Any__"

/** Compara se duas strings são iguais */
boolean isEquals(const char * first, const char *second) {
  if (strcmp(first, second) == 0) return True;
  return False;
}

/**
 * O usuário DEVE liberar a memória após o uso
*/
char *duplicateString(const char *stringToBeDuplicated) {
    char *destinationString = malloc(strlen (stringToBeDuplicated) + 1);  // Space for length plus nul
    if (destinationString == NULL) return NULL;                           // No memory
    strcpy(destinationString, stringToBeDuplicated);                      // Copy the characters
    return destinationString;                                             // Return the new string
}

boolean _hasElementInAMap(Map* self, char* key) {
  if (self->length == 0) return False;
  MapEntry *itemsInTheMap = (MapEntry *) self->_items;

  for (MapEntry *item = itemsInTheMap; item != NULL; item = item->sibling) {
    if(isEquals(item->key, key)) return True;
  }

  return False;
}

boolean _deleteElementInAMap(Map* self, char* key) {
  if (self->length == 0 || self->_items == NULL ) return False;
  MapEntry *itemsInTheMap = (MapEntry *) self->_items;

  /**
   * Se deletando o primeiro da lista
  */
  if (isEquals(self->_items->key, key)) {
    self->_items = self->_items->sibling;             // define o proximo item como entrada da lista
    *((int *) &self->length) = self->length - 1;      // reduz a contagem de itens

    boolean isItemAnMap = isEquals(itemsInTheMap->type, MAP_OBJECT);

    /**
     * Se o valor do item for um mapa aninhado, limpa todas
     * as entradas antes de liberar a memória do mapa aninhado
     */
    if (isItemAnMap) {
      Map *nestedMap = (Map*) itemsInTheMap->value;
      nestedMap->clear(nestedMap);                    // deve ser desroy não clear
    }

    itemsInTheMap->destroy(&itemsInTheMap);

    return True;
  }

  /**
   * Como previousItem nunca vai ser o elemento procurado, verifica então
   * sempre o proximo item da listas
   */
  for (MapEntry *previousItem = itemsInTheMap; previousItem != NULL;) {
    MapEntry *nextItem = previousItem->sibling;
    if (nextItem ==NULL) break;
    boolean isThisTheOneToDelete = isEquals(nextItem->key, key);

    if (isThisTheOneToDelete) {
      MapEntry *itemToDelete = nextItem;
      previousItem->sibling = nextItem->sibling;
      *((int *) &self->length) = self->length - 1;      // reduz a contagem de itens

      boolean isItemAnMap = isEquals(itemsInTheMap->type, MAP_OBJECT);

      /**
       * Se o valor do item for um mapa aninhado, limpa todas
       * as entradas antes de liberar a memória do mapa aninhado
       */
      if (isItemAnMap) {
        Map *nestedMap = (Map*) itemToDelete->value;
        nestedMap->clear(nestedMap);        // deve ser destroy
      }

      itemToDelete->destroy(&itemToDelete);

      return True;
    }

    previousItem = nextItem;
  }

  return False;
}

void* _getItemInAMap(Map* self, char* key) {
  MapEntry *itemsInTheMap = (MapEntry *) self->_items;

  for (MapEntry *item = itemsInTheMap; item != NULL; item = item->sibling) {
    if(isEquals(item->key, key)) return (void*) item->value;
  }

  return NULL;
}

char* _mapEntryToString(MapEntry* self) {
  int keyLenght = strlen(self->key);
  int typeLenght = strlen(self->type);
  int separatorLenght = 2;
  char *separator = ": ";

  /** Se o valor guardado for uma string */
  if (isEquals(self->type, STRING_OBJECT)) {
    int valueLenght = strlen((char*) self->value);
    int bufferSize = keyLenght + separatorLenght + valueLenght + 1;
    char *buffer = malloc(bufferSize);
    snprintf(buffer, bufferSize, "%s%s%s", self->key, separator, (char*) self->value);

    return buffer;
  }

  /** Se o valor guardado for um map */
  if (isEquals(self->type, MAP_OBJECT)) {
    int valueLenght = strlen("{ Map }");
    int bufferSize = keyLenght + separatorLenght + valueLenght + 1;
    char *buffer = malloc(bufferSize);
    snprintf(buffer, bufferSize, "%s%s%s", self->key, separator, "{ Map }");

    return buffer;
  }

  /** Se o valor guardado for de outro tipo */
  int bufferSize = keyLenght + separatorLenght + typeLenght + 1;
  char *buffer = malloc(bufferSize);
  snprintf(buffer, bufferSize, "%s%s%s", self->key, separator, self->type);

  return buffer;
}

char* _mapToString(Map* self) {
  int numberOfItems = self->length;
  if (numberOfItems == 0) return "{ }";
  char *begginingOfTheString = "{\n";
  char *endingOfTheString = "}";
  char *identation = "\t";
  char *endOfLine = ",\n";
  int identationLen = 1, endOfLineLen = 2;
  int endOfStringLen = 1; // \0 ao final da strin

  char *buffer = duplicateString("");
  for (MapEntry *item = (MapEntry *) self->_items; item != NULL; item = item->sibling) {
    /** Representação em string do valor da entrada*/
    const char *pairOfValues = item->toString(item);
    
    /** O tamanho do buffer e da representação do valor e do novo buffer para gurdar ambos */
    int bufferLength = buffer == NULL ? 0 : strlen(buffer);
    int pairOfValuesLen = strlen(pairOfValues);
    int newBufferLenght = bufferLength + identationLen + endOfLineLen + pairOfValuesLen + endOfStringLen;

    /** Aloca espaço suficiente para guradar os tamanhos */
    char *newBuffer = malloc(newBufferLenght);

    /** Copia todos os dado para o novo buffer */
    snprintf(newBuffer, newBufferLenght, "%s%s%s%s", buffer, identation, pairOfValues, endOfLine);
    
    /** Libera o espaço aclocado anteriorrmente */
    buffer == NULL ? 0 : free(buffer);

    // atualiza a referencia do buffer
    buffer = (char*) newBuffer;
  }

  int newBufferLenght = strlen(buffer) + strlen(begginingOfTheString) + strlen(endingOfTheString) + endOfStringLen;
  char *newBuffer = malloc(newBufferLenght);
  snprintf(newBuffer, newBufferLenght, "%s%s%s", begginingOfTheString, buffer, endingOfTheString);

  /** Libera o espaço aclocado anteriorrmente */
  buffer == NULL ? 0 : free(buffer);

  return newBuffer;
}

int _lengthOfAMap(Map* self) {
  return self->length;
}

/**
 * Destroi o registro liberando memória
 */
void destroyMapEntry(struct MapEntry **self) {
  if (!*self) return;
  free(*self);
  memset(*self, 0, sizeof(MapEntry));
  *self = NULL;
}

MapEntry *newMapEntry(char *key, void *value, char *type) {
  MapEntry *newEntry = (MapEntry*) malloc(sizeof(MapEntry));
  memset(newEntry, 0, sizeof(MapEntry));

  newEntry->key = key;
  newEntry->value = value;
  newEntry->type = type;
  newEntry->sibling = NULL;

  newEntry->destroy = destroyMapEntry;
  newEntry->toString = _mapEntryToString;
  return newEntry;
}

Map *_setElementOfAMap(Map* self, char* key, void* value) {
  MapEntry *newEntry = newMapEntry(
    duplicateString(key),
    value,
    duplicateString(ANY_OBJECT));
  newEntry->sibling = NULL;
  newEntry->value = value;

  /** Se vazio */
  if (self->length == 0) self->_items = newEntry;
  else {
    for (MapEntry *item = (MapEntry *) self->_items; item != NULL; item = item->sibling) {
      /** Se já existir uma entrada com a chave especificada, sobrescreve o valor */
      if(isEquals(item->key, key)) {
        item->value = value;

        // apaga a chave e o valor escritos na memória
        memset((void *) newEntry->key, 0, strlen(newEntry->key));
        memset((void *) newEntry->type, 0, strlen(newEntry->type));
        // libera pq não vai ser mais usado
        free((void *) newEntry->key);
        free((void *) newEntry->type);
        newEntry->destroy(&newEntry); // Não foi usado, já que a entrada anterior foi reaproveitada

        return self; // retorna antes para que length não seja incrementado
        break;
      }
      /** Do contrário, adiciona uma nova entrada aos outros itens */
      if (item->sibling == NULL) {
        item->sibling = newEntry;
        break;
      }
    }
  }

  *((int *) &self->length) = self->length + 1;
  return self;
}

void _clearAllKeyValuePairsFromAMap(Map *self) {
  MapEntry *itemsInTheMap = (MapEntry *) self->_items;

  /**
   * Se itemsInTheMap for NULL, o map não tem itens logo
   * não há nada para deletar
   */
  for (MapEntry *item = itemsInTheMap; item != NULL;) {
    MapEntry *nextItem = item->sibling;
    boolean isItemAnMap = isEquals(item->type, MAP_OBJECT);

    /**
     * Se o valor do item for um mapa aninhado, limpa todas
     * as entradas antes de liberar a memória do mapa aninhado
     */
    if (isItemAnMap) {
      Map *nestedMap = (Map*) item->value;
      nestedMap->clear(nestedMap); // deve ser destroy na verdade
    }

    item->destroy(&item);

    if(item != NULL) exit(1);

    item = nextItem;    // Repete caso exista um proximo item
  }

  int *length = (int *) &(self->length);
  *length = 0;
  self->_items = NULL;
}


Map* newMap() {
  Map *map = malloc(sizeof(Map));
  *((int *) &map->length) = 0;
  map->clear = _clearAllKeyValuePairsFromAMap;
  map->del = _deleteElementInAMap;
  map->get = _getItemInAMap;
  map->has = _hasElementInAMap;
  map->setAny = _setElementOfAMap;
  map->toString = _mapToString;
  map->_items = NULL;

  return map;
}
