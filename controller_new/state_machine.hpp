#include <stdint.h>
#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

//User option settings
static const uint8_t PatternCount = 2;
static const uint8_t ColorCount = 32;

static const uint8_t ColorSetPhaseCount = 6;

static const uint8_t  StrongColorIncreaseDelta = 0x10;

static const uint8_t  FastSpeedChangeDelta = 0x20;
static const uint16_t SlowestChangeSpeed = 0x200;

static const uint8_t  FastLedCountChangeDelta = 0x10;
static const uint16_t MaxLedCount = 0x100;

#include "global.hpp"

class StateMachine
{
  public:
    StateMachine(uint8_t selected_pattern, uint8_t selected_color2, uint8_t selected_color4, uint16_t selected_speed, uint16_t led_count);

    //Types
    enum ModeType 
    {
      BackgroundMode = 0, 
      LedCountSelectMode = 1,
      PatternSelectMode = 2, 
      ColorSelectMode = 3, 
      Color2SettingMode = 4,
      Color4Setting0Mode = 5,
      Color4Setting1Mode = 6,
      SpeedSelectMode = 7,
      UndefinedMode = 99
    };

    enum ActionType 
    {
      Select = 0,
      SelectLong = 1,
      Mode = 2,
      ModeLong = 3
    };

    void TransitState(ActionType action);
    ModeType GetCurrentMode();
    uint8_t GetSelectedPattern();
    uint16_t GetSelectedSpeed();
    uint8_t GetSelectedColor2Option();
    uint8_t GetSelectedColor4Option();
    uint16_t GetSelectedLedCount();
    void GetSelectedColors(uint32_t* color_a, uint32_t* color_b, uint32_t* color_c, uint32_t* color_d);
    uint8_t GetColorSettingPhase();
  
  private:
    void ProcessColorSettingMode(ActionType action, uint32_t* color_a, uint32_t* color_b, ModeType next_mode);
    void DecreaseColor(uint8_t color_setting_phase, uint32_t* color_x);
    void IncreaseColor(uint8_t color_setting_phase, uint32_t* color_x);
    void IncreaseColorStrongly(uint8_t color_setting_phase, uint32_t* color_x);

    enum ModeType current_mode;

    uint8_t selected_pattern;
    uint8_t selected_color2;
    uint8_t selected_color4;
    uint16_t selected_speed;
    uint16_t led_count;

    uint32_t color0;
    uint32_t color1;
    uint32_t color2;
    uint32_t color3;
    uint8_t color_setting_phase;
};

#define LPStateMachine StateMachine* 

#endif