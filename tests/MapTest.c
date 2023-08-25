#include "../src/util/TestSuite/TestSuite.h" // strcpy
#include "../src/util/Map/Map.h"
#include <stdlib.h> // free

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

  return expectToBeNotNull(map->set);
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
  map->set(map, key, value);

  int itemsCount = map->length;

  return assertEquals(1, itemsCount);
}

TestResult _lengthShouldChangeAfterMultInsertion() {
  Map *map = newMap();
  map->set(map, "key1", "val1");
  map->set(map, "key2", "val2");
  map->set(map, "key3", "val3");
  map->set(map, "key4", "val4");
  map->set(map, "key5", "val5");
  map->set(map, "key6", "val6");
  map->set(map, "key7", "val7");

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
  map->set(map, "key1", "val1");
  map->set(map, "key2", "val2");
  map->set(map, "key3", "val3");
  map->set(map, "key4", "val4");
  map->set(map, "key5", "val5");
  map->set(map, "key6", "val6");
  map->set(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "something");

  return expectToBeFalse(hasTheKey);
}

TestResult _hasReturnTrueIfTheKeyIsInTheMap() {
  Map *map = newMap();
  map->set(map, "key1", "val1");
  map->set(map, "key2", "val2");
  map->set(map, "key3", "val3");
  map->set(map, "key4", "val4");
  map->set(map, "key5", "val5");
  map->set(map, "key6", "val6");
  map->set(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "key4");

  return expectToBeTrue(hasTheKey);
}

TestResult _hasReturnTrueIfTheKeyIsInTheBegginingOfTheMap() {
  Map *map = newMap();
  map->set(map, "key1", "val1");
  map->set(map, "key2", "val2");
  map->set(map, "key3", "val3");
  map->set(map, "key4", "val4");
  map->set(map, "key5", "val5");
  map->set(map, "key6", "val6");
  map->set(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "key1");

  return expectToBeTrue(hasTheKey);
}

TestResult _hasReturnTrueIfTheKeyIsInTheEndingOfTheMap() {
  Map *map = newMap();
  map->set(map, "key1", "val1");
  map->set(map, "key2", "val2");
  map->set(map, "key3", "val3");
  map->set(map, "key4", "val4");
  map->set(map, "key5", "val5");
  map->set(map, "key6", "val6");
  map->set(map, "key7", "val7");

  boolean hasTheKey = map->has(map, "key7");

  return expectToBeTrue(hasTheKey);
}

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
      it("Atributo set tem valor válido (not null)", _setShouldBeNotNull);
      break;
    CASE ("_toStringShouldBeNotNull")
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
      it("has retorna False se não houver items no Map", _hasReturnFalseIfThereNoItems);
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