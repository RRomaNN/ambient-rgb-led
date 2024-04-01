#include "neopixel.hpp"

NeoPixel::NeoPixel(uint8_t led_count, bool is_rgbw_strip)
{
  this->led_count = led_count;
  this->is_rgbw_strip = is_rgbw_strip;

  neopixel = new Adafruit_NeoPixel(led_count == 0 ? 1 : led_count, LED_STRIP_PIN, (is_rgbw_strip ? NEO_GRBW : NEO_GRB) + NEO_KHZ800);
  neopixel->begin();
  neopixel->setBrightness(DEFAULT_LED_BRIGHTNESS);
}

void NeoPixel::SetNeoPixel4Color(uint16_t pixel_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
  neopixel->setPixelColor(pixel_num, red, green, blue, white);
}

void NeoPixel::SetNeoPixel3Color(uint16_t pixel_num, uint8_t red, uint8_t green, uint8_t blue)
{
  neopixel->setPixelColor(pixel_num, red, green, blue); 
}

void NeoPixel::Display()
{
  neopixel->show();
}

void NeoPixel::ShutDown()
{
  for(int i = 0; i < led_count; ++i) 
    if (is_rgbw_strip)
      neopixel->setPixelColor(i, 0, 0, 0, 0);
    else
      neopixel->setPixelColor(i, 0, 0, 0);

  neopixel->show();
}

NeoPixel::~NeoPixel()
{
  delete neopixel;
}