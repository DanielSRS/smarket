#include "../../Boolean/Boolean.h"      // boolean, True, False

#ifndef NUMBER_PARSER_H
#define NUMBER_PARSER_H

typedef struct ParsedNumber
{
  double value;
  boolean withErrors;
} ParsedNumber;

ParsedNumber parseNumber(int *offset, int max, const char *buffer);

#endif