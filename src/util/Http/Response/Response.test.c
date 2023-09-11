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

TestResult toStringResponseWithBody() {
  Response* res = newResponse();
  res->withJSON(res);
  alocatedCString headers = res->toString(res);
  char* expectedString = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: application/json\r\nContent-Length: 3\r\n\r\n{ }";

  return expectStringsToBeEquals(expectedString, headers);
}

TestResult toStringResponseWithBodyFilled() {
  Response* res = newResponse();
  res
    ->withStatusCode(201, res)
    ->withStatusMessage("KO", res)
    ->withJSON(res)
    ->addStringToJson("keya", "valuea", res)
    ->addStringToJson("surfando", "na terra", res);

  alocatedCString headers = res->toString(res);
  char* expectedString = "HTTP/1.1 201 KO\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: application/json\r\nContent-Length: 39\r\n\r\n{\"keya\":\"valuea\",\"surfando\":\"na terra\"}";

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
    CASE ("toStringResponseWithBody")
      it("toString com json body", toStringResponseWithBody);
      break;
    CASE ("toStringResponseWithBodyFilled")
      it("toString com json body que tem elementos", toStringResponseWithBodyFilled);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}