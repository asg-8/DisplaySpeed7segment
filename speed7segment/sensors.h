#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#define MILIS_GATE 3000

typedef enum sensor_enum {SENSOR_A, SENSOR_B} sensor_enum_t;
typedef enum sensor_type {SENSOR_DIGITAL, SENSOR_ANALOG} sensor_type_t;

void sensors_scheduled(void);
int sensors_get_speed(void);

#endif //SENSORS_H