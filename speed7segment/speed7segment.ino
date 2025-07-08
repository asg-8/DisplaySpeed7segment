#include "defines.h"
#include "display.h"

unsigned long micros_display = 0; //DO A FUNCTION FOR ELAPSED MILLIS
int gate_ms[] = {500, 1000, 1500, 2000, 2500, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

void callback_button_pressed(void);

void setup()
{
  // put your setup code here, to run once:
  setup_digital_pins();

  Serial.begin(19200);
  while (!Serial);  // wait for serial port to connect. Needed for native USB port only

  lamp_test();

  button_setup_callbacks(&callback_button_pressed, nullptr);

  Serial.println("SETUP DONE");

  loop_test_adc_dig();
  loop_test_adc_dig_N(10);
  loop_test_adc_dig_N(100);
  loop_test_adc_dig_N(1000);
  loop_test_adc_dig_N(10000);
}

int mode = 1;

void callback_button_pressed(void)
{
  ++mode;
  if (mode > 2) mode = 0;
  Serial.println(mode);
}

void loop()
{
  button_scheduled();
  scheduled_sensors();

  switch(mode)
  {
    case 0:
    {
      display_set_value(0, 0b111);
      loop_mode_passthrough();
      break;
    }
    
    case 1:
    {
      loop_mode_speed();
      break;
    }

    case 2:
    {
      loop_test_pot();
      break;
    }

  }

  loop_update_display();
}

void loop_mode_speed(void)
{
  // put your main code here, to run repeatedly:
  int speed = sensors_get_speed();

  if (speed < 0) speed = 0;
  if (speed > 999) speed = 999;
  // speed = 123;
  display_set_value(speed);
}

int potval_prev = 0;
void loop_test_pot(void)
{
  int potval = potRead();
  int configval = potControlDiscrete(potval, 10);
  int configval2 = rescale(potval, 30, 1000, 0, sizeof(gate_ms)/sizeof(gate_ms[0])-1);

  if(potUpdated(potval, potval_prev, 15))
  {
    potval_prev = potval;
    Serial.print(potval);
    Serial.print("\t");
    Serial.print(configval);
    Serial.print("\t");
    Serial.print(configval2);
    Serial.print("\t");
    Serial.print(gate_ms[configval2]);
    Serial.println();
  }

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

void loop_mode_passthrough(void)
{
  digitalWrite(6, !digitalRead(A3));
  digitalWrite(7, !digitalRead(A2));
}

void loop_test_adc_dig(void)
{
  unsigned long micros_A_adc = 0;
  unsigned long micros_B_adc = 0;
  unsigned long micros_A_dig = 0;
  unsigned long micros_B_dig = 0;
  int potVal = 0;
  int digVal = 0;

  micros_A_adc = micros();
  potVal = analogRead(PIN_AI_POT);
  micros_B_adc = micros();

  micros_A_dig = micros();
  digVal = digitalRead(A3);
  micros_B_dig = micros();

  Serial.print(micros_B_adc - micros_A_adc); //208 us
  Serial.print("\t");
  Serial.print(micros_B_dig - micros_A_dig); //4 us
  Serial.println();
}

void loop_test_adc_dig_N(int N)
{
  unsigned long micros_A_adc = 0;
  unsigned long micros_B_adc = 0;
  unsigned long micros_A_dig = 0;
  unsigned long micros_B_dig = 0;
  unsigned long micros_A_ctrl = 0;
  unsigned long micros_B_ctrl = 0;
  int potVal = 0;
  int digVal = 0;

  micros_A_adc = micros();
  for(int i = 0; i < N; ++i)
  potVal = analogRead(PIN_AI_POT);
  micros_B_adc = micros();

  micros_A_dig = micros();
  for(int i = 0; i < N; ++i)
  digVal = digitalRead(A3);
  micros_B_dig = micros();

  micros_A_ctrl = micros();
  for(int i = 0; i < N; ++i) i;
  micros_B_ctrl = micros();

  Serial.print(N);
  Serial.print("\t");
  Serial.print(micros_B_adc - micros_A_adc);
  Serial.print("\t");
  Serial.print(micros_B_dig - micros_A_dig);
  Serial.print("\t");
  Serial.print(micros_B_ctrl - micros_A_ctrl); //4 us
  Serial.println();
  /*
  10    1120    44
  100   11200   424
  1000  112008  4152
  10000 1120012 41500
  */
}

void lamp_test(void)
{
  display_set_value(888, 0, 0b111);
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