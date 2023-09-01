#include "../TestSuite/TestSuite.h"
#include "Map.h"

TestResult newListCreatesANonNullReference() {
  List *list = newList();

  return expectToBeNotNull(list);
}

TestResult _mapAttrShouldBeNotNull() {
  List *list = newList();

  return expectToBeNotNull(list->_map);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("newListCreatesANonNullReference")
      it("Retorna uma referência válida ao criar nova List", newListCreatesANonNullReference);
      break;
    CASE ("_mapAttrShouldBeNotNull")
      it("Atributo _map de uma nova Lista não pode ser NULL", _mapAttrShouldBeNotNull);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}