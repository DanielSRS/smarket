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


int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("_newMapEntryCreatesANonNullReference")
      it("Retorna uma referência válida ao criar novo MapEntry", _newMapEntryCreatesANonNullReference);
      break;
    CASE ("_newMapEntrySiblingAttrShouldBeNull")
      it("Atributo sibling de novo MapEntry deve ser NULL", _newMapEntrySiblingAttrShouldBeNull);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}