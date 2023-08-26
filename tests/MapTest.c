#include "../src/util/TestSuite/TestSuite.h" // strcpy
#include "../src/util/Map/Map.h"
#include <stdlib.h> // free
#include <stdio.h> //snprintf

TestResult _newMapCreatesANonNullReference() {
  Map *map = newMap();

  return expectToBeNotNull(map);
}

TestResult _newMapEntry_itemsAttrShouldBeNull() {
  Map *map = newMap();

  return expectToBeNull(map->_items);
}

TestResult _clearShouldBeNotNull() {
  Map *map = newMap();

  return expectToBeNotNull(map->clear);
}

TestResult _delShouldBeNotNull() {
  Map *map = newMap();

  return expectToBeNotNull(map->del);
}

TestResult _getShouldBeNotNull() {
  Map *map = newMap();

  return expectToBeNotNull(map->get);
}

TestResult _hasShouldBeNotNull() {
  Map *map = newMap();

  return expectToBeNotNull(map->has);
}

TestResult _setShouldBeNotNull() {
  Map *map = newMap();

  return expectToBeNotNull(map->setAny);
}

TestResult _toStringShouldBeNotNull() {
  Map *map = newMap();

  return expectToBeNotNull(map->toString);
}

TestResult _lengthShouldBeZero() {
  Map *map = newMap();
  int initialValueShouldBeZero = 0;
  int mapLength = map->length;

  return assertEquals(initialValueShouldBeZero, mapLength);
}

TestResult _lengthShouldChangeAfterInsertion() {
  Map *map = newMap();
  char *key = "chave";
  char *value = "valor";
  map->setAny(map, key, value);

  int itemsCount = map->length;

  return assertEquals(1, itemsCount);
}

TestResult _lengthShouldChangeAfterMultInsertion() {
  Map *map = newMap();
  map->setAny(map, "key1", "val1");
  map->setAny(map, "key2", "val2");
  map->setAny(map, "key3", "val3");
  map->setAny(map, "key4", "val4");
  map->setAny(map, "key5", "val5");
  map->setAny(map, "key6", "val6");
  map->setAny(map, "key7", "val7");

  int itemsCount = map->length;

  return assertEquals(7, itemsCount);
}

TestResult _hasReturnFalseIfThereNoItems() {
  Map *map = newMap();

  boolean hasTheKey = map->has(map, "anything");

  return expectToBeFalse(hasTheKey);
}

TestResult _hasReturnFalseIfThereNoMatchingItems() {
  Map *map = newMap();
  map->setAny(map, "key1", "val1");
  map->setAny(map, "key2", "val2");
  map->setAny(map, "key3", "val3");
  map->setAny(map, "key4", "val4");
  map->setAny(map, "key5", "val5");
  map->setAny(map, "key6", "val6");
  map->setAny(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "something");

  return expectToBeFalse(hasTheKey);
}

TestResult _hasReturnTrueIfTheKeyIsInTheMap() {
  Map *map = newMap();
  map->setAny(map, "key1", "val1");
  map->setAny(map, "key2", "val2");
  map->setAny(map, "key3", "val3");
  map->setAny(map, "key4", "val4");
  map->setAny(map, "key5", "val5");
  map->setAny(map, "key6", "val6");
  map->setAny(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "key4");

  return expectToBeTrue(hasTheKey);
}

TestResult _hasReturnTrueIfTheKeyIsInTheBegginingOfTheMap() {
  Map *map = newMap();
  map->setAny(map, "key1", "val1");
  map->setAny(map, "key2", "val2");
  map->setAny(map, "key3", "val3");
  map->setAny(map, "key4", "val4");
  map->setAny(map, "key5", "val5");
  map->setAny(map, "key6", "val6");
  map->setAny(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "key1");

  return expectToBeTrue(hasTheKey);
}

TestResult _hasReturnTrueIfTheKeyIsInTheEndingOfTheMap() {
  Map *map = newMap();
  map->setAny(map, "key1", "val1");
  map->setAny(map, "key2", "val2");
  map->setAny(map, "key3", "val3");
  map->setAny(map, "key4", "val4");
  map->setAny(map, "key5", "val5");
  map->setAny(map, "key6", "val6");
  map->setAny(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "key7");

  return expectToBeTrue(hasTheKey);
}

