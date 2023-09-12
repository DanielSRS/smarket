#include "Map.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAP_OBJECT "__Map__"
#define STRING_OBJECT "__String__"
#define ANY_OBJECT "__Any__"
#define LIST_OBJECT "__List__"


alocatedCString htttpHeadersCString(Map* self);
alocatedCString mapEntryToJSONString(MapEntry* self);
alocatedCString mapToJsonString(Map* self);
alocatedCString listToJsonString(List* self);
Map *nestList(Map* self, char* key);
Map *setList(Map* self, char* key, void* value);
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

    boolean isItemAnMap = isEquals(itemsInTheMap->type, MAP_OBJECT);
    boolean isString = isEquals(itemsInTheMap->type, STRING_OBJECT);
    boolean isList = isEquals(itemsInTheMap->type, LIST_OBJECT);

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

    /**
     * Se for uma string, apaga o conteudo e libera a memoria
    */
    if (isString) {
      char *stringVal = (char*) itemsInTheMap->value;
      memset((void*) stringVal, 0, strlen(stringVal));
      free(stringVal);
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

      boolean isItemAnMap = isEquals(itemToDelete->type, MAP_OBJECT);
      boolean isString = isEquals(itemToDelete->type, STRING_OBJECT);
      boolean isList = isEquals(itemToDelete->type, LIST_OBJECT);

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

/**
 * Libere a memoria apos o uso!!!!!!
*/
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
    char *mapStringfied = ((Map *) self->value)->toString((Map *) self->value);
    int valueLenght = strlen(mapStringfied);
    int bufferSize = keyLenght + separatorLenght + valueLenght + 1;
    char *buffer = malloc(bufferSize);
    snprintf(buffer, bufferSize, "%s%s%s", self->key, separator, mapStringfied);

    mapStringfied == NULL ? 0 : free(mapStringfied); // map.toString() aloca memoria

    return buffer;
  }

  /** Se o valor guardado for uma lista */
  if (isEquals(self->type, LIST_OBJECT)) {
    alocatedCString listStringfied = ((List *) self->value)->toString((List *) self->value);
    alocatedCString buffer = formatedCString(
      "%s: %s",
      self->key,
      listStringfied
    );

    freeAlocatedCString(listStringfied);

    return buffer;
  }

  /** Se o valor guardado for de outro tipo */
  int bufferSize = keyLenght + separatorLenght + typeLenght + 1;
  char *buffer = malloc(bufferSize);
  snprintf(buffer, bufferSize, "%s%s%s", self->key, separator, self->type);

  return buffer;
}

