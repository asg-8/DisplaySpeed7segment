#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#define DIGITS 3

void display_scheduled(void);
void display_set_value(int, uint8_t = 0, int = 0);

#endif //DISPLAY_H
