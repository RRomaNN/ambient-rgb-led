#ifndef __NEOPIXEL__
#define __NEOPIXEL__

#define   DEFAULT_LED_BRIGHTNESS  255 
#define   LED_STRIP_PIN            10

#include "global.hpp"

class NeoPixel
{
  public:
    NeoPixel(uint8_t led_count);
    ~NeoPixel();

    void SetNeoPixelColors(byte* intensities);
    void SetLedCount(byte led_count);
    void ShutDown();
  
  private:
    Adafruit_NeoPixel* neopixel;
    byte led_count;
};

#define LPNeoPixel NeoPixel*

#endif