#include "defines.h"

//RE DO
//ADD HISTERESYS
uint16_t potControlDiscrete(uint16_t input, uint16_t steps)
{
    if (steps < 1) steps = 1;
    if (steps > 1000) steps = 1000;
    if (input > 1000) input = 1000;
    
    return (input / (1000/steps + 1));
}

int rescale(int input, int in_min, int in_max, int out_min, int out_max)
{
    double ratio = (double)(out_max - out_min + 1) / (double)(in_max - in_min);

    input++;
    if (input > in_max) input = in_max;
    if (input < in_min) input = in_min;
    input--;

    return ratio * (double)(input - in_min);
}

int digitalFilter(int input, int prev, int alpha1k)
{
    return input;
}

bool potUpdated(int value, int ref, int delta)
{
    return abs(value - ref) >= delta;
}

int potRead(void)
{
  return analogRead(PIN_AI_POT);
}