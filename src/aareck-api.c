#include <stdlib.h>
#include "aare-guru-adapter.h"

UT_array * get_cities() {
  // invoke aare-guru-adapter
  return request_cities();
}

UT_array * get_hydrometric_data(RequestData *request) {
  // invoke aare-guru-adapter
  return request_hydrometric_data(request);
}

UT_array * get_mixed_data(RequestData *request) {
  // invoke aare-guru-adapter
  return request_mixed_data(request);
}

UT_array * get_weather_data(RequestData *request) {
  // invoke aare-guru-adapter
  // return request_weather_data(request);
}
