#include <stdlib.h>

#include "helpers.h"

void trim_trailing(char *str) {
  int index, i;
  index = -1;

  i = 0;
  while(str[i] != '\0') {
    if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
      index= i;
    }

    i++;
  }

  str[index + 1] = '\0';
}
