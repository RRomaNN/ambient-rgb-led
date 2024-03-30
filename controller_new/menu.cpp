#include "menu.hpp"

Menu::Menu(LPLcd lcd, LPStateMachine state_machine)
{
  this->lcd = lcd;
  this->state_machine = state_machine;
}

void Menu::PrintBackground(StateMachine::ModeType mode_type, uint8_t pattern_index, uint8_t color2, uint8_t color4, uint16_t speed, uint8_t led_count, float current_amps)
{
  char raw0[17]; //"P0 C32 S512 L256"
  char raw1[17]; //"Current: 2.120 A"

  raw0[0x0] = 'P';
  itoa(pattern_index, raw0 + 0x1, 10);
  raw0[0x2] = ' ';
  raw0[0x3] = 'C';
  if (pattern_index == 0)
  {
    if (color2 < 10)
    {
      raw0[0x4] = '0';
      itoa(color2, raw0 + 0x5, 10);
    }
    else 
      itoa(color2, raw0 + 0x4, 10);
  }
  else 
  {
    if (color4 < 10)
    {
      raw0[0x3] = '0';
      itoa(color4, raw0 + 0x5, 10);
    }
    else 
      itoa(color4, raw0 + 0x4, 10);
  }
  raw0[0x6] = ' ';
  raw0[0x7] = 'S';
  if (speed < 10)
  {
    raw0[0x8] = '0';
    raw0[0x9] = '0';
    itoa(speed, raw0 + 0xA, 10);
  }
  else if (speed < 100)
  {
    raw0[0x8] = '0';
    itoa(speed, raw0 + 0x9, 10);
  }
  else 
    itoa(speed, raw0 + 0x8, 10);
  raw0[0xB] = ' ';
  raw0[0xC] = 'L';
  if (led_count < 10)
  {
    raw0[0xD] = '0';
    raw0[0xE] = '0';
    itoa(led_count, raw0 + 0xF, 10);
  }
  else if (led_count < 100)
  {
    raw0[0xD] = '0';
    itoa(led_count, raw0 + 0xE, 10);
  }
  else 
    itoa(led_count, raw0 + 0xD, 10);
  raw0[0x10] = '\0';
  
  lcd->Print(0, raw0);
  lcd->PrintCurrentAmps(current_amps);
}

void Menu::RenderCurrentState(float current_amps)
{
  StateMachine::ModeType mode_type = state_machine->GetCurrentMode();
  uint8_t pattern_index = state_machine->GetSelectedPattern();
  uint8_t color2 = state_machine->GetSelectedColor2Option();
  uint8_t color4 = state_machine->GetSelectedColor4Option();
  uint16_t speed = state_machine->GetSelectedSpeed();
  uint8_t led_count = state_machine->GetSelectedLedCount();

  switch(mode_type)
  {
    case StateMachine::BackgroundMode:
      PrintBackground(mode_type, pattern_index, color2, color4, speed, led_count, current_amps);
      break;
  }
}