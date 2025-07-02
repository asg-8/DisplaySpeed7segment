#include "defines.h"

unsigned long micros_display = 0; //DO A FUNCTION FOR ELAPSED MILLIS

void setup()
{
  // put your setup code here, to run once:
  setup_digital_pins();

  Serial.begin(19200);
  while (!Serial);  // wait for serial port to connect. Needed for native USB port only

  lamp_test();

  Serial.println("SETUP DONE");
}

void loop()
{
  // put your main code here, to run repeatedly:
  int speed = loop_speed();

  if (speed < 0) speed = 0;
  if (speed > 999) speed = 999;
  // speed = 123;
  // display_set_value(speed);

  int potval = potRead();
  int configval = potControlDiscrete(potval, 10);
  Serial.print(potval);
  Serial.print("\t");
  Serial.println(configval);

  if (potval > 999) potval = 999;
  display_set_value(potval, 0);

  if(digitalRead(A5))
  {
    digitalWrite(8, potval > 333);
    digitalWrite(9, potval < 666);
  }
  else
  {
    digitalWrite(8, false);
    digitalWrite(9, false);
  }

  loop_update_display();
}

void loop_update_display(void)
{
  unsigned long time_micros = micros();

  if (time_micros >= micros_display + DISPLAY_REFRESH)
  {
    micros_display = time_micros;
    display_scheduled();
    // Serial.println("DISPLAY UPDATE");
  }
}

void lamp_test(void)
{
  display_set_value(888, 0);
  digitalWrite(6, true);
  digitalWrite(7, false);
  digitalWrite(8, true);
  digitalWrite(9, false);

  delayUpdateDisplay(500);

  digitalWrite(6, false);
  digitalWrite(7, true);
  digitalWrite(8, false);
  digitalWrite(9, true);

  delayUpdateDisplay(500);

  display_set_value(0, 0b111);
  digitalWrite(6, false);
  digitalWrite(7, false);
  digitalWrite(8, false);
  digitalWrite(9, false);
  
  delayUpdateDisplay(500);
}

void delayUpdateDisplay(int delay)
{
  //delay(300);
  for(int i = 0; i < delay; ++i)
  {
    display_scheduled();
    delayMicroseconds(1000);
  }
}