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