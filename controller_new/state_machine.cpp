#include "state_machine.hpp"

StateMachine::StateMachine(uint8_t selected_pattern, uint8_t selected_color2, uint8_t selected_color4, uint8_t selected_speed, uint8_t led_count, bool preview_colors)
{
  current_mode = BackgroundMode;

  this->selected_pattern = selected_pattern;
  this->selected_color2 = selected_color2;
  this->selected_color4 = selected_color4;
  this->selected_speed = selected_speed;
  this->led_count = led_count;
  this->preview_colors = preview_colors;
}

void StateMachine::DecreaseColor(uint8_t color_setting_phase, uint32_t* color_x)
{
  uint8_t color_offset = color_setting_phase * 8;
  uint32_t color = (*color_x >> color_offset) & (uint32_t)0x000000FF;
  color = color == 0 ? 0xFF : color - 1;
  uint32_t new_part = color << color_offset;
  uint32_t old_part = *color_x & (~((uint32_t)0x000000FF << color_offset));
  *color_x = (new_part + old_part) & (uint32_t)0x00FFFFFF;
}

void StateMachine::IncreaseColor(uint8_t color_setting_phase, uint32_t* color_x)
{
  uint8_t color_offset = color_setting_phase * 8;
  uint32_t color = (*color_x >> color_offset) & (uint32_t)0x000000FF;
  color = color == 0xFF ? 0 : color + 1;
  uint32_t new_part = color << color_offset;
  uint32_t old_part = *color_x & (~((uint32_t)0x000000FF << color_offset));
  *color_x = (new_part + old_part) & (uint32_t)0x00FFFFFF;
}

void StateMachine::IncreaseColorStrongly(uint8_t color_setting_phase, uint32_t* color_x)
{
  uint8_t color_offset = color_setting_phase * 8;
  uint32_t color = (*color_x >> color_offset) & (uint32_t)0x000000FF;
  color = (color + StrongColorIncreaseDelta) % (uint32_t)0x00000100;
  uint32_t new_part = color << color_offset;
  uint32_t old_part = *color_x & (~((uint32_t)0x000000FF << color_offset));
  *color_x = (new_part + old_part) & (uint32_t)0x00FFFFFF;
}

void StateMachine::ProcessColorSettingMode(ActionType action, uint32_t* color_a, uint32_t* color_b, ModeType next_mode)
{
  switch(action)
  {
    case Select: // v
      if (color_setting_phase < 3) //first color
        DecreaseColor(color_setting_phase, color_a);
      else
        DecreaseColor(color_setting_phase - 3, color_b);
      break;
    case SelectLong:
      if (color_setting_phase < 3) //first color
        IncreaseColorStrongly(color_setting_phase, color_a);
      else
        IncreaseColorStrongly(color_setting_phase - 3, color_b);
      break;
    case Mode: // ^
      if (color_setting_phase < 3) //first color
        IncreaseColor(color_setting_phase, color_a);
      else
        IncreaseColor(color_setting_phase - 3, color_b);
      break;
    case ModeLong:
      if (color_setting_phase < ColorSetPhaseCount - 1)
        ++color_setting_phase;
      else
      {
        color_setting_phase = 0;
        current_mode = next_mode;
      }
      break;
  }
}

