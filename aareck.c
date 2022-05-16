/* 
aareck -- aare check; hydraulic and weather data on switzerland's most beautiful river "Aare"
   
MIT License

Copyright (c) 2022 Sven Zaugg - zaugg84[at]gmail.com

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
#include <sys/types.h>
#include <getopt.h>
#include <stdbool.h>  

/* The official name of this program */
#define PROGRAM_NAME "aareck"

static struct option const long_options[] = {
  {"city", required_argument, NULL, 'c'},
  {"color", optional_argument, NULL, 'C'},
  {"interactive", no_argument, NULL, 'i'},
  {"send", required_argument, NULL, 's'},
  {"verbose", no_argument, NULL, 'v'},
  {"help", no_argument, NULL, 'h'},
  {"version", no_argument, NULL, 'V'}
};

void usage () {
  printf(
    "Usage: %s [OPTION]...\n\n", PROGRAM_NAME);
  
  fputs(
    "-c, --city=NAME...       restrict output to city NAME...\n"
    "-C, --color[=WHEN]       colorize the output; WHEN can be 'always' (default\n"
    "                         if omitted), 'auto', or 'never'; more info below\n"
    "-i, --interactive        prompt whether to remove destinations\n"
    "-s, --send=MAIL_ADDRESS  send an e-mail to MAIL_ADDRESS instead of output to stdin\n"
    "-v, --verbose            verbosely output\n\n"
    "-h, --help               display this help and exit\n"
    "-V, --version            output version information and exit\n", stdout);
}

int main (int argc, char **argv) {
  while (true) {
    int c = getopt_long(argc, argv, "c:C::is:vhV", long_options, NULL);

    switch (c) {
      case -1:
        exit(EXIT_SUCCESS);
      case 'c':
        break;
      case 'C':
        break;
      case 'i':
        break;
      case 's':
        break;
      case 'v':
        break;
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
      case 'V':
        break;
      default:
        usage();
        exit(EXIT_FAILURE);
    }
  }
}