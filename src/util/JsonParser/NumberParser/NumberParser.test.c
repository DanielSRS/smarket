#include "../../TestSuite/TestSuite.h"
#include "NumberParser.h"

// 0
// -0
// -(1-9) seguido por indefinidos 0-9
// (1-9) seguido por indefinidos 0-9
// 0.(0-9)
// 0.(0-9)
// -0.12E3

TestResult zeroValue() {
  char *stringToBeParsed = "0 ";
  double expected = 0;
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectDoublesToBeEquals(expected, parsedNumber.value);
}

TestResult zeroErr() {
  char *stringToBeParsed = "0 ";
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectToBeFalse(parsedNumber.withErrors);
}

TestResult zeroNegativeIsZero() {
  char *stringToBeParsed = "-0 ";
  double expected = 0;
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectDoublesToBeEquals(expected, parsedNumber.value);
}

TestResult zeroNegativeIsValid() {
  char *stringToBeParsed = "-0 ";
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectToBeFalse(parsedNumber.withErrors);
}

TestResult negativeNumber() {
  char *stringToBeParsed = "-3425345 ";
  double expected = -3425345;
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectDoublesToBeEquals(expected, parsedNumber.value);
}

TestResult positiveNumber() {
  char *stringToBeParsed = "3425345 ";
  double expected = 3425345;
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectDoublesToBeEquals(expected, parsedNumber.value);
}

TestResult fractionNumber() {
  char *stringToBeParsed = "0.61865 ";
  double expected = 0.61865;
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectDoublesToBeEquals(expected, parsedNumber.value);
}

TestResult parseCorrectNumber() {
  char *stringToBeParsed = "-0.12E3 ";
  double expected = -120;
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectDoublesToBeEquals(expected, parsedNumber.value);
}

TestResult leadingZeroIsInvalid() {
  char *stringToBeParsed = "05 ";
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectToBeTrue(parsedNumber.withErrors);
}

TestResult leadingNegZeroIsInvalid() {
  char *stringToBeParsed = "-0183 ";
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectToBeTrue(parsedNumber.withErrors);
}

TestResult positiveSignIsInvalid() {
  char *stringToBeParsed = "+83 ";
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);

  return expectToBeTrue(parsedNumber.withErrors);
}

TestResult commaNotAllowed() {
  char *stringToBeParsed = "3,14 ";
  int offset = 0;
  int max = strlen(stringToBeParsed);
  ParsedNumber parsedNumber = parseNumber(&offset, max, stringToBeParsed);
  offset++; // passa a virgula
  ParsedNumber parsedNumber2 = parseNumber(&offset, max, stringToBeParsed);

  boolean a = parsedNumber.value == 3 && parsedNumber2.value == 14;
  boolean b = parsedNumber.withErrors == False && parsedNumber2.withErrors == False;

  return expectToBeTrue(a && b);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("zeroValue")
      it("Reconhece um zero", zeroValue);
      break;
    CASE ("zeroErr")
      it("Reconhece um zero sem erros", zeroErr);
      break;
    CASE ("zeroNegativeIsZero")
      it("-0 é igual a 0", zeroNegativeIsZero);
      break;
    CASE ("zeroNegativeIsValid")
      it("-0 é válido", zeroNegativeIsValid);
      break;
    CASE ("negativeNumber")
      it("Um numero negativo", negativeNumber);
      break;
    CASE ("positiveNumber")
      it("Um numero positivo", positiveNumber);
      break;
    CASE ("fractionNumber")
      it("Número de ponto flutuante", fractionNumber);
      break;
    CASE ("parseCorrectNumber")
      it("Número em notação cintifica", parseCorrectNumber);
      break;
    CASE ("leadingZeroIsInvalid")
      it("Zero a esquerda não é permitido", leadingZeroIsInvalid);
      break;
    CASE ("leadingNegZeroIsInvalid")
      it("Zero a esquerda negativo é invalido", leadingNegZeroIsInvalid);
      break;
    CASE ("positiveSignIsInvalid")
      it("Sinal positivo é invalido", positiveSignIsInvalid);
      break;
    CASE ("commaNotAllowed")
      it("Virgula para o reconhecimetno e pode causer erro", commaNotAllowed);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}