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
#include <stdbool.h>
#include <sys/types.h>
#include <getopt.h>
#include <signal.h>

#include "aareck.h"
#include "split.h"
#include "utarray.h"
#include "aare-guru-api.h"

/* Global variables */
RequestData g_request_data;

static struct option const long_options[] = {
  {"city", required_argument, NULL, 'c'},
  {"color", optional_argument, NULL, 'C'},
  {"hydraulic-only", optional_argument, NULL, 'h'},
  {"interactive", no_argument, NULL, 'i'},
  {"list", no_argument, NULL, 'l'},
  {"send", required_argument, NULL, 's'},
  {"verbose", no_argument, NULL, 'v'},
  {"help", no_argument, NULL, 'H'},
  {"version", no_argument, NULL, 'V'}
};

void show_usage() {
  printf(
    "Usage: %s [OPTION]...\n\n", PROGRAM_NAME);
  fputs(
    "-c, --city=NAME...       restrict output to city NAME...\n"
    "-C, --color[=WHEN]       colorize the output; WHEN can be 'always' (default\n"
    "                         if omitted), 'auto', or 'never'; more info below\n"
    "-h, --hydraulic-only     reports only hydraulic data.\n"
    "-i, --interactive        prompt whether to remove destinations\n"
    "-l, --list               list available cities.\n"
    "-s, --send=MAIL_ADDRESS  send an e-mail to MAIL_ADDRESS instead of output to stdin\n"
    "-v, --verbose            verbosely output\n\n"
    "    --help               display this help and exit\n"
    "-V, --version            output version information and exit\n", stdout);
}

void show_version() {
  printf(
    "aarechk %s - <https://github.com/svensson84/aareck>\n\n", PROGRAM_VERSION);

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

void test() {
  char **p = NULL;
  while ( (p=(char**)utarray_next(g_request_data.cities,p))) {
    printf("%s\n",*p);
  }
  printf("%d\n", g_request_data.flags);
}

void free_memory() {
  if (g_request_data.cities != NULL)
    utarray_free(g_request_data.cities);
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

void init_default_request() {
  UT_array *cities;
  utarray_new(cities, &ut_str_icd);
  char *city = "Bern";
  utarray_push_back(cities, &city);
  g_request_data.cities = cities;
  g_request_data.flags = FLAG_DEFAULT_MASK;
}

int main (int argc, char **argv) {
  register_handlers();
  init_default_request();
  int c;

  while ((c = getopt_long(argc, argv, "c:C::hHils:vhV", long_options, NULL)) != -1) {
    switch (c) {
      case 'c':
        char *cities = optarg;
        g_request_data.cities = split(cities, ' ');
        break;
      case 'C':
        g_request_data.flags |= FLAG_COLOR;
        break;
      case 'h':
        g_request_data.flags |= FLAG_HYDRAULIC_ONLY;
        break;
      case 'i':
        break;
      case 'l':
        list_cities();
        break;
      case 's':
        break;
      case 'v':
        g_request_data.flags |= FLAG_VERBOSE;
        break;
      case 'H':
        show_usage();
        exit(EXIT_SUCCESS);
      case 'V':
	show_version();
        break;
      case -1:
        break;
      default:
        show_usage();
        exit(EXIT_FAILURE);
    }
  }

  // do rest api call
  exit(EXIT_SUCCESS);
}
