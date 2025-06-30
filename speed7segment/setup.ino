#include "defines.h"

void setup_digital_pins(void)
{
  pinMode(2, OUTPUT); //BCD A
  pinMode(3, OUTPUT); //BCD B
  pinMode(4, OUTPUT); //BCD C
  pinMode(5, OUTPUT); //BCD D

  pinMode(PIN_DO_7SEG_DP, OUTPUT); //DP

  pinMode(10, OUTPUT); //MUX DIG1
  pinMode(11, OUTPUT); //MUX DIG2
  pinMode(12, OUTPUT); //MUX DIG3

  pinMode(6, OUTPUT); //TRIG rdy A
  pinMode(7, OUTPUT); //TRIG rdy B

  pinMode(A2, INPUT); //Sensor A
  pinMode(A3, INPUT); //Sensor B
}