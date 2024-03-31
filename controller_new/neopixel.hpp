#include <stdint.h>
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

    void SetLedCount(uint8_t led_count);
    void SetNeoPixelColor(uint16_t pixel_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
    void Display();
    void ShutDown();
  
  private:
    Adafruit_NeoPixel* neopixel;
    byte led_count;
};

#define LPNeoPixel NeoPixel*

#endif