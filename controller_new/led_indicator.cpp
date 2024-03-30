#include "led_indicator.hpp"

LedIndicator::LedIndicator()
{
}

void LedIndicator::SetLedStatus(bool status)
{
  digitalWrite(SYS_LED_GPIO_PIN, status ? HIGH : LOW);
}

void LedIndicator::Blink(int times, int on_time, int off_time)
{
  for (int i = 0; i < times; ++i) 
  {
    SetLedStatus(true);
    delay(on_time);
    SetLedStatus(false);
    if (i != times - 1)
      delay(off_time);
  }
}