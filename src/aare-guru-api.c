#include <stdlib.h>
#include <stdio.h>

#include <json-c/json.h>

#include "aareck.h"
#include "aare-guru-api.h"
#include "curl-util.h"
#include "helpers.h"

const char* AARE_GURU_REST_API_URL = "https://aareguru.existenz.ch/v2018/";
const char* AARE_GURU_REST_API_URL_PARAMETERS = "app=aarechk&version=1.0.0";

void print_json_value(json_object *json_parent, char *key, char *caption, char *measure_unit) {
  json_object *json_child;
  int success = json_object_object_get_ex(json_parent, key, &json_child);
  if (!success) {
    fprintf(stderr, "TODO: error handling");
    exit(1);
  }
  printf("%s: %s %s\n", caption, json_object_get_string(json_child), measure_unit);
}

void list_cities() {
  char url[255] = "";
  strcat(url, AARE_GURU_REST_API_URL);
  strcat(url, "cities?");
  strcat(url, AARE_GURU_REST_API_URL_PARAMETERS);

  const char *json_string = curl(url);
  json_object *json = json_tokener_parse(json_string);
  //printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

  int number_of_cities = json_object_array_length(json);
  int i;

  printf("measurement data is available for the following cities:\n");

  for (i=0; i<number_of_cities; i++){
    json_object *json_city = json_object_array_get_idx(json, i);

    json_object *json_city_name;
    int success = json_object_object_get_ex(json_city, "city", &json_city_name);
    if (!success) {
      fprintf(stderr, "TODO: error handling");
      exit(1);
    }

    printf("(%d) %s\n", i+1, json_object_get_string(json_city_name));
  }
}

void list_measure_data(RequestData *request) {
  char **city;
  while ((city=(char**)utarray_next(request->cities,city))) {
    trim_trailing(*city);

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

    printf("measuring station: %s\n", *city);
    print_json_value(json_aare, "temperature", "temperature", "C°");
    print_json_value(json_aare, "forecast2h", "forecast in 2h", "C°");
    print_json_value(json_aare, "flow", "water quantity", "m³/s");

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

    print_json_value(json_weather_current, "tt", "air temperature", "C°");
    printf("\n");
  }
}
