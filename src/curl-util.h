#ifndef CURL_UTIL_H
#define CURL_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemoryStruct {
  char *data;
  size_t size;
} memory;

size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp);

const char* curl(char url[]);

#endif
