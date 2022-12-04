#ifndef SPLIT_H
#define SPLIT_H

#include <stdlib.h>
#include "utarray.h"

/*
splits a string into tokens by a given separator.

prevents disadvantages of standard c strtok (or strtok_r) function:
split does not overwrite the separators in the string being split with nul characters, 
so the original string is still usable afterwards. i.e., split is non-destructive!
split is also re-entrant as strtok_r.
*/
UT_array * split(const char *str, char sep) {
  UT_array *array;
  utarray_new(array, &ut_str_icd);
  unsigned int start = 0, stop = 0;

  do {
    if (str[stop] == sep) {
      char *token = calloc(stop+1-start, sizeof(char));
      memcpy(token, str+start, stop+1-start);
      token[stop] = '\0';
      utarray_push_back(array, &token);
      start = stop + 1;
    }
    stop++;
  } while (str[stop]);

  char *token = calloc(stop+1-start, sizeof(char));
  memcpy(token, str+start, stop+1-start);
  token[stop] = '\0';
  utarray_push_back(array, &token);
  return array;
}

#endif
