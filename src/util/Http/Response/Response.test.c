#include "../../TestSuite/TestSuite.h"
#include "Response.h"

TestResult newResponseIsNotNull() {
  Response* res = newResponse();

  return expectToBeNotNull(res);
}

TestResult toStringResponse() {
  Response* res = newResponse();
  alocatedCString headers = res->toString(res);
  char* expectedString = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\n\r\n";

  return expectStringsToBeEquals(expectedString, headers);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("newResponseIsNotNull")
      it("Criar nova Response retorna uma referencia valida", newResponseIsNotNull);
      break;
    CASE ("toStringResponse")
      it("toString retorna as headers formatadas", toStringResponse);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}