#include "TestSuite.h" // TestResult, boolean
#include <stdio.h> // snprintf, fprintf

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