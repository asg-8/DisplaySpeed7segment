#define DISPLAY_REFRESH 3 //ms

unsigned long milis_display = 0; //DO A FUNCTION FOR ELAPSED MILLIS

void setup() {
  // put your setup code here, to run once:
  setup_digital_pins();

  Serial.begin(19200);
  while (!Serial);  // wait for serial port to connect. Needed for native USB port only
}

void loop() {
  // put your main code here, to run repeatedly:
  int speed = loop_speed();

  if (speed > 999) speed = 999;
  display_set_value(speed);

  if (millis() >= milis_display + DISPLAY_REFRESH)
  {
    milis_display = millis();
    display_scheduled();
  }
}
