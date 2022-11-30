#include <stdlib.h>
#include <stdio.h>

#include "curl-util.h"
#include "aare-guru-api.h"

void list_cities() {
  const char *json = curl("https://aareguru.existenz.ch/v2018/cities?app=my.app.ch&version=1.0.42");
  printf("%s", json);
}