alocatedCString mapEntryValueToString(MapEntry* self) {
  int keyLenght = strlen(self->key);
  int typeLenght = strlen(self->type);
  int separatorLenght = 2;
  char *separator = ": ";

  /** Se o valor guardado for uma string */
  if (isEquals(self->type, STRING_OBJECT)) {
    int valueLenght = strlen((char*) self->value);
    int bufferSize = valueLenght + 1;
    char *buffer = malloc(bufferSize);
    snprintf(buffer, bufferSize, "%s", (char*) self->value);

    return buffer;
  }

  /** Se o valor guardado for um map */
  if (isEquals(self->type, MAP_OBJECT)) {
    char *mapStringfied = ((Map *) self->value)->toString((Map *) self->value);
    int valueLenght = strlen(mapStringfied);
    int bufferSize = valueLenght + 1;
    char *buffer = malloc(bufferSize);
    snprintf(buffer, bufferSize, "%s", mapStringfied);

    mapStringfied == NULL ? 0 : free(mapStringfied); // map.toString() aloca memoria

    return buffer;
  }

  /** Se o valor guardado for uma list */
  if (isEquals(self->type, LIST_OBJECT)) {
    alocatedCString stringfied = ((List *) self->value)->toString((List *) self->value);
    alocatedCString buffer = formatedCString("%s", stringfied);

    freeAlocatedCString(stringfied);

    return buffer;
  }

  /** Se o valor guardado for de outro tipo */
  int bufferSize = typeLenght + 1;
  char *buffer = malloc(bufferSize);
  snprintf(buffer, bufferSize, "%s", self->type);

  return buffer;
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

/**
 * Destroi o registro liberando memória
 */
void destroyMapEntry(struct MapEntry **self) {
  if (!*self) return;
  free(*self);
  memset(*self, 0, sizeof(MapEntry));
  *self = NULL;
}

void destroyMap(struct Map **self) {
  if (!*self) return;
  (*self)->clear(*self);               // Apaga todos os itens
  free(*self);
  memset(*self, 0, sizeof(Map));
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
  newEntry->toJsonString = mapEntryToJSONString;
  return newEntry;
}

Map *_setElementOfAMap(Map* self, char* key, void* value, char *type) {
  MapEntry *newEntry = newMapEntry(
    duplicateString(key),
    value,
    duplicateString(type));

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

Map *setAny(Map* self, char* key, void* value) {
  return _setElementOfAMap(self, key, value, ANY_OBJECT);
}

Map *setString(Map* self, char* key, char* value) {
  return _setElementOfAMap(self, key, duplicateString(value), STRING_OBJECT);
}

Map *setMap(Map* self, char* key, Map* value) {
  return _setElementOfAMap(self, key, value, MAP_OBJECT);
}

Map *nest(Map* self, char* key) {
  Map *newmap = newMap();
  _setElementOfAMap(self, key, newmap, MAP_OBJECT);
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
    boolean isItemAnMap = isEquals(item->type, MAP_OBJECT);
    boolean isString = isEquals(item->type, STRING_OBJECT);
    boolean isList = isEquals(item->type, LIST_OBJECT);

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

    /**
     * Se for uma string, apaga o conteudo e libera a memoria
    */
    if (isString) {
      char *stringVal = (char*) item->value;
      memset((void*) stringVal, 0, strlen(stringVal));
      free(stringVal);
    }

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

alocatedCString mapEntryToJSONString(MapEntry* self) {
  /** Se o valor guardado for uma string */
  if (isEquals(self->type, STRING_OBJECT)) {
    alocatedCString buffer = formatedCString("\"%s\":\"%s\"", self->key, (char*) self->value);

    return buffer;
  }

  /** Se o valor guardado for um map */
  if (isEquals(self->type, MAP_OBJECT)) {
    alocatedCString mapJsonString = ((Map *) self->value)->toJsonString((Map *) self->value);
    
    alocatedCString buffer = formatedCString("\"%s\":%s", self->key, mapJsonString);

    mapJsonString == NULL ? 0 : free(mapJsonString); // map.toString() aloca memoria

    return buffer;
  }

  /** Se o valor guardado for uma list */
  if (isEquals(self->type, LIST_OBJECT)) {
    alocatedCString mapJsonString = ((List *) self->value)->toJsonString((List *) self->value);
    
    alocatedCString buffer = formatedCString("\"%s\":%s", self->key, mapJsonString);

    freeAlocatedCString(mapJsonString);

    return buffer;
  }

  /** Se o valor guardado for de outro tipo */
  alocatedCString buffer = formatedCString("\"%s\":\"type -> %s\"", self->key, self->type);

  return buffer;
}

/** Se dentro de uma lista não tem key */
alocatedCString listEntryToJSONString(MapEntry* self) {
  /** Se o valor guardado for uma string */
  if (isEquals(self->type, STRING_OBJECT)) {
    alocatedCString buffer = formatedCString("\"%s\"", (char*) self->value);

    return buffer;
  }

  /** Se o valor guardado for um map */
  if (isEquals(self->type, MAP_OBJECT)) {
    alocatedCString mapJsonString = ((Map *) self->value)->toJsonString((Map *) self->value);
    
    alocatedCString buffer = formatedCString("%s", mapJsonString);

    freeAlocatedCString(mapJsonString);

    return buffer;
  }

  /** Se o valor guardado for uma list */
  if (isEquals(self->type, LIST_OBJECT)) {
    alocatedCString mapJsonString = ((List *) self->value)->toJsonString((List *) self->value);
    
    alocatedCString buffer = formatedCString("%s", mapJsonString);

    freeAlocatedCString(mapJsonString);

    return buffer;
  }

  /** Se o valor guardado for de outro tipo */
  alocatedCString buffer = formatedCString("\"type -> %s\"", self->type);

  return buffer;
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
  _setElementOfAMap(self, key, newlist, LIST_OBJECT);
  return self;
}

Map *setList(Map* self, char* key, void* value) {
  return _setElementOfAMap(self, key, value, LIST_OBJECT);
}


int listPushMap(List *self, Map *value) {
  alocatedCString key = intToCString(self->length(self));
  self->_map->setMap((Map*)self->_map, key, value);

  return self->length(self);

  freeAlocatedCString(key);
}
