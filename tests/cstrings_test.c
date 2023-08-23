#include "../src/util/Cstrings/Cstrings.h"
#include <stdio.h> // snprintf
#include "../src/util/TestSuite/TestSuite.h"

TestResult _compareIfStringsAreEqual() {
    const char* str1 = "Sou uma string aa";
    const char* str2 = "Sou uma string aa";
    const int result = isEquals(str1, str2);

    return assertEquals(True, result);
}

TestResult _compareIfStringsAreDiff() {
    const char* str1 = "Sou uma string aa";
    const char* str2 = "Sou uma string bb";
    const int result = isEquals(str1, str2);

    return assertEquals(False, result);
}

int main(int argc, char **argv){
    char *fileName = argv[0];
    char *testName = argv[1];

    /** Se não informado o nome do teste,falha*/
    if (argc <= 1) {
        fprintf(stderr, "❌ - Test name not provided");
        return 1;
    }

    /** Argumentos demais, não sei lidar */
    if (argc > 2) {
        fprintf(stderr, "❌ - Too many arguments, expected 1, but received: %i", argc - 1);
        return 1;
    }

    SWITCH (testName) {
        CASE ("_compareIfStringsAreEqual")
            return it(
                "Compara duas string e retorna true se forem iguais",
                _compareIfStringsAreEqual);
        CASE ("_compareIfStringsAreDiff")
            return it(
                "Compara duas string e retorna false se forem diferentes",
                _compareIfStringsAreDiff);
        DEFAULT
            fprintf(stderr, "❌ - No test found with the name of: %s", testName);
            return 1;
    } END;
}