TestResult _hasAllTheInsertedItems() {
  Map *map = newMap();
  map->setAny(map, "key1a", "val1");
  map->setAny(map, "key2b", "val2");
  map->setAny(map, "kedy3", "val3");
  map->setAny(map, "key4g", "val4");
  map->setAny(map, "keys5", "val5");
  map->setAny(map, "kegy6", "val6");
  map->setAny(map, "kegy7", "val7");
  map->setAny(map, "ninal", "val8");

  boolean hasTheKey1 = map->has(map, "key1a");
  boolean hasTheKey2 = map->has(map, "key2b");
  boolean hasTheKey3 = map->has(map, "kedy3");
  boolean hasTheKey4 = map->has(map, "key4g");
  boolean hasTheKey5 = map->has(map, "keys5");
  boolean hasTheKey6 = map->has(map, "kegy6");
  boolean hasTheKey7 = map->has(map, "kegy7");
  boolean hasTheKey8 = map->has(map, "ninal");

  boolean lenghtIsCorrect = map->length == 8;

  boolean hasTheKeys = hasTheKey1
                    && hasTheKey2
                    && hasTheKey3
                    && hasTheKey4
                    && hasTheKey5
                    && hasTheKey6
                    && hasTheKey7
                    && hasTheKey8;

  return expectToBeTrue(hasTheKeys && lenghtIsCorrect);
}

TestResult _getReturnNullIfThereNoItems() {
  Map *map = newMap();

  void* itemValue = map->get(map, "anything");

  return expectToBeNull(itemValue);
}

TestResult _allTheInsertedValuesAreCorrect() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  boolean countIsCorrect = map->length == 7;

  boolean valuesAreTheSameInserted = 
      map->get(map, key1) == value1 &&
      map->get(map, key2) == value2 &&
      map->get(map, key3) == value3 &&
      map->get(map, key4) == value4 &&
      map->get(map, key5) == value5 &&
      map->get(map, key6) == value6 &&
      map->get(map, key7) == value7;

  return expectToBeTrue(valuesAreTheSameInserted && countIsCorrect);
}

TestResult _noRepeatedKeyAllowed() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key3",
       *key5 = "key5",
       *key6 = "key2",
       *key7 = "key2";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  boolean countIsCorrect = map->length == 4;

  boolean valuesAreTheSameInserted = 
      map->get(map, key1) == value1 &&
      map->get(map, key2) == value7 &&
      map->get(map, key3) == value4 &&
      map->get(map, key4) == value4 &&
      map->get(map, key5) == value5 &&
      map->get(map, key6) == value7 &&
      map->get(map, key7) == value7;

  return expectToBeTrue(valuesAreTheSameInserted && countIsCorrect);
}

TestResult _toStringOnEmptyMap() {
  Map *map = newMap();

  char *expectedFormat = "{ }";
  char *mapStringRepresentation = map->toString(map);

  return expectStringsToBeEquals(expectedFormat, mapStringRepresentation);
}

TestResult _toStringOnNonEmptyMap() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";

  char *type = "__Any__";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  char expectedFormat[500];
  snprintf(
    expectedFormat,
    500,
    "{\n\t%s: %s,\n\t%s: %s,\n\t%s: %s,\n\t%s: %s,\n\t%s: %s,\n\t%s: %s,\n\t%s: %s,\n}",
    key1, type,
    key2, type,
    key3, type,
    key4, type,
    key5, type,
    key6, type,
    key7, type
  );
  char *mapStringRepresentation = map->toString(map);

  return expectStringsToBeEquals(expectedFormat, mapStringRepresentation);
}

TestResult _lengthIsZeroAfterClear() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  int itemsCount = map->length == 7;                            // Antes de limpar
  map->clear(map);                                              // Removendo todas as entradas
  int itemsCountAfterClear = map->length == 0;                  // Depis de limpo

  return expectToBeTrue(itemsCount && itemsCountAfterClear);
}

