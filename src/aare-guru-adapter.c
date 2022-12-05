#include <stdlib.h>
#include <stdio.h>

#include <json-c/json.h>

#include "aareck.h"
#include "aare-guru-adapter.h"
#include "curl-util.h"
#include "helpers.h"

const char* AARE_GURU_REST_API_URL = "https://aareguru.existenz.ch/v2018/";
const char* AARE_GURU_REST_API_URL_PARAMETERS = "app=aarechk&version=1.0.0";

const char * get_json_value(json_object *json_parent, char *key) {
  json_object *json_child;
  int success = json_object_object_get_ex(json_parent, key, &json_child);
  if (!success) {
    fprintf(stderr, "TODO: error handling");
    exit(1);
  }
  return json_object_get_string(json_child);
}

UT_array * get_measurement_data(RequestData *request) {
  UT_icd measurement_data_icd = {sizeof(MeasurementData), NULL, NULL, NULL};
  UT_array *measurements;
  MeasurementData measurement_data;
  utarray_new(measurements,&measurement_data_icd);
  char **city;

  while ((city=(char**)utarray_next(request->cities,city))) {
    trim_trailing(*city);
    measurement_data.city = *city;

    char url[255] = "";
    strcat(url, AARE_GURU_REST_API_URL);
    strcat(url, "current?city=");
    strcat(url, *city);
    strcat(url, "&");
    strcat(url, AARE_GURU_REST_API_URL_PARAMETERS);

    const char *json_string = curl(url);
    json_object *json = json_tokener_parse(json_string);
    //printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

    json_object *json_aare;
    int success = json_object_object_get_ex(json, "aare", &json_aare);
    if (!success) {
      fprintf(stderr, "TODO: error handling");
      exit(1);
    }

    measurement_data.temperature_water = get_json_value(json_aare, "temperature");
    measurement_data.temperature_water_forecast2h = get_json_value(json_aare, "forecast2h");
    measurement_data.flow = get_json_value(json_aare, "flow");

    json_object *json_weather;
    success = json_object_object_get_ex(json, "weather", &json_weather);
    if (!success) {
      fprintf(stderr, "TODO: error handling");
      exit(1);
    }

    json_object *json_weather_current;
    success = json_object_object_get_ex(json_weather, "current", &json_weather_current);
    if (!success) {
      fprintf(stderr, "TODO: error handling");
      exit(1);
    }

    measurement_data.temperature_air = get_json_value(json_weather_current, "tt");
    utarray_push_back(measurements, &measurement_data);
  }
  return measurements;
}

UT_array * get_cities() {
  UT_array *cities;
  utarray_new(cities, &ut_str_icd);

  char url[255] = "";
  strcat(url, AARE_GURU_REST_API_URL);
  strcat(url, "cities?");
  strcat(url, AARE_GURU_REST_API_URL_PARAMETERS);

  const char *json_string = curl(url);
  json_object *json = json_tokener_parse(json_string);
  //printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

  int number_of_cities = json_object_array_length(json);
  int i;

  for (i=0; i<number_of_cities; i++) {
    json_object *json_city = json_object_array_get_idx(json, i);

    json_object *json_city_name;
    int success = json_object_object_get_ex(json_city, "city", &json_city_name);
    if (!success) {
      fprintf(stderr, "TODO: error handling");
      exit(1);
    }

    const char *city = json_object_get_string(json_city_name);
    utarray_push_back(cities, &city);
  }
  return cities;
}

UT_array * get_report() {
  UT_array *cities = get_cities();
  printf("size of: %d\n", utarray_len(cities));
  return cities;
}
