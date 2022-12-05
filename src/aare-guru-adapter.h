#ifndef AARE_GURU_ADAPTER_H
#define AARE_GURU_ADAPTER_H

extern const char* AARE_GURU_REST_API_URL;
extern const char* AARE_GURU_REST_API_URL_PARAMETERS;

typedef struct {
  const char *city;
  const char *temperature_water;
  const char *temperature_water_forecast2h;
  const char *temperature_air;
  const char *flow;
} MeasurementData;

UT_array * get_cities();
UT_array * get_report();
UT_array * get_measurement_data(RequestData *request);

#endif /* AARE_GURU_ADAPTER_H */
