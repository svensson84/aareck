#include <stdlib.h>
#include <stdio.h>

#include <json-c/json.h>

#include "aare-guru-api.h"
#include "curl-util.h"

const char* AARE_GURU_REST_API_URL = "https://aareguru.existenz.ch/v2018/";
const char* AARE_GURU_REST_API_URL_PARAMETERS = "?app=aarechk&version=1.0.0";

/*printing the value corresponding to boolean, double, integer and strings*/
void print_json_value(json_object *jobj){
  enum json_type type;
  printf("type: ",type);
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean: printf("json_type_booleann");
                         printf("value: %sn", json_object_get_boolean(jobj)? "true": "false");
                         break;
    case json_type_double: printf("json_type_doublen");
                        printf("          value: %lfn", json_object_get_double(jobj));
                         break;
    case json_type_int: printf("json_type_intn");
                        printf("          value: %dn", json_object_get_int(jobj));
                         break;
    case json_type_string: printf("json_type_stringn");
                         printf("          value: %sn", json_object_get_string(jobj));
                         break;
  }

}

void json_parse_array( json_object *jobj, char *key) {
  void json_parse(json_object *jobj); /*Forward Declaration*/
  enum json_type type;

  json_object *jarray = jobj; /*Simply get the array*/
  if(key) {
    jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  }

  int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
  printf("Array Length: %dn",arraylen);
  int i;
  json_object * jvalue;

  for (i=0; i< arraylen; i++){
    jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    type = json_object_get_type(jvalue);
    if (type == json_type_array) {
      json_parse_array(jvalue, NULL);
    }
    else if (type != json_type_object) {
      printf("value[%d]: ",i);
      print_json_value(jvalue);
    }
    else {
      json_parse(jvalue);
    }
  }
}

/*Parsing the json object*/
void json_parse(json_object *jobj) {
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    //type = json_object_get_type(val);
    //printf("type: %d\n", type);
    //printf("key: %s\n", key);
    //switch (type) {
    //  case json_type_boolean: 
    //  case json_type_double: 
    //  case json_type_int: 
    //  case json_type_string: print_json_value(val);
    //                       break; 
    //  case json_type_object: printf("json_type_objectn");
    //                       jobj = json_object_object_get(jobj, key);
    //                       json_parse(jobj); 
    //                       break;
    //  case json_type_array: printf("type: json_type_array, ");
    //                      json_parse_array(jobj, key);
    //                      break;
    //}
  }
}

void list_cities() {
  printf("measurement data is available for the following cities:\n");

  //char *request_url = strcat(strcat(AARE_GURU_REST_API_URL, "cities"), AARE_GURU_REST_API_URL_PARAMETERS);
  //const char *json_string = curl(request_url);

  const char *json_string = curl("https://aareguru.existenz.ch/v2018/cities?app=aarechk&version=1.0.0");
  json_object *json = json_tokener_parse(json_string);
  //printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

  int number_of_cities = json_object_array_length(json);
  int i;

  for (i=0; i<number_of_cities; i++){
    json_object *json_city = json_object_array_get_idx(json, i);

    json_object *json_city_name;
    int success = json_object_object_get_ex(json_city, "city", &json_city_name);
    if (!success)
      printf("TODO: error handling");

    printf("(%d) %s\n", i+1, json_object_get_string(json_city_name));
  }
}
