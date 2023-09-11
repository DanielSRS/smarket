#include "../../TestSuite/TestSuite.h"
#include "../Cstrings.h"

TestResult returnsRightSize() {
  char* string = "string ";
  char* formatada = "sformadata";
  int expectedSize = 31;
  int actualSize = formatedCStringSize("%s%s%d%s", "Eu sou uma ", string, 10, formatada);

  return assertEquals(expectedSize, actualSize);
}

TestResult returnsExpectedFormatedCString() {
  char* string = "string ";
  char* formatada = "sformadata";
  char* expectedString = "Eu sou uma string 10sformadata";
  alocatedCString actualString = formatedCString("%s%s%d%s", "Eu sou uma ", string, 10, formatada);

  return expectStringsToBeEquals(expectedString, actualString);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("returnsRightSize")
      it("Retorna o tamanho de uma string formatada", returnsRightSize);
      break;
    CASE ("returnsExpectedFormatedCString")
      it("String formatada adequadamente", returnsExpectedFormatedCString);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}