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

TestResult lengthShouldChangeAfterMultInsertion() {
  List *list = newList();
  list->pushString(list, "first value");
  list->pushString(list, "first value2");
  list->pushString(list, "first value3");
  list->pushString(list, "first value4");
  list->pushString(list, "first value5");
  list->pushString(list, "first value6");
  list->pushString(list, "first value7");
  list->pushString(list, "first value8");
  list->pushString(list, "first value9");
  list->pushString(list, "first value10");

  int insertedValues = 10;
  int listLength = list->length(list);

  return assertEquals(insertedValues, listLength);
}

TestResult pushReturnTheNewLengthAfterMultInsertion() {
  List *list = newList();
  list->pushString(list, "first value");
  list->pushString(list, "first value2");
  list->pushString(list, "first value3");
  list->pushString(list, "first value4");
  list->pushString(list, "first value5");
  list->pushString(list, "first value7");
  list->pushString(list, "first value8");
  list->pushString(list, "first value9");
  int listLength = list->pushString(list, "first value10");

  int insertedValues = 9;

  return assertEquals(insertedValues, listLength);
}

TestResult destroyAList() {
  List *list = newList();

  List *listRef = list;
  Map *mapRef = (Map*) list->_map;

  list->destroy(&list);

  boolean listIsNullAfterDestroy = list == NULL;
  boolean mapIsNull = listRef->_map == NULL;
  boolean lengthIsNull = listRef->length == NULL;
  boolean destroyIsNull = listRef->destroy == NULL;
  boolean pushAnyIsNull = listRef->pushAny == NULL;
  boolean pushStringIsNull = listRef->pushString == NULL;
  boolean mapItemsIsNull = mapRef->_items == NULL;

  boolean assertAll = listIsNullAfterDestroy
                      && mapIsNull
                      && lengthIsNull
                      && pushAnyIsNull
                      && pushStringIsNull
                      && mapItemsIsNull
                      && destroyIsNull;

  return expectToBeTrue(assertAll);
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
    CASE ("lengthShouldChangeAfterMultInsertion")
      it("A quantidade de items deve ser 10 após dez inserções", lengthShouldChangeAfterMultInsertion);
      break;
    CASE ("pushReturnTheNewLengthAfterMultInsertion")
      it("Push deve retornar o tamanho após multiplas inserções", pushReturnTheNewLengthAfterMultInsertion);
      break;
    CASE ("destroyAList")
      it("Destrói a lista", destroyAList);
      break;
    DEFAULT
      noTestFoundWithGiven(args.testName);
  } END;
}