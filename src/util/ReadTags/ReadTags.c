#include "ReadTags.h"
#include "../Cstrings/Cstrings.h"
#include <stdio.h> // printf, null
#include "../../libs/MercuryApi/ReadRFIDTags.h"

#define DEFAULT_TMR "tmr:///dev/ttyUSB0"
#define DEFAULT_ANTENA "1"

int mockedRead(int argc, char **argv,  Map * result) {
  if (result == 0) {
    printf("result é null\n");
    return 0;
  }
  for (int i = 0; i < 5; i++) {
    alocatedCString key = intToCString(i);
    result->setString(result, key, "rfidValue");
  }

  return 0;
}

Map* readTagsWithArgs(char *tmr, char *antena, Map * result) {
  char *args[4];
  args[0] = "readRFID";
  args[1] = tmr;
  args[2] = "--ant";
  args[3] = antena;

  readRFIDTags(4, args, result);
  return result;
}

Map* readTags() {
  Map *resultMap = newMap();

  return readTagsWithArgs(DEFAULT_TMR, DEFAULT_ANTENA, resultMap);
}
