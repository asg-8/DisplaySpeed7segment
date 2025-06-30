#include "defines.h"

unsigned long micros_display = 0; //DO A FUNCTION FOR ELAPSED MILLIS

void setup() {
  // put your setup code here, to run once:
  setup_digital_pins();

  Serial.begin(19200);
  while (!Serial);  // wait for serial port to connect. Needed for native USB port only
  Serial.println("SETUP DONE");
}

void loop() {
  // put your main code here, to run repeatedly:
  int speed = loop_speed();

  if (speed < 0) speed = 0;
  if (speed > 999) speed = 999;
  // speed = 123;
  // display_set_value(speed);

  unsigned long time_micros = micros();
  if (time_micros >= micros_display + DISPLAY_REFRESH)
  {
    micros_display = time_micros;
    display_scheduled();
    // Serial.println("DISPLAY UPDATE");
  }
  // Serial.println(micros_display);

  int potval = potRead();
  int configval = potControlDiscrete(potval, 10);
  Serial.print(potval);
  Serial.print("\t");
  Serial.println(configval);
  if (potval > 999) potval = 999;
  display_set_value(potval);
}
