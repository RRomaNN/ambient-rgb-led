#include "neopixel.hpp"

NeoPixel::NeoPixel(uint8_t led_count)
{
  this->led_count = led_count;

  neopixel = new Adafruit_NeoPixel(led_count, LED_STRIP_PIN, NEO_GRBW + NEO_KHZ800);
  neopixel->begin();
  neopixel->setBrightness(DEFAULT_LED_BRIGHTNESS);
}

void NeoPixel::SetNeoPixelColors(byte* intensities)
{
  for(int i = 0; i < led_count; ++i) 
    neopixel->setPixelColor(i, intensities[i * 3], intensities[i * 3 + 1], intensities[i * 3 + 2], intensities[i * 3 + 3]);
  neopixel->show();
}

void NeoPixel::ShutDown()
{
  for(int i = 0; i < led_count; ++i) 
    neopixel->setPixelColor(i, 0, 0, 0, 0);
  neopixel->show();
}

NeoPixel::~NeoPixel()
{
  delete neopixel;
}