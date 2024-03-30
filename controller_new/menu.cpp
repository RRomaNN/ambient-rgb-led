#include "menu.hpp"

Menu::Menu(LPLcd lcd, LPStateMachine state_machine)
{
  this->lcd = lcd;
  this->state_machine = state_machine;
}

void Menu::Print3DigitNumber(uint16_t number, char* buffer)
{
  if (number < 10)
  {
    buffer[0x0] = '0';
    buffer[0x1] = '0';
    itoa(number, buffer + 0x2, 10);
  }
  else if (number < 100)
  {
    buffer[0x0] = '0';
    itoa(number, buffer + 0x1, 10);
  }
  else 
    itoa(number, buffer + 0x0, 10);
}

void Menu::Print2DigitNumber(uint8_t number, char* buffer)
{
  if (number < 10)
  {
    buffer[0x0] = '0';
    itoa(number, buffer + 0x1, 10);
  }
  else 
    itoa(number, buffer + 0x0, 10);
}

void Menu::PrintBackground(uint8_t pattern_index, uint8_t color, uint16_t speed, uint16_t led_count, float current_amps)
{
  //"P0 C32 S512 L256"
  //"Current: 2.120 A"
  char raw0[17]; 

  raw0[0x0] = 'P';
  itoa(pattern_index, raw0 + 0x1, 10);
  raw0[0x2] = ' ';
  raw0[0x3] = 'C';
  Print2DigitNumber(color, raw0 + 0x4);
  raw0[0x6] = ' ';
  raw0[0x7] = 'S';
  Print3DigitNumber(speed, raw0 + 0x8);
  raw0[0xB] = ' ';
  raw0[0xC] = 'L';
  Print3DigitNumber(led_count, raw0 + 0xD);
  raw0[0x10] = '\0';
  
  lcd->Print(0, raw0);
  lcd->PrintCurrentAmps(current_amps);
}

void Menu::ColorToHexString(uint8_t* color, uint8_t len, char* buffer)
{
  for (int i = 0; i < 3; ++i)
  {
    byte nib1 = (color[i] >> 4) & 0x0F;
    byte nib2 = color[i] & 0x0F;
    buffer[(2 - i) * 2] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[(2 - i) * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[6] = '\0';
}

void Menu::PrintColorSelectMode(uint8_t pattern_index, uint8_t color, uint32_t set_color_0, uint32_t set_color_1, uint32_t set_color_2, uint32_t set_color_3)
{
  char raw0[17]; //"001122 334455 P0"
  char raw1[17]; //"667788 99AABB 32"

  ColorToHexString((uint8_t*) &set_color_0, 6, raw0);
  raw0[0x6] = ' ';
  ColorToHexString((uint8_t*) &set_color_1, 6, raw0 + 0x7);
  raw0[0xD] = ' ';
  raw0[0xE] = 'P';
  itoa(pattern_index, raw0 + 0xF, 10);
  raw0[0x10] = '\0';

  ColorToHexString((uint8_t*) &set_color_2, 6, raw1);
  raw1[0x6] = ' ';
  ColorToHexString((uint8_t*) &set_color_3, 6, raw1 + 0x7);
  raw1[0xD] = ' ';
  Print2DigitNumber(color, raw1 + 0xE);
  raw1[0x10] = '\0';
  
  lcd->PrintAll(raw0, raw1);
}

void Menu::PrintColorSetMode(uint32_t set_color_0, uint32_t set_color_1, uint8_t color_setting_phase, uint8_t page)
{
  //"001122 < Color A"    "001122    vv    "
  //"  ^^      99AABB"    "Color B > 99AABB"
  char col[7]; 
  char pointer[7];
  
  uint8_t phase_index = color_setting_phase / 3;
  uint8_t sub_phase = color_setting_phase % 3;
  if (phase_index == 0)
    for (int i = 0; i < 3; ++i)
      if (i == 2 - sub_phase)
      {
        pointer[i * 2] = '^';
        pointer[i * 2 + 1] = '^';
      }
      else 
      {
        pointer[i * 2] = ' ';
        pointer[i * 2 + 1] = ' ';  
      }
  else
    for (int i = 0; i < 3; ++i)
      if (i == 2 - sub_phase)
      {
        pointer[i * 2] = 'v';
        pointer[i * 2 + 1] = 'v';
      }
      else 
      {
        pointer[i * 2] = ' ';
        pointer[i * 2 + 1] = ' ';  
      }
      
  pointer[6] = '\0';

  if (phase_index == 0)
    ColorToHexString((uint8_t*) &set_color_0, 6, col);
  else
    ColorToHexString((uint8_t*) &set_color_1, 6, col);

  lcd->PrintColorSettingWindow(col, pointer, phase_index, page, sub_phase);
}

void Menu::PrintLedSettings(uint16_t led_count)
{
  char buffer[4];
  Print3DigitNumber(led_count, buffer);
  lcd->PrintLedSettingWindow(buffer);
}

void Menu::PrintSpeedSettings(uint16_t speed)
{
  char buffer[4];
  Print3DigitNumber(speed, buffer);
  lcd->PrintPlaySpeedWindow(buffer);
}

void Menu::RenderCurrentState(float current_amps, uint32_t set_color_0, uint32_t set_color_1, uint32_t set_color_2, uint32_t set_color_3)
{
  StateMachine::ModeType mode_type = state_machine->GetCurrentMode();
  uint8_t pattern_index = state_machine->GetSelectedPattern();
  uint8_t color2 = state_machine->GetSelectedColor2Option();
  uint8_t color4 = state_machine->GetSelectedColor4Option();
  uint16_t speed = state_machine->GetSelectedSpeed();
  uint16_t led_count = state_machine->GetSelectedLedCount();
  uint8_t color_setting_phase = state_machine->GetColorSettingPhase();

  uint32_t color_a, color_b, color_c, color_d;
  state_machine->GetSelectedColors(&color_a, &color_b, &color_c, &color_d);

  switch(mode_type)
  {
    case StateMachine::BackgroundMode:
      PrintBackground(pattern_index, pattern_index == 0 ? color2 : color4, speed, led_count, current_amps);
      break;
    case StateMachine::LedCountSelectMode:
      PrintLedSettings(led_count);
      break;
    case StateMachine::PatternSelectMode:
      lcd->PrintPatternDescription(pattern_index);
      break;
    case StateMachine::ColorSelectMode:
      PrintColorSelectMode(pattern_index, pattern_index == 0 ? color2 : color4, set_color_0, set_color_1, set_color_2, set_color_3);
      break;
    case StateMachine::Color2SettingMode:
    case StateMachine::Color4Setting0Mode:
      PrintColorSetMode(color_a, color_b, color_setting_phase, 0);
      break;
    case StateMachine::Color4Setting1Mode:
      PrintColorSetMode(color_c, color_d, color_setting_phase, 1);
      break;
    case StateMachine::SpeedSelectMode:
      PrintSpeedSettings(speed);
      break;
  }
}