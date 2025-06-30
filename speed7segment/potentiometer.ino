#include "defines.h"

//RE DO
//ADD HISTERESYS
uint16_t potControlDiscrete(uint16_t input, uint16_t steps)
{
    if (steps < 1)
        steps = 1;
    if (steps > 1000)
        steps = 1000;
    
    return (input / (1000/steps + 1));
}

int potRead(void)
{
  return analogRead(PIN_AI_POT);
}