#include <stdint.h>
#ifndef __RENDERING_ENGINE__
#define __RENDERING_ENGINE__

#include "neopixel.hpp"
#include "state_machine.hpp"
#include "eeprom.hpp"

static const uint8_t PixelsPer2ColorBlock = 0x80;
static const uint8_t PixelsPer4ColorBlock = 0x40;
static const uint8_t ImageRowCount = 225;

class RendringEngine
{
  public:
    RendringEngine(LPStateMachine state_machine, LPEeprom eeprom, LPNeoPixel neopixel);

    void InitRendering();
    void Render();
  
  private:
    uint8_t GetWhiteComponent(uint8_t red, uint8_t green, uint8_t blue);
    void GetColorComponents(uint8_t* red, uint8_t* green, uint8_t* blue, uint32_t color);

    LPStateMachine state_machine;
    LPEeprom eeprom;
    LPNeoPixel neopixel;

    uint16_t current_counter;
    uint8_t current_row;
};

#define LPRendringEngine RendringEngine*

#endif