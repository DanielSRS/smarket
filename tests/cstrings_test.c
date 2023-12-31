#include "../src/util/Cstrings/Cstrings.h"
#include "../src/util/TestSuite/TestSuite.h"

TestResult _compareIfStringsAreEqual() {
  const char* str1 = "Sou uma string aa";
  const char* str2 = "Sou uma string aa";
  const int result = isEquals(str1, str2);

  return assertEquals(True, result);
}

TestResult _compareIfStringsAreDiff() {
  const char* str1 = "Sou uma string aa";
  const char* str2 = "Sou uma string bb";
  const int result = isEquals(str1, str2);

  return assertEquals(False, result);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("_compareIfStringsAreEqual")
      it("Compara duas string e retorna true se forem iguais", _compareIfStringsAreEqual);
      break;
    CASE ("_compareIfStringsAreDiff")
      it("Compara duas string e retorna false se forem diferentes", _compareIfStringsAreDiff);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}