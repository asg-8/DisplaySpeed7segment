int potval_prev = 0;
void loop_test_pot(int display)
{
  int potval = potRead();
  int configval = potControlDiscrete(potval, 10);
  int configval2 = rescale(potval, 20, 985, 0, sizeof(gate_ms)/sizeof(gate_ms[0])-1);

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

  switch (display)
  {
    case 0:
      if (potval > 999) potval = 999;
      display_set_value(potval, 0);
      digitalWrite(PIN_DO_BICOLOR_R, potval > 333);
      digitalWrite(PIN_DO_BICOLOR_G, potval < 666);
      break;

    case 1:
      display_set_value_auto_dot(gate_ms[configval2], 3);
      digitalWrite(PIN_DO_BICOLOR_R, 0);
      digitalWrite(PIN_DO_BICOLOR_G, 0);
      break;
  }
}





void loop_test_button(void)
{
  long button_press_ms = button_get_ms_press();
  
  // if (button_press_ms == 0)
  //   display_set_value(button_ms_released/10, 0, 0b100);
  // else
  //   display_set_value(button_press_ms/10, 0, 0b100);
  
  if (button_press_ms == 0)
    display_set_value_auto_dot(button_ms_released, 3);
  else
    display_set_value_auto_dot(button_press_ms, 3);

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

void test_adc(void)
{
  loop_test_adc_dig();
  loop_test_adc_dig_N(10);
  loop_test_adc_dig_N(100);
  loop_test_adc_dig_N(1000);
  loop_test_adc_dig_N(10000);
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