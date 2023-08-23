#include "TestSuite.h" // TestResult, boolean
#include <stdio.h> // snprintf, fprintf
#include <stdlib.h> // exit

TestResult assertEquals(int expected, int received) {
  TestResult result;
  if (expected == received) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected %i but received: %i\n", expected, received);
  result.pass = False;

  return result;
}

int it(char *testDesciption, TestResult(*test)()) {
  TestResult testResult;
  testResult = test();

  if (testResult.pass) {
    printf("\n✅ %s - pass", testDesciption);
    return 0;
  } else {
    fprintf(stderr, "\n❌ %s - failed", testDesciption);
    fprintf(stderr, "\n\t\t%s", testResult.errorDescription);
    return 1;
  }
}

TestArgs _parseTestArgs(int argc, char **argv, void(*onEror)()) {
  TestArgs testArgs;
  /** Se não informado o nome do teste,falha*/
  if (argc <= 1) {
    fprintf(stderr, "❌ - Test name not provided");
    onEror();
  }

  /** Argumentos demais, não sei lidar */
  if (argc > 2) {
    fprintf(stderr, "❌ - Too many arguments, expected 1, but received: %i", argc - 1);
    onEror();
  }

  testArgs.testFileName = argv[0];
  testArgs.testName = argv[1];

  return testArgs;
}

/**
 * Encerra a execução do programa imadiatamente
*/
void exitOnError() {
  exit(1);
}

TestArgs parseTestArgs(int argc, char **argv) {
  _parseTestArgs(argc, argv, exitOnError);
}
