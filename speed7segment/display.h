#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#define DISPLAY_DIGITS 3

enum {LED_BICOLOR_OFF, LED_BICOLOR_GREEN, LED_BICOLOR_RED, LED_BICOLOR_ORANGE};
enum {LED_SENSOR_A, LED_SENSOR_B};

void display_scheduled(void);
void display_set_value(int, uint8_t = 0, int = 0);
void display_set_value_auto_dot(long, int = 1);
void LED_bicolor(int);
void LED_sensor_A(bool);
void LED_sensor_B(bool);

#endif //DISPLAY_H
