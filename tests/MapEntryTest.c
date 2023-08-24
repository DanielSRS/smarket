#include "../src/util/TestSuite/TestSuite.h"
#include "../src/util/Map/Map.h"

TestResult _newMapEntryCreatesANonNullReference() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry);
}

TestResult _newMapEntrySiblingAttrShouldBeNull() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNull(entry->sibling);
}

TestResult _keyShouldBeTheSameAsParam() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectPointersToBeEquals(key, entry->key);
}

TestResult _valueShouldBeTheSameAsParam() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectPointersToBeEquals(value, entry->value);
}

TestResult _typeShouldBeTheSameAsParam() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectPointersToBeEquals(value, entry->value);
}

TestResult _destroyShouldBeNotNull() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry->destroy);
}

TestResult _toStringShouldBeNotNull() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry->toString);
}

TestResult _pointerIsNullAfterDestroy() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  entry->destroy(&entry);

  return expectToBeNull(entry);
}


int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("_newMapEntryCreatesANonNullReference")
      it("Retorna uma referência válida ao criar novo MapEntry", _newMapEntryCreatesANonNullReference);
      break;
    CASE ("_newMapEntrySiblingAttrShouldBeNull")
      it("Atributo sibling de novo MapEntry deve ser NULL", _newMapEntrySiblingAttrShouldBeNull);
      break;
    CASE ("_keyShouldBeTheSameAsParam")
      it("Atributo key tem o mesmo valor informado como parametro", _keyShouldBeTheSameAsParam);
      break;
    CASE ("_valueShouldBeTheSameAsParam")
      it("Atributo value tem o mesmo valor informado como parametro", _valueShouldBeTheSameAsParam);
      break;
    CASE ("_typeShouldBeTheSameAsParam")
      it("Atributo type tem o mesmo valor informado como parametro", _typeShouldBeTheSameAsParam);
      break;
    CASE ("_destroyShouldBeNotNull")
      it("Atributo destroy tem valor válido (not null)", _destroyShouldBeNotNull);
      break;
    CASE ("_toStringShouldBeNotNull")
      it("Atributo toString tem valor válido (not null)", _toStringShouldBeNotNull);
      break;
    CASE ("_pointerIsNullAfterDestroy")
      it("Destruir o MapEntry define self (ponteiro para si mesmo) como null", _pointerIsNullAfterDestroy);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}