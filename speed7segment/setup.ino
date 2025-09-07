#include "defines.h"

void setup_digital_pins(void)
{
  pinMode(PIN_DO_7SEG_BCD_A, OUTPUT); //BCD A
  pinMode(PIN_DO_7SEG_BCD_B, OUTPUT); //BCD B
  pinMode(PIN_DO_7SEG_BCD_C, OUTPUT); //BCD C
  pinMode(PIN_DO_7SEG_BCD_D, OUTPUT); //BCD D

  pinMode(PIN_DO_7SEG_DP, OUTPUT); //DP

  pinMode(PIN_DO_7SEG_MUX_1, OUTPUT); //MUX DIG1
  pinMode(PIN_DO_7SEG_MUX_2, OUTPUT); //MUX DIG2
  pinMode(PIN_DO_7SEG_MUX_3, OUTPUT); //MUX DIG3

  pinMode(PIN_DO_TRIGRDY_A, OUTPUT); //TRIG rdy A
  pinMode(PIN_DO_TRIGRDY_B, OUTPUT); //TRIG rdy B

  pinMode(PIN_DO_BICOLOR_R, OUTPUT); //LED bicolor R
  pinMode(PIN_DO_BICOLOR_G, OUTPUT); //LED bicolor G

  pinMode(PIN_DI_SENSOR_A, INPUT); //Sensor A
  pinMode(PIN_DI_SENSOR_B, INPUT); //Sensor B
  pinMode(PIN_AI_SENSOR_A, INPUT); //Sensor A
  pinMode(PIN_AI_SENSOR_B, INPUT); //Sensor B

  pinMode(PIN_DI_BUTTON, INPUT); //Pushbutton
  pinMode(PIN_AI_POT, INPUT); //Potentiometer
}