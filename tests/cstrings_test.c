#include "../src/util/Cstrings/Cstrings.h"


int main(){
    const char* str1 = "Sou uma string aa";
    const char* str2 = "Sou uma string aa";
    const int actual = isEquals(str1, str2);
    const int expected = 1;
    
    if (actual == expected) return 0;
    return 1;
}