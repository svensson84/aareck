/*
aareck-api.h -- public api for aareck which determines semantic versioning,
                i.e. by changing these interfaces, a major release will
                be published!

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

#ifndef AARECK_API_H
#define AARECK_API_H

#include <stdlib.h>
#include "utarray.h"

/* Bit masks for flag options */
#define FLAG_HYDROMETRIC_DATA 0b000000001
#define FLAG_MIXED_DATA       0b000000010
#define FLAG_WEATHER_DATA     0b000000100
#define FLAG_AUTO_UPDATE      0b000001000
#define FLAG_COLORIZED        0b000010000
#define FLAG_DEFAULT_MASK     0b000000000

typedef u_int bitmask;

typedef struct {
  UT_array *cities;
  bitmask flags;
} RequestData;

typedef struct {
  const char *city;
  const char *temperature_water;
  const char *temperature_water_forecast2h;
  const char *temperature_air;
  const char *flow;
} HydrometricData;

typedef struct {
  const char *city;
  const char *temperature_water;
  const char *temperature_air_afternoon;
  const char *temperature_air_evening;
  const char *weather_condition;
} MixedData;

void list_hydrometric_data(RequestData *request);
void list_mixed_data(RequestData *request);
void list_weather_data(RequestData *request);

#endif /* AARECK_API_H */
