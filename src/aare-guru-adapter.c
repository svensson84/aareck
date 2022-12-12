#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#include "aareck.h"
#include "aare-guru-adapter.h"
#include "curl-util.h"
#include "helpers.h"

const char* AARE_GURU_REST_API_URL = "https://aareguru.existenz.ch/v2018/";
const char* AARE_GURU_REST_API_URL_PARAMETERS = "app=aarechk&version=1.0.0";

const char * map_weather_condition_code(int weather_condition_code) {
  switch(weather_condition_code) {
    case 1:
      return "sunny";
    case 2:
      return "mostly sunny";
    case 3:
      return "cloudy";
    case 4:
      return "heavily cloudy";
    case 5:
      return "thunderstorm";
    case 6:
      return "heavy rain";
    case 7:
      return "snowfall";
    case 8:
      return "fog";
    case 9:
      return "sleet";
    case 10:
      return "sleet";
    case 11:
      return "light rain";
    case 12:
      return "snow shower";
    case 13:
      return "thunderstorm";
    case 14:
      return "low stratus";
    case 15:
      return "sleet shower";
    default:
      fprintf(stderr, "TODO: error handling");
      exit(1);
  }
}

const char * get_json_value(json_object *json_parent, char *key) {
  json_object *json_child;
  int success = json_object_object_get_ex(json_parent, key, &json_child);
  if (!success) {
    fprintf(stderr, "TODO: error handling");
    exit(1);
  }
  return json_object_get_string(json_child);
}

json_object * get_json_object_child(json_object *json_parent, char *key) {
  json_object *json_child;
  int success = json_object_object_get_ex(json_parent, key, &json_child);
  if (!success) {
    fprintf(stderr, "TODO: error handling, key: %s\n", key);
    exit(1);
  }
  return json_child;
}

UT_array * request_hydrometric_data(RequestData *request) {
  UT_icd hydrometric_data_icd = {sizeof(HydrometricData), NULL, NULL, NULL};
  UT_array *measurements;
  utarray_new(measurements,&hydrometric_data_icd);
  char **city=NULL;

  while (city=(char**)utarray_next(request->cities,city)) {
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

    json_object *json_aare = get_json_object_child(json, "aare");

    HydrometricData hydrometric_data;
    hydrometric_data.city = *city;
    hydrometric_data.temperature_water = get_json_value(json_aare, "temperature");
    hydrometric_data.temperature_water_forecast2h = get_json_value(json_aare, "forecast2h");
    hydrometric_data.flow = get_json_value(json_aare, "flow");

    utarray_push_back(measurements, &hydrometric_data);
  }

  return measurements;
}

UT_array * request_mixed_data(RequestData *request) {
  UT_icd mixed_data_icd = {sizeof(MixedData), NULL, NULL, NULL};
  UT_array *measurements;
  utarray_new(measurements,&mixed_data_icd);
  char **city=NULL;

  while (city=(char**)utarray_next(request->cities,city)) {
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

    json_object *json_aare = get_json_object_child(json, "aare");
    json_object *json_weather = get_json_object_child(json, "weather");
    json_object *json_weather_current = get_json_object_child(json_weather, "current");
    json_object *json_weather_today = get_json_object_child(json_weather, "today");
    json_object *json_weather_afternoon = get_json_object_child(json_weather_today, "v");
    json_object *json_weather_evening = get_json_object_child(json_weather_today, "a");

    MixedData mixed_data;
    mixed_data.city = *city;
    mixed_data.temperature_water = get_json_value(json_aare, "temperature");
    mixed_data.temperature_air = get_json_value(json_weather_current, "tt");
    mixed_data.temperature_air_afternoon = get_json_value(json_weather_afternoon, "tt");
    mixed_data.weather_condition = map_weather_condition_code(atoi(get_json_value(json_weather_afternoon, "symt")));
    mixed_data.temperature_air_evening = get_json_value(json_weather_evening, "tt");

    utarray_push_back(measurements, &mixed_data);
  }
  return measurements;
}

UT_array * request_cities() {
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
    json_object *json_city_name = get_json_object_child(json_city, "city");
    const char *city = json_object_get_string(json_city_name);
    utarray_push_back(cities, &city);
  }
  return cities;
}
