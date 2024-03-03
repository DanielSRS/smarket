#include "../../TestSuite/TestSuite.h"      // TestResult, expectToBeNotNull, expectToBeNull, expectPointersToBeEquals
#include "MapEntry.h"                       // MapEntry

char *key = "idDaEntrada";
char *value = "Sou um valor qualquer";
double numberValue = 45.456;
EntryValueType type = UNKNOWN_ENTRY_VALUE;

TestResult _newMapEntryCreatesANonNullReference() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry);
}

TestResult _newMapEntrySiblingAttrShouldBeNull() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNull(entry->sibling);
}

TestResult _keyShouldBeTheSameAsParam() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectPointersToBeEquals(key, entry->key);
}

TestResult _valueShouldBeTheSameAsParam() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectPointersToBeEquals(value, entry->value);
}

TestResult _typeShouldBeTheSameAsParam() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectPointersToBeEquals(value, entry->value);
}

TestResult _destroyShouldBeNotNull() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry->destroy);
}

TestResult _toStringShouldBeNotNull() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry->toString);
}

TestResult _toJsonStringShouldBeNotNull() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry->toJsonString);
}

TestResult _pointerIsNullAfterDestroy() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  entry->destroy(&entry);

  return expectToBeNull(entry);
}

TestResult _toStringHasTheExpectedFormat() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  char *stringRepresentation = entry->toString(entry);

  return expectStringsToBeEquals("idDaEntrada: UNKNOWN_ENTRY_VALUE", stringRepresentation);
}

TestResult _toStringHasTheExpectedFormatStringVal() {
  MapEntry *entry = newMapEntry(key, (void *) value, STRING_ENTRY_VALUE);

  alocatedCString stringRepresentation = entry->toString(entry);

  return expectStringsToBeEquals("idDaEntrada: Sou um valor qualquer", stringRepresentation);
}

TestResult _toJsonStringHasTheExpectedFormat() {
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  char *stringRepresentation = entry->toJsonString(entry);

  return expectStringsToBeEquals("\"idDaEntrada\":\"UNKNOWN_ENTRY_VALUE\"", stringRepresentation);
}

TestResult _toJsonWithNumberValues() {
  MapEntry *entry = newMapEntry(key, (void *) &numberValue, NUMBER_ENTRY_VALUE);

  char *stringRepresentation = entry->toJsonString(entry);

  return expectStringsToBeEquals("\"idDaEntrada\":45.456", stringRepresentation);
}

TestResult _toStringWithNumberValues() {
  MapEntry *entry = newMapEntry(key, (void *) &numberValue, NUMBER_ENTRY_VALUE);

  char *stringRepresentation = entry->toString(entry);

  return expectStringsToBeEquals("idDaEntrada: 45.456", stringRepresentation);
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
    CASE ("_toJsonStringShouldBeNotNull")
      it("Atributo toJsonString tem valor válido (not null)", _toJsonStringShouldBeNotNull);
      break;
    CASE ("_pointerIsNullAfterDestroy")
      it("Destruir o MapEntry define self (ponteiro para si mesmo) como null", _pointerIsNullAfterDestroy);
      break;
    CASE ("_toStringHasTheExpectedFormat")
      it("toString retorna a representação do dado no formato esperado", _toStringHasTheExpectedFormat);
      break;
    CASE ("_toJsonStringHasTheExpectedFormat")
      it("toJsonString retorna a representação do dado no formato esperado", _toJsonStringHasTheExpectedFormat);
      break;
    CASE ("_toStringHasTheExpectedFormatStringVal")
      it("toJsonString retorna a representação do dado no formato esperado", _toStringHasTheExpectedFormatStringVal);
      break;
    CASE ("_toJsonWithNumberValues")
      it("toJsonString com um numero", _toJsonWithNumberValues);
      break;
    CASE ("_toStringWithNumberValues")
      it("toJsonString com um numero", _toStringWithNumberValues);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}