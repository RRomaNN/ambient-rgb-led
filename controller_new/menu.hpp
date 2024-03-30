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
    void PrintBackground(StateMachine::ModeType mode_type, uint8_t pattern_index, uint8_t color2, uint8_t color4, uint16_t speed, uint8_t led_count, float current_amps);

    LPLcd lcd;
    LPStateMachine state_machine;
};

#define LPMenu Menu*

#endif