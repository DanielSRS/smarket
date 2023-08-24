#include "../src/util/TestSuite/TestSuite.h"
#include "../src/util/Map/Map.h"

TestResult _newMapEntryCreatesANonNullReference() {
  char *key = "idDaEntrada";
  char *value = "Sou um valor qualquer";
  char *type = "Custom type";
  MapEntry *entry = newMapEntry(key, (void *) value, type);

  return expectToBeNotNull(entry);
}


int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("_newMapEntryCreatesANonNullReference")
      it("Retorna uma referência válida ao criar novo MapEntry", _newMapEntryCreatesANonNullReference);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}