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

  button_setup_callbacks(&callback_button_pressed, &callback_button_released);

  Serial.println("SETUP DONE");

  // test_adc();
}

void test_adc(void)
{
  loop_test_adc_dig();
  loop_test_adc_dig_N(10);
  loop_test_adc_dig_N(100);
  loop_test_adc_dig_N(1000);
  loop_test_adc_dig_N(10000);
}

int mode = 0;

void callback_button_pressed(void)
{
  if (mode != 3) ++mode;
  // if (mode > 2) mode = 0;
  Serial.println(mode);
}

int button_ms_released = 0;
void callback_button_released(int ms)
{
  button_ms_released = ms;
}

void loop()
{
  button_scheduled();

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
      sensors_scheduled();
      loop_mode_speed();
      break;
    }

    case 2:
    {
      loop_test_pot();
      break;
    }

    case 3:
    {
      loop_test_button();
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

  // if(!digitalRead(PIN_DI_BUTTON))
  // {
    digitalWrite(PIN_DO_BICOLOR_R, potval > 333);
    digitalWrite(PIN_DO_BICOLOR_G, potval < 666);
  // }
  // else
  // {
  //   digitalWrite(PIN_DO_BICOLOR_R, false);
  //   digitalWrite(PIN_DO_BICOLOR_G, false);
  // }
}



void loop_test_button(void)
{
  int button_press_ms = button_get_ms_press();
  
  if (button_press_ms == 0)
    display_set_value(button_ms_released/10, 0, 0b100);
  else
    display_set_value(button_press_ms/10, 0, 0b100);

  if(button_press_ms == 0)
  {
    digitalWrite(PIN_DO_BICOLOR_G, 0);
    digitalWrite(PIN_DO_BICOLOR_R, 0);
  }
  else
  if(button_press_ms < 300)
  {
    digitalWrite(PIN_DO_BICOLOR_G, 1);
    digitalWrite(PIN_DO_BICOLOR_R, 0);
  }
  else
  if(button_press_ms < 1000)
  {
    digitalWrite(PIN_DO_BICOLOR_G, 1);
    digitalWrite(PIN_DO_BICOLOR_R, 1);
  }
  else
  {
    digitalWrite(PIN_DO_BICOLOR_G, 0);
    digitalWrite(PIN_DO_BICOLOR_R, 1);
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
  bool sensorA = false;
  bool sensorB = false;

  sensorA = !digitalRead(PIN_DI_SENSOR_A);
  sensorB = !digitalRead(PIN_DI_SENSOR_B);

  digitalWrite(PIN_DO_TRIGRDY_A, !sensorA);
  digitalWrite(PIN_DO_TRIGRDY_B, !sensorB);
  digitalWrite(PIN_DO_BICOLOR_R, sensorA);
  digitalWrite(PIN_DO_BICOLOR_G, sensorB);
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
  digVal = digitalRead(PIN_AI_POT);
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
  digVal = digitalRead(PIN_AI_POT);
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
  digitalWrite(PIN_DO_TRIGRDY_A, false);
  digitalWrite(PIN_DO_TRIGRDY_B, true);
  digitalWrite(PIN_DO_BICOLOR_R, true);
  digitalWrite(PIN_DO_BICOLOR_G, false);

  delayUpdateDisplay(500);

  digitalWrite(PIN_DO_TRIGRDY_A, true);
  digitalWrite(PIN_DO_TRIGRDY_B, false);
  digitalWrite(PIN_DO_BICOLOR_R, false);
  digitalWrite(PIN_DO_BICOLOR_G, true);

  delayUpdateDisplay(500);

  display_set_value(0, 0b111);
  digitalWrite(PIN_DO_TRIGRDY_A, true);
  digitalWrite(PIN_DO_TRIGRDY_B, true);
  digitalWrite(PIN_DO_BICOLOR_R, false);
  digitalWrite(PIN_DO_BICOLOR_G, false);
  
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