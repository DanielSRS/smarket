#include "switchString.h"
#include "../Boolean/Boolean.h" // boolean

#ifndef TESTSUIT_H
#define TESTSUIT_H

/**
 * Resultado da execução de um teste (asserção)
 * 
 * Indica se um teste passou e do contrário, a descrição
 * do erro ocorrido
*/
typedef struct TestResult
{
  /**
   * Indica se um o teste passou.
   * 
   * False indica que houve um erro no teste
  */
  boolean pass;
  /**
   * Descrição do erro quando o teste não passa.
   * 
   * Deve ser definido quando pass é False. do contrário
   * o valor definido deve ser "" (string vazia)
  */
  char errorDescription[1000];
} TestResult;

typedef struct TestArgs
{
  /** Nome do arquivo que contém os testes */
  char *testFileName;
  /** Nome do teste a ser executado */
  char *testName;
} TestArgs;


TestResult assertEquals(int expected, int received);

TestResult expectPointersToBeEquals(const void *expected, const void *received);

TestResult expectToBeNotNull(const void *received);

TestResult expectToBeNull(const void *value);

TestResult expectToBeFalse(const boolean value);

TestResult expectToBeTrue(const boolean value);

TestResult expectStringsToBeEquals(const char *expected, const char *received);

// assertNull
// assertTrue
// assertFalse

/**
 * Executa um teste e verifica se houve houve um erro..
 * 
 * Caso o teste passe, é enviado para a saída padrão:
 *    ✅ - <descrição do teste> - pass
 * 
 * Em caso de erro, é enviado para a saída de erro:
 *    ❌ - <descrição do teste - failed
 *        <descrição do erro>
*/
void it(char *testDesciption, TestResult(*test)());

/**
 * Valida os argumentos de chamada de um conjunto de testes
 * 
 * Aborta se os argumentos estiverem em um formato incorreto
 */
TestArgs parseTestArgs(int argc, char **argv);

/**
 * Aborta caso não exista um teste com o nome especificado
*/
void noTestFoundWithGiven(char *name);

#endif // TESTSUIT_H