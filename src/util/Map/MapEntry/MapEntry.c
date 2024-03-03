#include "MapEntry.h"       // NULL, formatedCString
#include "../Map.h"
#include <stdlib.h>         // calloc
#include <string.h>         // memset
#define _NULL 0

void destroyMapEntry(struct MapEntry **self);
alocatedCString _mapEntryToString(MapEntry* self);
alocatedCString mapEntryToJSONString(MapEntry* self);

/**
 * Cria novo MapEntry
*/
MapEntry *newMapEntry(char *key, void *value, EntryValueType type) {
  MapEntry *newEntry = (MapEntry*) calloc(1, sizeof(MapEntry));

  newEntry->key = key;
  newEntry->value = value;
  newEntry->type = type;
  newEntry->sibling = _NULL;

  newEntry->destroy = destroyMapEntry;
  newEntry->toString = _mapEntryToString;
  newEntry->toJsonString = mapEntryToJSONString;
  return newEntry;
}

/**
 * Destroi o registro liberando memória
 */
void destroyMapEntry(struct MapEntry **self) {
  if (!*self) return;
  memset(*self, 0, sizeof(MapEntry));         // Apaga o dado na mamoria
  free(*self);                                // Libera a memoria
  *self = _NULL;                              // 
}

/**
 * Libere a memoria apos o uso!!!!!!
*/
alocatedCString _mapEntryToString(MapEntry* self) {
  char *separator = ": ";

  /** Se o valor guardado for uma string */
  if (self->type == STRING_ENTRY_VALUE) {
    return formatedCString("%s%s%s", self->key, separator, (char*) self->value);
  }

  /** Se o valor guardado for um numero */
  if (self->type == NUMBER_ENTRY_VALUE) {
    return formatedCString("%s: %f", self->key, *((double*) self->value));
  }

  /** Se o valor guardado for um map */
  if (self->type == MAP_ENTRY_VALUE) {
    alocatedCString mapStringfied = ((Map *) self->value)->toString((Map *) self->value);
    alocatedCString buffer = formatedCString("%s%s%s", self->key, separator, mapStringfied);

    free(mapStringfied);

    return buffer;
  }

  /** Se o valor guardado for uma lista */
  if (self->type == LIST_ENTRY_VALUE) {
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
  return formatedCString("%s%s%s", self->key, separator, "UNKNOWN_ENTRY_VALUE");
}

alocatedCString mapEntryToJSONString(MapEntry* self) {
  /** Se o valor guardado for uma string */
  if (self->type == STRING_ENTRY_VALUE) {
    alocatedCString buffer = formatedCString("\"%s\":\"%s\"", self->key, (char*) self->value);

    return buffer;
  }
  
  /** Se o valor guardado for um numero */
  if (self->type == NUMBER_ENTRY_VALUE) {
    alocatedCString buffer = formatedCString("\"%s\":%f", self->key, *((double*) self->value));

    return buffer;
  }

  /** Se o valor guardado for um map */
  if (self->type == MAP_ENTRY_VALUE) {
    alocatedCString mapJsonString = ((Map *) self->value)->toJsonString((Map *) self->value);
    
    alocatedCString buffer = formatedCString("\"%s\":%s", self->key, mapJsonString);

    free(mapJsonString);

    return buffer;
  }

  /** Se o valor guardado for uma list */
  if (self->type == LIST_ENTRY_VALUE) {
    alocatedCString mapJsonString = ((List *) self->value)->toJsonString((List *) self->value);
    
    alocatedCString buffer = formatedCString("\"%s\":%s", self->key, mapJsonString);

    freeAlocatedCString(mapJsonString);

    return buffer;
  }

  /** Se o valor guardado for de outro tipo */
  alocatedCString buffer = formatedCString("\"%s\":\"%s\"", self->key,"UNKNOWN_ENTRY_VALUE");

  return buffer;
}

