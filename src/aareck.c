/*
aareck -- aare check; Reports hydraulic and weather data of the most beautiful
river in Switzerland, "the Aare".

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <getopt.h>
#include <signal.h>

#include "aareck.h"
#include "aareck-interface.h"
#include "split.h"
#include "utarray.h"
#include "aare-guru-adapter.h"

static struct option const long_options[] = {
  {"city", required_argument, NULL, 'c'},
  {"hydrometic", no_argument, NULL, 'h'},
  {"mixed", no_argument, NULL, 'm'},
  {"weather", no_argument, NULL, 'w'},
  {"list", no_argument, NULL, 'l'},
  {"status", no_argument, NULL, 's'},
  {"auto-update", no_argument, NULL, 'a'},
  {"colorized", no_argument, NULL, 'C'},
  {"help", no_argument, NULL, 'H'},
  {"version", no_argument, NULL, 'v'}
};

void show_usage() {
  printf(
    "Usage: %s [OPTION]...\n"
    "aare check - Reports various measurement data of the swiss river called 'aare'.\n\n", PROGRAM_NAME);
  fputs(
    "Mandatory arguments to long options are mandatory for short options too.\n"
    "-c, --city=NAME...       restrict output to city NAME. multiple cities are\n"
    "                         allowed by spaces, e.g. --city='foo bar baz'\n"
    "                         --city='all' includes cities of all measuring stations\n"
    "-h, --hydrometric        reports mainly hydrometric data\n"
    "-m, --mixed              reports mixed measurements\n"
    "-w, --weather            reports current weather data and forecasts\n"
    "-l, --list               lists cities of all measuring stations\n"
    "-s, --status             check aareguru's rest api connection\n"
    "-a  --auto-update        update measurement data in a specific time interval\n"
    "-C, --colorized          colorize the output\n\n"
    "-H, --help               display this help and exit\n"
    "-v, --version            output version information and exit\n\n"
    "If the option --city is not set, the measuring station 'Bern' is queried.\n\n"
    "The options -h [1], -m [2] and -w [3] are mutually exclusive. If more than one\n"
    "is used at the same time, the precedence in the square brackets above applies.\n", stdout);
}

void show_version() {
  printf(
    "%s %s - <https://github.com/svensson84/aareck>\n\n", PROGRAM_NAME, PROGRAM_VERSION);

  fputs(
    "MIT license\n"
    "Copyright (c) 2022 Sven Zaugg - zaugg84[at]gmail.com\n"
    "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
    "of this software and associated documentation files (the \"Software\"), to deal\n"
    "in the Software without restriction, including without limitation the rights\n"
    "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
    "copies of the Software, and to permit persons to whom the Software is\n"
    "furnished to do so, subject to the following conditions:\n"
    "The above copyright notice and this permission notice shall be included in all\n"
    "copies or substantial portions of the Software.\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
    "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
    "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
    "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
    "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
    "SOFTWARE.\n\n"
    "Written by Sven Zaugg.\n", stdout);
}

void free_memory() {
  // NOOP
}

void clean_up() {
  free_memory();
}

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
  else if (signo == SIGTERM)
    printf("received SIGTERM\n");

  printf("freeing memory before exiting...\n");
  free_memory();
}

void register_handlers() {
  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");

  int status;
  status = atexit(clean_up);
  if (status != 0) {
      printf("function clean_up for int 'atexit(void (*func)(void))' could nod be registered.\n");
      exit(EXIT_FAILURE);
  }
}

void init_default_request(RequestData *request) {
  UT_array *cities;
  utarray_new(cities, &ut_str_icd);
  char *city = "Bern";
  utarray_push_back(cities, &city);
  request->cities = cities;
  request->flags = FLAG_DEFAULT_MASK;
}

void show_cities() {
  UT_array *cities = get_cities();
  char **city;
  int counter=0;

  while ((city=(char**)utarray_next(cities ,city))) {
    printf("(%d) %s\n", ++counter, *city);
  }

  utarray_free(cities);
}

void show_hydrometric_data(RequestData *request) {
  UT_array *measurements = get_hydrometric_data(request);
  HydrometricData *hydrometric_data;
  int counter=0;

  for(hydrometric_data=(HydrometricData*)utarray_front(measurements); hydrometric_data!=NULL;
      hydrometric_data=(HydrometricData*)utarray_next(measurements,hydrometric_data)) {
    counter++;
    printf("measuring station:       %s\n", hydrometric_data->city);
    printf("water temperature:       %s °C\n", hydrometric_data->temperature_water);
    printf("water temperature in 2h: %s °C\n", hydrometric_data->temperature_water_forecast2h);
    printf("water quantity:          %s m³/s\n", hydrometric_data->flow);
    if (utarray_len(measurements) > counter)
      printf("\n");
  }

  utarray_free(measurements);
}

void show_mixed_data(RequestData *request) {
  UT_array *measurements = get_mixed_data(request);
  MixedData *mixed_data;
  int counter=0;

  for(mixed_data=(MixedData*)utarray_front(measurements); mixed_data!=NULL;
      mixed_data=(MixedData*)utarray_next(measurements,mixed_data)) {
    counter++;
    printf("measuring station:         %s\n", mixed_data->city);
    printf("water temperature:         %s °C\n", mixed_data->temperature_water);
    printf("air temperature current    %s °C\n", mixed_data->temperature_air);
    printf("air temperature afternoon: %s °C\n", mixed_data->temperature_air_afternoon);
    printf("air temperature evening:   %s °C\n", mixed_data->temperature_air_evening);
    printf("weather condition:         %s\n", mixed_data->weather_condition);
    if (utarray_len(measurements) > counter)
      printf("\n");
  }

  utarray_free(measurements);
}

void show_weather_data(RequestData *request) {
  UT_array *measurements = get_weather_data(request);
  WeatherData *weather_data;
  int counter=0;

  for(weather_data=(WeatherData*)utarray_front(measurements); weather_data!=NULL;
      weather_data=(WeatherData*)utarray_next(measurements,weather_data)) {
    counter++;
    printf("measuring station: %s\n", weather_data->city);
    printf("  today\n");
    printf("    morning    %s °C   %s mm   %s\n",
      weather_data->today.temperature_air_morning,
      weather_data->today.rainfall_morning,
      weather_data->today.weather_condition_morning);
    printf("    afternoon  %s °C   %s mm   %s\n",
      weather_data->today.temperature_air_afternoon,
      weather_data->today.rainfall_afternoon,
      weather_data->today.weather_condition_afternoon);
    printf("    evening    %s °C   %s mm   %s\n",
      weather_data->today.temperature_air_evening,
      weather_data->today.rainfall_evening,
      weather_data->today.weather_condition_evening);

    UT_array *forecasts = weather_data->forecasts;
    WeatherForecastData *forecast_data;

    for(forecast_data=(WeatherForecastData*)utarray_front(forecasts); forecast_data!=NULL;
        forecast_data=(WeatherForecastData*)utarray_next(forecasts,forecast_data)) {
      printf("  %s\n", forecast_data->day);
      printf("    %s °C  %s °C   %s mm   %s %%   %s\n",
        forecast_data->temperature_air_afternoon,
        forecast_data->temperature_air_evening,
        forecast_data->rainfall,
        forecast_data->rainfall_probability,
        forecast_data->weather_condition);
    }

    utarray_free(forecasts);

    if (utarray_len(measurements) > counter)
      printf("\n");
  }

  utarray_free(measurements);
}

void show_measurement_data(RequestData *request) {
  UT_array *cities;
  char** city = (char**)utarray_front(request->cities);
  if (strcmp(*city, "all") == 0) {
    request->cities = get_cities();
  }

  if (request->flags & FLAG_HYDROMETRIC_DATA) {
    show_hydrometric_data(request);
  } else if (request->flags & FLAG_MIXED_DATA) {
    show_mixed_data(request);
  } else if (request->flags & FLAG_WEATHER_DATA) {
    show_weather_data(request);
  } else {
    // default
    show_hydrometric_data(request);
  }
}

int main (int argc, char **argv) {
  register_handlers();

  RequestData *request;
  request = (RequestData*)malloc(sizeof(RequestData));
  init_default_request(request);

  int c;

  while ((c = getopt_long(argc, argv, "c:hmwlsaCHv", long_options, NULL)) != -1) {
    switch (c) {
      case 'c':
        char *cities = optarg;
        request->cities = split(cities, ' ');
        break;
      case 'h':
        request->flags |= FLAG_HYDROMETRIC_DATA;
        break;
      case 'm':
        request->flags |= FLAG_MIXED_DATA;
        break;
      case 'w':
        request->flags |= FLAG_WEATHER_DATA;
        break;
      case 'l':
        show_cities();
        exit(EXIT_SUCCESS);
      case 's':
        //check_state();
        exit(EXIT_SUCCESS);
      case 'a':
        request->flags |= FLAG_AUTO_UPDATE;
        break;
      case 'C':
        request->flags |= FLAG_COLORIZED;
        break;
      case 'H':
        show_usage();
        exit(EXIT_SUCCESS);
      case 'v':
	show_version();
        exit(EXIT_SUCCESS);
      default: 
        show_usage();
        exit(EXIT_FAILURE);
    }
  }

  show_measurement_data(request);
  free(request);
  exit(EXIT_SUCCESS);
}
