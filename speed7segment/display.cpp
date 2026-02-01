#include "display.h"
#include "defines.h"

struct
{
  uint16_t value;
  uint8_t dots;
  uint8_t blank;
} display_status = {0};

struct
{
  uint16_t value;
  uint8_t dots;
  uint8_t blank;
} display_setpoint = {0};


// PRIVATE FUNCTIONS DECLARATION
void display_mux(uint8_t);




// PRIVATE FUNCTIONS DEFINITION

void display_7seg_ABCD(uint8_t value)
{
  // value %= 10;
  value &= 0x0F;

  bool pinA = (value & 0x01) > 0;
  bool pinB = (value & 0x02) > 0;
  bool pinC = (value & 0x04) > 0;
  bool pinD = (value & 0x08) > 0;

  digitalWrite(PIN_DO_7SEG_BCD_A, pinA);
  digitalWrite(PIN_DO_7SEG_BCD_B, pinB);
  digitalWrite(PIN_DO_7SEG_BCD_C, pinC);
  digitalWrite(PIN_DO_7SEG_BCD_D, pinD);
}

void display_mux(uint8_t output_enable)
{
  digitalWrite(PIN_DO_7SEG_MUX_3, (output_enable & 0x01) > 0);
  digitalWrite(PIN_DO_7SEG_MUX_2, (output_enable & 0x02) > 0);
  digitalWrite(PIN_DO_7SEG_MUX_1, (output_enable & 0x04) > 0);
}


void display_driver(uint16_t val_7seg, uint8_t mux, uint8_t blank, uint8_t dot)
{
  display_mux(0);
  
  if ((display_status.blank >> mux) & 0x01)
  {
    display_7seg_ABCD(0x0F);
  }
  else
  {
    display_7seg_ABCD((val_7seg >> (4*mux)) & 0x000F);
  }

  digitalWrite(PIN_DO_7SEG_DP, (display_status.dots >> mux) & 0x01);

  display_mux(1 << mux);
}

uint16_t display_integer_value_to_hex(uint16_t value)
{
  uint16_t hex_value = 0;
  hex_value |= value % 10;
  hex_value |= ((value/10) % 10) << 4;
  hex_value |= ((value/100) % 10) << 8;
  return hex_value;
}

// PUBLIC FUNCTIONS DEFINITION
void display_set_value(int value, uint8_t blank, int dots)
{
  display_setpoint.value = value;
  display_setpoint.blank = blank;
  display_setpoint.dots = dots;
}

void display_scheduled(void)
{
  static uint8_t mux = 0;

  if (mux == 0)
  {
    display_status.value = display_setpoint.value;
    display_status.dots = display_setpoint.dots;
    display_status.blank = display_setpoint.blank;
  }

  display_driver(display_integer_value_to_hex(display_status.value), mux, display_status.blank, display_status.dots);

  mux++;

  if (mux >= DISPLAY_DIGITS)
  {
    mux = 0;
  }
}



int number_len(long value)
{
  int len = 0;

  while (value)
  {
    value /= 10;
    ++len;
  }

  return len;
}

void display_set_value_auto_dot(long value, int decimals)
{
  int dots = 0;
  int digits_integer = number_len(value) - decimals;

  if (digits_integer <= 1) digits_integer = 1;

  switch (digits_integer)
  {
    case 1:   dots = 0b100; break;
    case 2:   dots = 0b010; break;
    default:  dots = 0b000;
  }

  // DISPLAY_DIGITS;
  for (int i = 0; i < digits_integer; ++i) value /= 10;

  // Serial.print(value);
  // Serial.print("\t");
  // Serial.print(decimals);
  // Serial.print("\t");
  // Serial.print(digits_integer);
  // Serial.print("\t");
  // Serial.print(dots);
  // Serial.println();

  display_set_value(value, 0, dots);
}

//fcns for TRg'd and bicolor

void LED_raw(bool LED_green, bool LED_red, bool LED_A, bool LED_B)
{
  digitalWrite(PIN_DO_BICOLOR_G, LED_green);
  digitalWrite(PIN_DO_BICOLOR_R, LED_red);
  digitalWrite(PIN_DO_TRIGRDY_A, LED_A);
  digitalWrite(PIN_DO_TRIGRDY_B, LED_B);
}

void LED_bicolor(color)
{
  digitalWrite(PIN_DO_BICOLOR_G, color == LED_BICOLOR_GREEN || color == LED_BICOLOR_ORANGE);
  digitalWrite(PIN_DO_BICOLOR_R, color == LED_BICOLOR_RED   || color == LED_BICOLOR_ORANGE);
}

void LED_sensor_A(bool state)
{
  digitalWrite(PIN_DO_TRIGRDY_A, !state);
}

void LED_sensor_B(bool state)
{
  digitalWrite(PIN_DO_TRIGRDY_B, !state);
}

void LED_sensor(led, bool state)
{
  int pin = -1;

  switch (led)
  {
    case LED_SENSOR_A:
      pin = PIN_DO_TRIGRDY_A;
      break;
    
    case LED_SENSOR_B:
      pin = PIN_DO_TRIGRDY_B;
      break;
  }

  if (pin != -1) digitalWrite(pin, state);
}

