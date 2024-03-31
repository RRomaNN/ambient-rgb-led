#include "neopixel.hpp"

NeoPixel::NeoPixel(uint8_t led_count)
{
  this->led_count = led_count;

  //neopixel = new Adafruit_NeoPixel(led_count, LED_STRIP_PIN, NEO_GRBW + NEO_KHZ800); //RGBW version
  neopixel = new Adafruit_NeoPixel(led_count == 0 ? 1 : led_count, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
  neopixel->begin();
  neopixel->setBrightness(DEFAULT_LED_BRIGHTNESS);
}

void NeoPixel::SetNeoPixelColor(uint16_t pixel_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
  //neopixel->setPixelColor(pixel_num, red, green, blue, white); //RGBW version
  neopixel->setPixelColor(pixel_num, red + white, green + white, blue + white); 
}

void NeoPixel::Display()
{
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