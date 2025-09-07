#include "defines.h"

uint8_t display_mux = 0;

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

// void display_set_value(int value, uint8_t blank = 0)
void display_set_value(int value, uint8_t blank, int dots = 0)
{
  display_setpoint.value = value;
  display_setpoint.blank = blank;
  display_setpoint.dots = dots;
}

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
  digitalWrite(12, (output_enable & 0x01) > 0);
  digitalWrite(11, (output_enable & 0x02) > 0);
  digitalWrite(10, (output_enable & 0x04) > 0);
}

void display_driver(uint16_t val_7seg, uint8_t mux, bool blank = 0, bool dot = 0)
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

  digitalWrite(PIN_DO_7SEG_DP, dot);

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

void display_scheduled(void)
{
  if (display_status.mux == 0)
  {
    display_status.value_current = display_status.value;
    //blank
    //dot
  }

  display_driver(display_integer_value_to_hex(display_status.value_current), display_status.mux, 0, display_status.mux == 0);

  display_status.mux++;
  if (display_status.mux >= 3)
  {
    display_status.mux = 0;
  }
}


//fcns for TRg'd and bicolor
