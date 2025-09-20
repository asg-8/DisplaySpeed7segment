#include "defines.h"
#include "display.h"

unsigned long micros_display = 0; //DO A FUNCTION FOR ELAPSED MILLIS
const int gate_ms[] = {500, 1000, 1500, 2000, 2500, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

void callback_button_pressed(void);



void setup()
{
  // put your setup code here, to run once:

  setup_digital_pins();

  Serial.begin(19200);
  while (!Serial);  // wait for serial port to connect. Needed for native USB port only

  lamp_test();

  button_setup_callbacks(&callback_button_pressed, &callback_button_released);
  
  // test_adc();

  Serial.println("SETUP DONE");
}



int mode = 0;

void loop()
{
  // put your main code here, to run repeatedly:

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
      loop_mode_speed(0);
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

void loop_mode_speed(bool reset)
{
  int speed = sensors_get_speed();
  int dots = 0; //overflow
  int blank = reset ? 0b111 : 0;

  if (speed < 0) speed = 0;
  if (speed > 999)
  {
    speed = 999;
    dots = 0b111;
  }
  display_set_value(speed, blank, dots);
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