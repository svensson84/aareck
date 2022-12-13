#ifndef AARE_GURU_ADAPTER_H
#define AARE_GURU_ADAPTER_H

#include "aareck-interface.h"

extern const char* AARE_GURU_REST_API_URL;
extern const char* AARE_GURU_REST_API_URL_PARAMETERS;

UT_array * request_hydrometric_data(RequestData *request);
UT_array * request_mixed_data(RequestData *request);
UT_array * request_weather_data(RequestData *request);
UT_array * request_cities();
const char * map_weather_condition_code(int code);
const char * map_day_name(const char *code);


#endif /* AARE_GURU_ADAPTER_H */
