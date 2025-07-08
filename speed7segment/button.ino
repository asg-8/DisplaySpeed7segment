#include "defines.h"

bool button_filtered = false;
long button_debounce_counter = 0;
long button_debounce_threshold = 10;

void (*p_fcn_press)(void) = nullptr;
void (*p_fcn_release)(void) = nullptr;

void button_scheduled(void)
{
  bool button = button_filter(!digitalRead(PIN_DI_BUTTON));
  static bool button_prev = false;

  Serial.print(button);
  Serial.print(button_prev);
  Serial.println();

  if (button && !button_prev)
  {
    button_cb_pressed();
    //milis
  }

  button_filtered = button;
  button_prev = button_filtered;
}

bool button_get_status(void)
{
  return button_filtered;
}

bool button_filter(bool button)
{
  static bool button_status = false;

  if (button)
  {
    ++button_debounce_counter;
  }
  else
  {
    --button_debounce_counter;
  }

  if (button_debounce_counter < 0) button_debounce_counter = 0;
  if (button_debounce_counter > button_debounce_threshold) button_debounce_counter = button_debounce_threshold;

  if (button_debounce_counter == 0) button_status = false;
  if (button_debounce_counter == button_debounce_threshold) button_status = true;

  return button_status;
}

//typedef void (*cbfcn)(void);

// void button_setup_callbacks(void (*press)(void) = nullptr, void (*release)(void) = nullptr)
void button_setup_callbacks(void (*press)(void), void (*release)(void))
{
  p_fcn_press = press;
  p_fcn_release = release;
}

void button_cb_pressed(void)
{
  if (p_fcn_press != nullptr)
  {
    (*p_fcn_press)();
  }
}

void button_cb_released(int type) //long or short
{

}


