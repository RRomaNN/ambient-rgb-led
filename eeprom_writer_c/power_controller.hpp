#ifndef __POWER_CONTROLLER__
#define __POWER_CONTROLLER__

#include "led_indicator.hpp"
#include "neopixel.hpp"
#include "lcd.hpp"

static const uint8_t CurrentMeterPin = 15; //A1
static const float CurrentTransformCoeff = (5.f * 25.f) / 1024.f; // (5[V] / 0.04 [Ohm]) / 1024 [ADC steps]
static const float CurrentCutOffThreshold = 5.f; //5 amps

class PowerController
{
  public:
    PowerController(LPLedIndicator led_indicatior, bool* global_lock, LPNeoPixel neopixel, LPLcd lcd);

    float AuditPowerStatusAndAct();
  
  private:
    LPLedIndicator led_indicatior;
    LPNeoPixel neopixel;
    LPLcd lcd;
    bool* global_lock;
};

#define LPPowerController PowerController*

#endif