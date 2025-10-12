#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#define DISPLAY_DIGITS 3

void display_scheduled(void);
void display_set_value(int, uint8_t = 0, int = 0);
void display_set_value_auto_dot(long, int = 1);

#endif //DISPLAY_H
