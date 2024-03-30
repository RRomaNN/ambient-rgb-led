#ifndef __MENU__
#define __MENU__

#include "lcd.hpp"
#include "state_machine.hpp"

class Menu
{
  public:
    Menu(LPLcd lcd, LPStateMachine state_machine);
    
    void RenderCurrentState(float current_amps);

  private:
    void PrintBackground(uint8_t pattern_index, uint8_t color, uint16_t speed, uint16_t led_count, float current_amps);
    void PrintColorSelectMode(uint8_t pattern_index, uint8_t color, uint32_t set_color_0, uint32_t set_color_1, uint32_t set_color_2, uint32_t set_color_3);
    void ColorToHexString(uint8_t* color, uint8_t len, char* buffer);
    void PrintColorSetMode(uint32_t set_color_0, uint32_t set_color_1, uint8_t color_setting_phase, uint8_t page);
    void PrintLedSettings(uint16_t led_count);
    void Print3DigitNumber(uint16_t number, char* buffer);
    void Print2DigitNumber(uint8_t number, char* buffer);
    void PrintSpeedSettings(uint16_t speed);

    LPLcd lcd;
    LPStateMachine state_machine;
};

#define LPMenu Menu*

#endif