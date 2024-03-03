#include "../TestSuite/TestSuite.h"
#include "JsonParser.h"
#include <stdio.h>

TestResult parseCorrectString() {
  char *stringToBeParsed = "\"string value\"";
  int offset = 1;
  int max = 14;
  printf("\nNão tem problema antes do parse\n");
  alocatedCString parsedString = parseString(&offset, max, stringToBeParsed);

  return expectStringsToBeEquals("string value", parsedString);
}

TestResult parseStringValueType() {
  char *stringToBeParsed = "    \"string value\"    ,";
  int offset = 1;
  int max = 23;
  printf("\nNão tem problema antes do parse\n");
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return assertEquals(String, parsed.type);
}

TestResult parseStringValue() {
  char *stringToBeParsed = "    \"string value\"    ,";
  int offset = 0;
  int max = 23;
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return expectStringsToBeEquals("string value", parsed.stringVAlue);
}

TestResult parseBooleanValue() {
  char *stringToBeParsed = "  true    ,";
  int offset = 0;
  int max = 23;
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return expectToBeTrue(parsed.boolValue);
}

TestResult parseBooleanValueType() {
  char *stringToBeParsed = "true]";
  int offset = 0;
  int max = 23;
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return assertEquals(Boolean, parsed.type);
}

TestResult parseBooleanFalseValue() {
  char *stringToBeParsed = "  false  }";
  int offset = 0;
  int max = 10;
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return expectToBeFalse(parsed.boolValue);
}

TestResult parseBooleanValueFalseType() {
  char *stringToBeParsed = "false, lkasçjdf]";
  int offset = 0;
  int max = 16;
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return assertEquals(Boolean, parsed.type);
}

TestResult parseNullType() {
  char *stringToBeParsed = "  null  ]";
  int offset = 0;
  int max = 6;
  ParsedValue parsed = parseValue(&offset, max, stringToBeParsed);

  return assertEquals(null, parsed.type);
}

TestResult parsedObjReturnsNotNull() {
  char *stringToBeParsed = "{ \"Dan\": \"Niel\" }";
  int offset = 1;
  int max = 17;
  Map* map = NULL;
  map = parseObject(&offset, max, stringToBeParsed);

  return expectToBeNotNull(map);
}

TestResult parsedObjCreateCorrectMap() {
  char *stringToBeParsed = "{ \"Dan\": \"Niel\" }";
  int offset = 1;
  int max = 17;
  Map* map = NULL;
  map = parseObject(&offset, max, stringToBeParsed);

  boolean hasTheKey = map->has(map, "Dan");
  boolean correctValue = isEquals(map->get(map, "Dan"), "Niel");
  boolean correctSize = map->length == 1;

  return expectToBeTrue(hasTheKey && correctValue && correctSize);
}

TestResult parsedObjectValue() {
  char *stringToBeParsed = "{ \"Santa\": { \"Ro\": \"Sa\", \"San\": \"Tos\" } }";
  int offset = 1;
  int max = 41;
  Map* map = NULL;
  map = parseObject(&offset, max, stringToBeParsed);

  boolean hasTheKey1 = map->has(map, "Santa");
  Map* nested = map->get(map, "Santa");
  boolean externSize = map->length == 1;
  boolean innerSize = nested->length == 2;
  boolean hasTheKey2 = nested->has(nested, "Ro");
  boolean hasTheKey3 = nested->has(nested, "San");

  boolean correctInner1 = isEquals("Sa", nested->get(nested, "Ro"));
  boolean correctInner2 = isEquals("Tos", nested->get(nested, "San"));

  boolean value = hasTheKey1
               && hasTheKey2
               && hasTheKey3
               && externSize
               && innerSize
               && correctInner1
               && correctInner2;

  return expectToBeTrue(value);
}

TestResult parsedObjWithNumber() {
  char *stringToBeParsed = "{ \"Dan\": -0.12E4 }";
  int offset = 1;
  int max = cStringLenght(stringToBeParsed);
  Map* map = NULL;
  map = parseObject(&offset, max, stringToBeParsed);

  boolean hasTheKey = map->has(map, "Dan");
  boolean correctValue = *((double*) map->get(map, "Dan")) == -1200;
  boolean correctSize = map->length == 1;

  return expectToBeTrue(hasTheKey && correctValue && correctSize);
}


int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("parseCorrectString")
      it("Reconhece uma string", parseCorrectString);
      break;
    CASE ("parseStringValueType")
      it("Reconhece um valor como string", parseStringValueType);
      break;
    CASE ("parseStringValue")
      it("Reconhece um valor string", parseStringValue);
      break;
    CASE ("parseBooleanValue")
      it("Reconhece um valor boleano", parseBooleanValue);
      break;
    CASE ("parseBooleanValueType")
      it("Reconhece um tipo boleano", parseBooleanValueType);
      break;
    CASE ("parseBooleanFalseValue")
      it("Reconhece um boleano false", parseBooleanFalseValue);
      break;
    CASE ("parseBooleanValueFalseType")
      it("Reconhece um tipo boleano false", parseBooleanValueFalseType);
      break;
    CASE ("parseNullType")
      it("Reconhece um tipo nulo", parseNullType);
      break;
    CASE ("parsedObjReturnsNotNull")
      it("Se um objeto é reconhecido, retorna um referencia não nula", parsedObjReturnsNotNull);
      break;
    CASE ("parsedObjCreateCorrectMap")
      it("Cria um Map com os valores  do Oject", parsedObjCreateCorrectMap);
      break;
    CASE ("parsedObjectValue")
      it("Reconhece objeto onde um valor é outro objeto", parsedObjectValue);
      break;
    CASE ("parsedObjWithNumber")
      it("Reconhece objeto onde um valor é outro objeto contendo um numero", parsedObjWithNumber);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}