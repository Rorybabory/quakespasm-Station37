#include <stdio.h>

typedef struct {
  int val;
  int endI;
} numParseResult;

numParseResult parse_readNum(char *str, int i);
int parseDiceNotation(char *dice);