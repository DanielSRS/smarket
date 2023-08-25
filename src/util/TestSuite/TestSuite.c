#include "TestSuite.h" // TestResult, boolean
#include <stdio.h> // snprintf, fprintf
#include <stdlib.h> // exit

/**
 * Encerra a execução do programa imadiatamente
*/
void exitOnError() {
  exit(1);
}

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

TestResult expectStringsToBeEquals(const char *expected, const char *received) {
  TestResult result;
  if (strcmp(expected, received) == 0) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected \"%s\" but received: \"%s\"\n", expected, received);
  result.pass = False;

  return result;
}

void it(char *testDesciption, TestResult(*test)()) {
  TestResult testResult;
  testResult = test();

  if (testResult.pass) {
    printf("\n✅ %s - pass", testDesciption);
  } else {
    fprintf(stderr, "\n❌ %s - failed", testDesciption);
    fprintf(stderr, "\n\t\t%s", testResult.errorDescription);
    exitOnError();
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

TestArgs parseTestArgs(int argc, char **argv) {
  _parseTestArgs(argc, argv, exitOnError);
}

void noTestFoundWithGiven(char *name) {
  fprintf(stderr, "❌ - No test found with the name of: %s", name);
  exitOnError();
}

TestResult expectToBeNotNull(const void *received) {

  TestResult result;
  if (received != NULL) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected pointer address but received: %p\n", received);
  result.pass = False;

  return result;
}

TestResult expectToBeNull(const void *value) {

  TestResult result;
  if (value == NULL) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected %p but received: %p\n", NULL, value);
  result.pass = False;

  return result;
}

TestResult expectToBeFalse(const boolean value) {

  TestResult result;
  if (value == False) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected \"False\" but received: True\n");
  result.pass = False;

  return result;
}

TestResult expectToBeTrue(const boolean value) {

  TestResult result;
  if (value == True) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected \"True\" but received: False\n");
  result.pass = False;

  return result;
}

TestResult expectPointersToBeEquals(const void *expected, const void *received) {
  TestResult result;
  if (expected == received) {
    result.errorDescription[0] = '\0';
    result.pass = True;
    return result;
  }

  snprintf(result.errorDescription, 100, "Expected %p but received: %p\n", expected, received);
  result.pass = False;

  return result;
}
