/* 
split.h -- defines a thread-safe and smarter way of splitting a string 
           into token than standard c function strtok (or strtok_r)
   
MIT License

Copyright (c) 2022 Sven Zaugg <zaugg84@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

#endif /* SPLIT_H */