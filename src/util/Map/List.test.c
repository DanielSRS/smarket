#include "../TestSuite/TestSuite.h"
#include "Map.h"

TestResult newListCreatesANonNullReference() {
  List *list = newList();

  return expectToBeNotNull(list);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("newListCreatesANonNullReference")
      it("Retorna uma referência válida ao criar nova List", newListCreatesANonNullReference);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}