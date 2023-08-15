#include "dice.h"
numParseResult parse_readNum(char *str, int i) {
  int val = 0;
  int isDice = 0;
  while (1 == 1) {
    if (i >= strlen(str)) {
      break;
    }
    if (str[i] < '0' || str[i] > '9') {
      if (str[i] == 'd') {
        isDice = 1;
        i++;
        continue;
      } else {
        break;
      }
    }
    val *= 10;
    val += (int)(str[i] - '0');
    i++;
  }
  numParseResult result;
  result.val = (rand() % val) + 1;
  result.endI = i;
  return result;
}
int parseDiceNotation(char *dice) {
  int currentNum = 0;
  int total = 0;
  int state = 0;
  srand(time(NULL));
  for (int i = 0; i < strlen(dice); i++) {
    numParseResult numresult = parse_readNum(dice, i);
    currentNum = numresult.val;
    i = numresult.endI;
    switch (state) {
    case 0:
      total += currentNum;
      break;
    case 1:
      total -= currentNum;
      break;
    case 2:
      total *= currentNum;
      break;
    case 3:
      total /= currentNum;

    default:
      break;
    }
    switch (dice[i]) {
    case '+':
      state = 0;
      break;
    case '-':
      state = 1;
      break;
    case '*':
      state = 2;
      break;
    case '/':
      state = 3;
      break;
    case 'd':
      state = 4;
    default:
      break;
    }
  }
  return total;
}