void StateMachine::TransitState(ActionType action)
{
  switch(current_mode)
  {
    case BackgroundMode:
      switch(action)
      {
        case SelectLong:
          current_mode = LedCountSelectMode;
          break;
        case Select:
          current_mode = PreviewColorMode;
          break;
        case Mode:
        case ModeLong:
          current_mode = PatternSelectMode;
          break;
      }
      break;
    case LedCountSelectMode:
      switch(action)
        {
          case Select: // v
            led_count = (led_count == 0) ? MaxLedCount : led_count - 1;
            break;
          case SelectLong: // ^^
            led_count = (led_count + FastLedCountChangeDelta) % (MaxLedCount + 1);
            break;
          case Mode: // ^
            led_count = (led_count == MaxLedCount) ? 0 : led_count + 1;
            break;
          case ModeLong:
            current_mode = BackgroundMode;
            break;
        }
      break;
    case PatternSelectMode:
      switch(action)
      {
        case Select:
        case SelectLong:
          selected_pattern = (selected_pattern == PatternCount - 1) ? 0 : selected_pattern + 1;
          break;
        case Mode:
        case ModeLong:
          current_mode = ColorSelectMode;
          break;
      }
      break;
    case ColorSelectMode:
      switch(action)
      {
        case Mode:        
          if (selected_pattern == 0)               
            selected_color2 = (selected_color2 == ColorCount - 1) ? 0 : selected_color2 + 1;
          else
            selected_color4 = (selected_color4 == ColorCount - 1) ? 0 : selected_color4 + 1;
          break;
        case Select:
          if (selected_pattern == 0)      
            selected_color2 = (selected_color2 == 0) ? ColorCount - 1 : selected_color2 - 1;
          else
            selected_color4 = (selected_color4 == 0) ? ColorCount - 1 : selected_color4 - 1;
          break;
        case ModeLong:
          current_mode = SpeedSelectMode;
          break;
        case SelectLong:       
          color_setting_phase = 0;
          if (selected_pattern == 0)   
            current_mode = Color2SettingMode;
          else
            current_mode = Color4Setting0Mode;
          break;
      }
      break;
    case Color2SettingMode:
      ProcessColorSettingMode(action, &color0, &color1, SpeedSelectMode);
      break;
    case Color4Setting0Mode:
      ProcessColorSettingMode(action, &color0, &color1, Color4Setting1Mode);
      break;
    case Color4Setting1Mode:
      ProcessColorSettingMode(action, &color2, &color3, SpeedSelectMode);
      break;
    case SpeedSelectMode:
      switch(action)
        {
          case Select: // v
            selected_speed = (selected_speed == 0) ? SlowestChangeSpeed : selected_speed - 1;
            break;
          case SelectLong: // ^^
            selected_speed = (selected_speed + FastSpeedChangeDelta) % (SlowestChangeSpeed + 1);
            break;
          case Mode: // ^
            selected_speed = (selected_speed == SlowestChangeSpeed) ? 0 : selected_speed + 1;
            break;
          case ModeLong:
            current_mode = BackgroundMode;
            break;
        }
      break;
    case PreviewColorMode:
      switch(action)
      {
        case Select:
        case Mode:
        case SelectLong:
          preview_colors = !preview_colors;
          break;
        case ModeLong:
          current_mode = BackgroundMode;
          break;
      }
      break;
  }
}

StateMachine::ModeType StateMachine::GetCurrentMode()
{
  return current_mode;
}

uint8_t StateMachine::GetSelectedPattern()
{
  return selected_pattern;
}

uint8_t StateMachine::GetSelectedSpeed()
{
  return selected_speed;
}

uint8_t StateMachine::GetSelectedColor2Option()
{
  return selected_color2;
}

uint8_t StateMachine::GetSelectedColor4Option()
{
  return selected_color4;
}

uint8_t StateMachine::GetSelectedLedCount()
{
  return led_count;
}

uint8_t StateMachine::GetColorSettingPhase()
{
  return color_setting_phase;
}

bool StateMachine::GetPreviewColorMode()
{
  return preview_colors;
}

bool StateMachine::IsRenderingEnabled()
{
  return preview_colors || current_mode == BackgroundMode;
}

uint16_t StateMachine::GetLogarithmicSpeed()
{
  return (uint16_t) pow(2., (float)selected_speed / 10);
}

void StateMachine::GetSelected2Colors(uint32_t* color_a, uint32_t* color_b)
{
  *color_a = color0;
  *color_b = color1;
}

void StateMachine::GetSelected4Colors(uint32_t* color_a, uint32_t* color_b, uint32_t* color_c, uint32_t* color_d)
{
  *color_a = color0;
  *color_b = color1;
  *color_c = color2;
  *color_d = color3;
}

void StateMachine::SetSelectedColors(uint32_t color_a, uint32_t color_b, uint32_t color_c, uint32_t color_d)
{
  color0 = color_a;
  color1 = color_b;
  color2 = color_c;
  color3 = color_d;
}