TestResult _deleteTheFirstItemInserted() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  int itemsCount = map->length == 7;                            // Antes de limpar
  boolean isDeleted = map->del(map, key1);                      // Removendo entrada
  int itemsCountAfterDel = map->length == 6;                  // Depis de removido
  boolean hasDeletedkey = map->has(map, key1);

  return expectToBeTrue(itemsCount && isDeleted && itemsCountAfterDel && !hasDeletedkey);
}

TestResult _deleteTheLastItemInserted() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  int itemsCount = map->length == 7;                            // Antes de limpar
  boolean isDeleted = map->del(map, key7);                      // Removendo entrada
  int itemsCountAfterDel = map->length == 6;                  // Depis de removido
  boolean hasDeletedkey = map->has(map, key7);

  return expectToBeTrue(itemsCount && isDeleted && itemsCountAfterDel && !hasDeletedkey);
}

TestResult _deleteTheItemInsertedInTheMiddle() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  int itemsCount = map->length == 7;                            // Antes de limpar
  boolean isDeleted = map->del(map, key4);                      // Removendo entrada
  int itemsCountAfterDel = map->length == 6;                  // Depis de removido
  boolean hasDeletedkey = map->has(map, key4);

  return expectToBeTrue(itemsCount && isDeleted && itemsCountAfterDel && !hasDeletedkey);
}

TestResult _delReturnsFalseIfItemDoNotExist() {
  Map *map = newMap();
  char *key1 = "key1",
       *key2 = "key2",
       *key3 = "key3",
       *key4 = "key4",
       *key5 = "key5",
       *key6 = "key6",
       *key7 = "key7";
  
  char *value1 = "value1",
       *value2 = "value2",
       *value3 = "value3",
       *value4 = "value4",
       *value5 = "value5",
       *value6 = "value6",
       *value7 = "value7";
  
  map->setAny(map, key1, value1);
  map->setAny(map, key2, value2);
  map->setAny(map, key3, value3);
  map->setAny(map, key4, value4);
  map->setAny(map, key5, value5);
  map->setAny(map, key6, value6);
  map->setAny(map, key7, value7);

  int itemsCount = map->length == 7;                            // Antes de limpar
  boolean hasThekey = map->has(map, "key8");
  boolean isDeleted = map->del(map, "key8");                      // Removendo entrada
  int itemsCountAfterDel = map->length == 7;                  // Depis de removido

  return expectToBeTrue(itemsCount && !isDeleted && itemsCountAfterDel && !hasThekey);
}

// same key should override value

/*TestResult _pointerIsNullAfterDestroy() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  entry->destroy(&entry);

  return expectToBeNull(entry);
}*/

/*
TestResult _toStringHasTheExpectedFormat() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  char *stringRepresentation = entry->toString(entry);
  int stringLen = strlen(stringRepresentation);
  char received[stringLen];
  strcpy(received, stringRepresentation);

  /** toString aloca memória. É preciso liberar antes da asserção *//*
  free(stringRepresentation);

  return expectStringsToBeEquals("idDaEntrada: Custom type", received);
}
*/


