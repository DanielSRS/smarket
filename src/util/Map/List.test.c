#include "../TestSuite/TestSuite.h"
#include "Map.h"

TestResult newListCreatesANonNullReference() {
  List *list = newList();

  return expectToBeNotNull(list);
}

TestResult _mapAttrShouldBeNotNull() {
  List *list = newList();

  return expectToBeNotNull(list->_map);
}

TestResult destroyShouldBeNotNull() {
  List *list = newList();

  return expectToBeNotNull(list->destroy);
}

TestResult lengthShouldBeNotNull() {
  List *list = newList();

  return expectToBeNotNull(list->length);
}

TestResult pushAnyShouldBeNotNull() {
  List *list = newList();

  return expectToBeNotNull(list->pushAny);
}

TestResult pushStringShouldBeNotNull() {
  List *list = newList();

  return expectToBeNotNull(list->pushString);
}

TestResult lengthShouldBeZero() {
  List *list = newList();
  int initialValueShouldBeZero = 0;
  int listLength = list->length(list);

  return assertEquals(initialValueShouldBeZero, listLength);
}

TestResult lengthShouldChangeAfterInsertion() {
  List *list = newList();
  list->pushString(list, "first value");
  int insertedValues = 1;
  int listLength = list->length(list);

  return assertEquals(insertedValues, listLength);
}

TestResult lengthIsTheSameAsPushResult() {
  List *list = newList();
  int listLength = list->pushString(list, "first value");
  int insertedValues = 1;

  return assertEquals(insertedValues, listLength);
}

int main(int argc, char **argv){
  TestArgs args = parseTestArgs(argc, argv);

  SWITCH (args.testName) {
    CASE ("newListCreatesANonNullReference")
      it("Retorna uma referência válida ao criar nova List", newListCreatesANonNullReference);
      break;
    CASE ("_mapAttrShouldBeNotNull")
      it("Atributo _map de uma nova Lista não pode ser NULL", _mapAttrShouldBeNotNull);
      break;
    CASE ("destroyShouldBeNotNull")
      it("Atributo destroy tem valor válido (não nulo)", destroyShouldBeNotNull);
      break;
    CASE ("lengthShouldBeNotNull")
      it("Atributo length tem valor válido (não nulo)", lengthShouldBeNotNull);
      break;
    CASE ("pushAnyShouldBeNotNull")
      it("Atributo pushAny tem valor válido (não nulo)", pushAnyShouldBeNotNull);
      break;
    CASE ("pushStringShouldBeNotNull")
      it("Atributo pushString tem valor válido (não nulo)", pushStringShouldBeNotNull);
      break;
    CASE ("lengthShouldBeZero")
      it("O retorno de length() deve ser 0 (zero) quando a lista é criada", lengthShouldBeZero);
      break;
    CASE ("lengthShouldChangeAfterInsertion")
      it("A quantidade de items deve ser 1 após uma inserção", lengthShouldChangeAfterInsertion);
      break;
    CASE ("lengthIsTheSameAsPushResult")
      it("Push deve retornar 1 se for a primeira inserção", lengthIsTheSameAsPushResult);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}