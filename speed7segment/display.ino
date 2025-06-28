struct
{
  uint16_t value;
  uint16_t value_current;
  uint8_t flags;
  uint8_t mux;
} display_status = {0};

void display_set_value(int value)
{
  display_status.value = value;
}

void display_7seg_ABCD(uint8_t value)
{
  value %= 10;

  bool pinA = (value & 0x01) > 0;
  bool pinB = (value & 0x02) > 0;
  bool pinC = (value & 0x04) > 0;
  bool pinD = (value & 0x08) > 0;

  digitalWrite(2, pinA);
  digitalWrite(3, pinB);
  digitalWrite(4, pinC);
  digitalWrite(5, pinD);
}

void display_mux(uint8_t output_enable)
{
  digitalWrite(12, (output_enable & 0x01) > 0);
  digitalWrite(11, (output_enable & 0x02) > 0);
  digitalWrite(10, (output_enable & 0x04) > 0);
}

void display_driver(uint16_t val_7seg, uint8_t flags, uint8_t mux)
{
  display_mux(0);
  display_7seg_ABCD((val_7seg >> (4*mux)) & 0x000F);
  //flags
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
  }

  display_driver(display_integer_value_to_hex(display_status.value_current), 0, display_status.mux);

  display_status.mux++;
  if (display_status.mux >= 3)
  {
    display_status.mux = 0;
  }
}

