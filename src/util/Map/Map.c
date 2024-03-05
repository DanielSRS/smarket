#include "Map.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

alocatedCString htttpHeadersCString(Map* self);
alocatedCString mapToJsonString(Map* self);
alocatedCString listToJsonString(List* self);
Map *nestList(Map* self, char* key);
Map *setList(Map* self, char* key, void* value);
Map *setNumber(Map* self, char* key, double value);
int listPushMap(List *self, Map *value);

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

    boolean isItemAnMap = itemsInTheMap->type == MAP_ENTRY_VALUE;
    boolean isString = itemsInTheMap->type == STRING_ENTRY_VALUE;
    boolean isList = itemsInTheMap->type == LIST_ENTRY_VALUE;
    boolean isNumber = itemsInTheMap->type == NUMBER_ENTRY_VALUE;

    /**
     * Se o valor do item for um mapa aninhado, limpa todas
     * as entradas antes de liberar a memória do mapa aninhado
     */
    if (isItemAnMap) {
      Map *nestedMap = (Map*) itemsInTheMap->value;
      nestedMap->destroy(&nestedMap);
    }
    
    /** Se for uma lista */
    if (isList) {
      List* nestedList = (List*) itemsInTheMap->value;
      nestedList->destroy(&nestedList);
    }
    
    /** Se for uma numero */
    if (isNumber) {
      free((void*) itemsInTheMap->value);
    }

    /**
     * Se for uma string, apaga o conteudo e libera a memoria
    */
    if (isString) {
      freeAlocatedCString((char*) itemsInTheMap->value);
    }

    freeAlocatedCString(itemsInTheMap->key);
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

      boolean isItemAnMap = itemToDelete->type == MAP_ENTRY_VALUE;
      boolean isString = itemToDelete->type == STRING_ENTRY_VALUE;
      boolean isList = itemToDelete->type == LIST_ENTRY_VALUE;
      boolean isNumber = itemToDelete->type == NUMBER_ENTRY_VALUE;

      /**
       * Se o valor do item for um mapa aninhado, limpa todas
       * as entradas antes de liberar a memória do mapa aninhado
       */
      if (isItemAnMap) {
        Map *nestedMap = (Map*) itemToDelete->value;
        nestedMap->destroy(&nestedMap);
      }

      /** Se for uma lista */
      if (isList) {
        List* nestedList = (List*) itemToDelete->value;
        nestedList->destroy(&nestedList);
      }

      /** Se for um numero */
      if (isNumber) {
        free((void*) itemToDelete->value);
      }

      /**
       * Se for uma string, apaga o conteudo e libera a memoria
      */
      if (isString) {
        char *stringVal = (char*) itemToDelete->value;
        memset((void*) stringVal, 0, strlen(stringVal));
        free(stringVal);
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

alocatedCString mapEntryValueToString(MapEntry* self) {
  char *separator = ": ";

  /** Se o valor guardado for uma string */
  if (self->type == STRING_ENTRY_VALUE) {
    return formatedCString("%s", (char*) self->value);
  }

  /** Se o valor guardado for um numero */
  if (self->type == NUMBER_ENTRY_VALUE) {
    return formatedCString("%g", *((double*) self->value));
  }

  if (self->type == NULL_ENTRY_VALUE) {
    return formatedCString("%s", "null");
  }

  /** Se o valor guardado for um map */
  if (self->type == MAP_ENTRY_VALUE) {
    return ((Map *) self->value)->toString((Map *) self->value);
  }

  /** Se o valor guardado for uma list */
  if (self->type = LIST_ENTRY_VALUE) {
    return ((List *) self->value)->toString((List *) self->value);
  }

  /** Se o valor guardado for de outro tipo */
  return formatedCString("UNKNOWN_ENTRY_VALUE");
}

/**
 * Libere a memoria apos o uso!!!!!!
*/
alocatedCString _mapToString(Map* self) {
  int numberOfItems = self->length;
  if (numberOfItems == 0) return duplicateString("{ }");
  char *begginingOfTheString = "{\n";
  char *endingOfTheString = "}";
  char *identation = "\t";
  char *endOfLine = ",\n";
  int identationLen = 1, endOfLineLen = 2;
  int endOfStringLen = 1; // \0 ao final da strin

  char *buffer = duplicateString("");
  for (MapEntry *item = (MapEntry *) self->_items; item != NULL; item = item->sibling) {
    /** Representação em string do valor da entrada*/
    char *pairOfValues = item->toString(item);
    
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
    pairOfValues == NULL ? 0 : free(pairOfValues);

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

void destroyMap(struct Map **self) {
  if (!*self) return;
  (*self)->clear(*self);               // Apaga todos os itens
  free(*self);
  memset(*self, 0, sizeof(Map));
  *self = NULL;
}

Map *_setElementOfAMap(Map* self, char* key, void* value, EntryValueType type) {
  MapEntry *newEntry = newMapEntry(
    duplicateString(key),
    value,
    type);

  /** Se vazio */
  if (self->length == 0) self->_items = newEntry;
  else {
    for (MapEntry *item = (MapEntry *) self->_items; item != NULL; item = item->sibling) {
      /** Se já existir uma entrada com a chave especificada, sobrescreve o valor */
      if(isEquals(item->key, key)) {
        item->value = value;

        // apaga a chave e o valor escritos na memória
        freeAlocatedCString(newEntry->key);
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

Map *setAny(Map* self, char* key, void* value) {
  return _setElementOfAMap(self, key, value, UNKNOWN_ENTRY_VALUE);
}

Map *setString(Map* self, char* key, char* value) {
  return _setElementOfAMap(self, key, duplicateString(value), STRING_ENTRY_VALUE);
}

Map *setMap(Map* self, char* key, Map* value) {
  return _setElementOfAMap(self, key, value, MAP_ENTRY_VALUE);
}

Map *setNull(Map* self, char* key) {
  return _setElementOfAMap(self, key, NULL, NULL_ENTRY_VALUE);
}

Map *nest(Map* self, char* key) {
  Map *newmap = newMap();
  _setElementOfAMap(self, key, newmap, MAP_ENTRY_VALUE);
  return newmap;
}

void _clearAllKeyValuePairsFromAMap(Map *self) {
  MapEntry *itemsInTheMap = (MapEntry *) self->_items;

  /**
   * Se itemsInTheMap for NULL, o map não tem itens logo
   * não há nada para deletar
   */
  for (MapEntry *item = itemsInTheMap; item != NULL;) {
    MapEntry *nextItem = item->sibling;

    boolean isItemAnMap = item->type == MAP_ENTRY_VALUE;
    boolean isString = item->type == STRING_ENTRY_VALUE;
    boolean isList = item->type == LIST_ENTRY_VALUE;
    boolean isNumber = item->type == NUMBER_ENTRY_VALUE;

    /**
     * Se o valor do item for um mapa aninhado, limpa todas
     * as entradas antes de liberar a memória do mapa aninhado
     */
    if (isItemAnMap) {
      Map *nestedMap = (Map*) item->value;
      nestedMap->destroy(&nestedMap);
    }

    /** Is list */
    if (isList) {
      List *nestedMap = (List*) item->value;
      nestedMap->destroy(&nestedMap);
    }

    /** Is number */
    if (isNumber) {
      free((void*) item->value);
    }

    /**
     * Se for uma string, apaga o conteudo e libera a memoria
    */
    if (isString) {
      freeAlocatedCString((char*) item->value);
    }

    freeAlocatedCString(item->key);
    item->destroy(&item);

    if(item != NULL) exit(1);

    item = nextItem;    // Repete caso exista um proximo item
  }

  int *length = (int *) &(self->length);
  *length = 0;
  self->_items = NULL;
}

char **getMapKeys(Map *self) {
  int mapSize = self->length;
  if (mapSize == 0) {
    return NULL;
  }

  char **keysList = malloc(mapSize * sizeof(char *));

  MapEntry *itemsInTheMap = (MapEntry *) self->_items;

  int i = 0;
  for (MapEntry *item = itemsInTheMap; item != NULL; item = item->sibling, i++) {
    keysList[i] = duplicateString(item->key);
  }

  return keysList;
}

Map* newMap() {
  Map *map = malloc(sizeof(Map));
  *((int *) &map->length) = 0;
  map->clear = _clearAllKeyValuePairsFromAMap;
  map->del = _deleteElementInAMap;
  map->get = _getItemInAMap;
  map->has = _hasElementInAMap;
  map->setAny = setAny;
  map->setString = setString;
  map->setMap = setMap;
  map->toString = _mapToString;
  map->toJsonString = mapToJsonString;
  map->destroy = destroyMap;
  map->nest = nest;
  map->getKeys = getMapKeys;
  map->toHtttpHeadersCString = htttpHeadersCString;
  map->_items = NULL;
  map->nestList = nestList;
  map->setList = setList;
  map->setNumber = setNumber;
  map->setNull = setNull;

  return map;
}

int listLength(List *self) {
  return self->_map->length;
}

int listPushAny(List *self, void *value) {
  alocatedCString key = intToCString(self->length(self));
  self->_map->setAny((Map*) self->_map, key, value);

  return self->length(self);

  free(key);
}

int listPushString(List *self, char *value) {
  alocatedCString key = intToCString(self->length(self));
  self->_map->setString((Map*)self->_map, key, value);

  return self->length(self);

  free(key);
}

void destroyList(List **self) {
  if (!*self) return;
  Map* map = (Map*) (*self)->_map;
  (*self)->_map->destroy(&map);            // Apaga todos os itens
  free(*self);
  memset(*self, 0, sizeof(List));
  *self = NULL;
}

alocatedCString listToString(List* self) {
  int numberOfItems = self->length(self);
  if (numberOfItems == 0) return duplicateString("[ ]");
  char *begginingOfTheString = "[\n";
  char *endingOfTheString = "]";
  char *identation = "\t";
  char *endOfLine = ",\n";
  int identationLen = 1, endOfLineLen = 2;
  int endOfStringLen = 1; // \0 ao final da strin

  alocatedCString buffer = duplicateString("");
  for (MapEntry *item = (MapEntry *) self->_map->_items; item != NULL; item = item->sibling) {
    /** Representação em string do valor da entrada*/
    alocatedCString serializedValue = mapEntryValueToString(item);
    
    /** O tamanho do buffer e da representação do valor e do novo buffer para gurdar ambos */
    int bufferLength = strlen(buffer);
    int serializedValueLen = strlen(serializedValue);
    int newBufferLenght = bufferLength + identationLen + endOfLineLen + serializedValueLen + endOfStringLen;

    /** Aloca espaço suficiente para guradar os tamanhos */
    char *newBuffer = malloc(newBufferLenght);

    /** Copia todos os dado para o novo buffer */
    snprintf(newBuffer, newBufferLenght, "%s%s%s%s", buffer, identation, serializedValue, endOfLine);
    
    /** Libera o espaço aclocado anteriorrmente */
    free(buffer);
    free(serializedValue);

    // atualiza a referencia do buffer
    buffer = (char*) newBuffer;
  }

  int newBufferLenght = strlen(buffer) + strlen(begginingOfTheString) + strlen(endingOfTheString) + endOfStringLen;
  char *newBuffer = malloc(newBufferLenght);
  snprintf(newBuffer, newBufferLenght, "%s%s%s", begginingOfTheString, buffer, endingOfTheString);

  /** Libera o espaço aclocado anteriorrmente */
  free(buffer);

  return newBuffer;
}


// --------------------- Lista

List* newList() {
  List *list = malloc(sizeof(List));
  memset(list, 0, sizeof(List));

  list->_map = newMap();
  list->length = listLength;
  list->pushAny = listPushAny;
  list->pushString = listPushString;
  list->destroy = destroyList;
  list->toString = listToString;
  list->toJsonString = listToJsonString;
  list->pushMap = listPushMap;

  return list;
}


/**
 * USE APENAS SE TODOS OS VALORES NO MAP FOREM STRINGS!!!
*/
alocatedCString htttpHeadersCString(Map* self) {
  int numberOfItems = self->length;
  if (numberOfItems == 0) return "\r\n";

  alocatedCString buffer = duplicateString("");
  for (MapEntry *item = (MapEntry *) self->_items; item != NULL; item = item->sibling) {
    /** Formata os valores */ 
    alocatedCString newBuffer = formatedCString("%s%s: %s\r\n", buffer, item->key, (char*) item->value);
    
    /** Libera o espaço aclocado anteriorrmente */
    free(buffer);

    // atualiza a referencia do buffer
    buffer = newBuffer;
  }

  // adiciona linha em branco ao fim das headers
  alocatedCString newBuffer = formatedCString("%s\r\n", buffer);
  free(buffer);
  return newBuffer;
}

/** Se dentro de uma lista não tem key */
alocatedCString listEntryToJSONString(MapEntry* self) {
  /** Se o valor guardado for uma string */
  if (self->type == STRING_ENTRY_VALUE) {
    return formatedCString("\"%s\"", (char*) self->value);
  }
  
  /** Se o valor guardado for um numero */
  if (self->type == NUMBER_ENTRY_VALUE) {
    return formatedCString("%f", *((double*) self->value));
  }

  /** Se o valor guardado for um map */
  if (self->type == MAP_ENTRY_VALUE) {
    return ((Map *) self->value)->toJsonString((Map *) self->value);
  }

  /** Se o valor guardado for uma list */
  if (self->type == LIST_ENTRY_VALUE) {
    return ((List *) self->value)->toJsonString((List *) self->value);
  }

  /** Se o valor guardado for de outro tipo */
  return formatedCString("\"type -> UNKNOWN_ENTRY_VALUE\"");
}

alocatedCString mapToJsonString(Map* self) {
  int numberOfItems = self->length;
  if (numberOfItems == 0) return duplicateString("{ }");

  alocatedCString buffer = duplicateString("");
  int itemCount = 0;
  for (MapEntry *item = (MapEntry *) self->_items; item != NULL; item = item->sibling, ++itemCount) {
    /** Representação em string do valor da entrada*/
    alocatedCString keyValuePairs = item->toJsonString(item);
    
    /** Cria uma string formatada */
    alocatedCString newBuffer = NULL;
    if (itemCount != numberOfItems - 1) {                             // Se não for o ultimo da lista
      newBuffer = formatedCString("%s%s,", buffer, keyValuePairs);    // coloca virgula antes do proximo item
    } else {                                                          // caso seja o ultimo
      newBuffer = formatedCString("%s%s", buffer, keyValuePairs);     // não tem virgula
    }
    
    /** Libera o espaço aclocado anteriorrmente */
    buffer == NULL ? 0 : free(buffer);
    keyValuePairs == NULL ? 0 : free(keyValuePairs);

    // atualiza a referencia do buffer
    buffer = newBuffer;
  }

  // coloca as chaves no objeto
  alocatedCString newBuffer = formatedCString("%s%s%s", "{", buffer, "}");

  /** Libera o espaço aclocado anteriorrmente */
  buffer == NULL ? 0 : free(buffer);

  return newBuffer;
}

alocatedCString listToJsonString(List* self) {
  int numberOfItems = self->length(self);
  if (numberOfItems == 0) return duplicateString("[ ]");

  alocatedCString buffer = duplicateString("");
  int itemCount = 0;
  for (MapEntry *item = (MapEntry *) self->_map->_items; item != NULL; item = item->sibling, ++itemCount) {
    /** Representação em string do valor da entrada*/
    alocatedCString keyValuePairs = listEntryToJSONString(item);
    
    /** Cria uma string formatada */
    alocatedCString newBuffer = NULL;
    if (itemCount != numberOfItems - 1) {                             // Se não for o ultimo da lista
      newBuffer = formatedCString("%s%s,", buffer, keyValuePairs);    // coloca virgula antes do proximo item
    } else {                                                          // caso seja o ultimo
      newBuffer = formatedCString("%s%s", buffer, keyValuePairs);     // não tem virgula
    }
    
    /** Libera o espaço aclocado anteriorrmente */
    freeAlocatedCString(buffer);
    freeAlocatedCString(keyValuePairs);

    // atualiza a referencia do buffer
    buffer = newBuffer;
  }

  // coloca as chaves no objeto
  alocatedCString newBuffer = formatedCString("%s%s%s", "[", buffer, "]");

  /** Libera o espaço aclocado anteriorrmente */
  freeAlocatedCString(buffer);

  return newBuffer;
}

/** Adiciona uma lista como valor de uma chave */
Map *nestList(Map* self, char* key) {
  List* newlist = newList();
  _setElementOfAMap(self, key, newlist, LIST_ENTRY_VALUE);
  return self;
}

Map *setList(Map* self, char* key, void* value) {
  return _setElementOfAMap(self, key, value, LIST_ENTRY_VALUE);
}

int listPushMap(List *self, Map *value) {
  alocatedCString key = intToCString(self->length(self));
  self->_map->setMap((Map*)self->_map, key, value);

  return self->length(self);

  freeAlocatedCString(key);
}

Map* setNumber(Map* self, char* key, double value) {
  double* valueRef = calloc(1, sizeof(double));
  *valueRef = value;
  return _setElementOfAMap(self, key, valueRef, NUMBER_ENTRY_VALUE);
}
