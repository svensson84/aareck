/* 
aareck.h -- types used by aareck
   
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

#ifndef AARECK_H
#define AARECK_H

#include <stdlib.h>
#include "utarray.h"

/* The official name of this program */
#define PROGRAM_NAME "aareck"

/* The official version of this program */
#define PROGRAM_VERSION "0.1.0"
 
/* Bit masks for flag options */
#define FLAG_VERBOSE         0b000000001;
#define FLAG_COLOR           0b000000010;
#define FLAG_HYDRAULIC_ONLY  0b000000100;
#define FLAG_DEFAULT_MASK    0b000000000;

typedef u_int bitmask; 

typedef struct {
  UT_array *cities;
  bitmask flags;
} RequestData;

#endif /* AARECK_H */
