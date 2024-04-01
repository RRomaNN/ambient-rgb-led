#include "power_controller.hpp"

 PowerController::PowerController(LPLedIndicator led_indicatior, bool* global_lock, LPNeoPixel neopixel, LPLcd lcd)
 {
   this->led_indicatior = led_indicatior;
   this->global_lock = global_lock;
   this->neopixel = neopixel;
   this->lcd = lcd;
 }
  
float PowerController::AuditPowerStatusAndAct()
{
  float led_current = (float)analogRead(CurrentMeterPin) * CurrentTransformCoeff; 

  if (led_current > CurrentCutOffThreshold)
  {
    neopixel->ShutDown();
    lcd->PrintCurrentTestError();
    led_indicatior->Blink(10, 100, 100);
    *global_lock = true;
  }

  return led_current;
}