int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("_newMapCreatesANonNullReference")
      it("Retorna uma referência válida ao criar novo Map", _newMapCreatesANonNullReference);
      break;
    CASE ("_newMapEntry_itemsAttrShouldBeNull")
      it("Atributo _items de novo Map deve ser NULL", _newMapEntry_itemsAttrShouldBeNull);
      break;
    CASE ("_clearShouldBeNotNull")
      it("Atributo clear tem valor válido (not null)", _clearShouldBeNotNull);
      break;
    CASE ("_delShouldBeNotNull")
      it("Atributo del tem valor válido (not null)", _delShouldBeNotNull);
      break;
    CASE ("_getShouldBeNotNull")
      it("Atributo get tem valor válido (not null)", _getShouldBeNotNull);
      break;
    CASE ("_hasShouldBeNotNull")
      it("Atributo has tem valor válido (not null)", _hasShouldBeNotNull);
      break;
    CASE ("_setShouldBeNotNull")
      it("Atributo setAny tem valor válido (not null)", _setShouldBeNotNull);
      break;
    CASE ("toStringShouldBeNotNull")
      it("Atributo toString tem valor válido (not null)", _toStringShouldBeNotNull);
      break;
    CASE ("_lengthShouldBeZero")
      it("Atributo length tem valor 0 (zero) quando um Map é criado", _lengthShouldBeZero);
      break;
    CASE ("_lengthShouldChangeAfterInsertion")
      it("A quantidade de items deve ser 1 após uma inserção", _lengthShouldBeZero);
      break;
    CASE ("_lengthShouldChangeAfterMultInsertion")
      it("A quantidade de items deve ser 7 após sete inserções inserção", _lengthShouldChangeAfterMultInsertion);
      break;
    CASE ("_hasReturnFalseIfThereNoItems")
      it("has retorna False se não houver itens no Map", _hasReturnFalseIfThereNoItems);
      break;
    CASE ("_hasReturnFalseIfThereNoMatchingItems")
      it("has retorna False se não houver item com a key procurada", _hasReturnFalseIfThereNoMatchingItems);
      break;
    CASE ("_hasReturnTrueIfTheKeyIsInTheMap")
      it("has retorna True se algum houver item com a key procurada", _hasReturnTrueIfTheKeyIsInTheMap);
      break;
    CASE ("_hasReturnTrueIfTheKeyIsInTheBegginingOfTheMap")
      it("has retorna True se o item com a key procurada estiver no início do Map", _hasReturnTrueIfTheKeyIsInTheBegginingOfTheMap);
      break;
    CASE ("_hasReturnTrueIfTheKeyIsInTheEndingOfTheMap")
      it("has retorna True se o item com a key procurada estiver no fim do Map", _hasReturnTrueIfTheKeyIsInTheEndingOfTheMap);
      break;
    CASE ("_hasAllTheInsertedItems")
      it("todas as chaves inseridas no Map estão presentes", _hasAllTheInsertedItems);
      break;
    CASE ("_getReturnNullIfThereNoItems")
      it("get retorna NULL se não houver itens no Map", _getReturnNullIfThereNoItems);
      break;
    CASE ("_allTheInsertedValuesAreCorrect")
      it("os valores recuperados do Map são os mesmos previamente inseridos", _allTheInsertedValuesAreCorrect);
      break;
    CASE ("_noRepeatedKeyAllowed")
      it("inserções com mesma key sobrescrevem valores setados anteriormente", _noRepeatedKeyAllowed);
      break;
    CASE ("_toStringOnEmptyMap")
      it("se a representação em string do map vazio está correta", _toStringOnEmptyMap);
      break;
    CASE ("_toStringOnNonEmptyMap")
      it("se a representação em string do map está correta", _toStringOnNonEmptyMap);
      break;
    CASE ("_lengthIsZeroAfterClear")
      it("o tamanho é zero após limpar todas as entradas", _lengthIsZeroAfterClear);
      break;
    CASE ("_deleteTheFirstItemInserted")
      it("deleta primeiro elemento inserido", _deleteTheFirstItemInserted);
      break;
    CASE ("_deleteTheLastItemInserted")
      it("deleta ultimo elemento inserido", _deleteTheLastItemInserted);
      break;
    CASE ("_deleteTheItemInsertedInTheMiddle")
      it("deleta elemento inserido no meio do Map", _deleteTheItemInsertedInTheMiddle);
      break;
    CASE ("_delReturnsFalseIfItemDoNotExist")
      it("tentar deletar item que não está no map retorna False", _delReturnsFalseIfItemDoNotExist);
      break;
/*
    CASE ("_pointerIsNullAfterDestroy")
      it("Destruir o MapEntry define self (ponteiro para si mesmo) como null", _pointerIsNullAfterDestroy);
      break;
    CASE ("_toStringHasTheExpectedFormat")
      it("toString retorna a representação do dado no formato esperado", _toStringHasTheExpectedFormat);
      break;*/
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}