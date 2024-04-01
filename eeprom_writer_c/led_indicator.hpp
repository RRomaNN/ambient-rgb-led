#ifndef __LED_INDICATOR__
#define __LED_INDICATOR__

#define SYS_LED_GPIO_PIN     13

#define LONG_SIGNAL_MS     1000
#define SHORT_SIGNAL_MS     100

#include "global.hpp"

class LedIndicator
{
  public:
    LedIndicator();

    void SetLedStatus(bool status);
    void Blink(int times, int on_time, int off_time);
};

#define LPLedIndicator LedIndicator*

#endif