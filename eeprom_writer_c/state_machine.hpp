#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include "global.hpp"

static const uint8_t PatternCount = 2;
static const uint8_t ColorCount = 32;

static const uint8_t ColorSetPhaseCount = 6;

static const uint8_t StrongColorIncreaseDelta = 0x10;

static const uint8_t FastSpeedChangeDelta = 10;
static const uint8_t SlowestChangeSpeed = 159;

static const uint8_t FastLedCountChangeDelta = 0x10;
static const uint8_t MaxLedCount = 225;

class StateMachine
{
  public:
    StateMachine(uint8_t selected_pattern, uint8_t selected_color2, uint8_t selected_color4, uint8_t selected_speed, uint8_t led_count, bool preview_colors, bool is_rgbw_strip);

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
      PreviewColorMode = 8,
      SelectStripTypeMode = 9,
      RestartNeededMode = 10,
      RestartIsReallyNeededMode = 11,
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
    uint8_t GetSelectedSpeed();
    uint16_t GetLogarithmicSpeed();
    uint8_t GetSelectedColor2Option();
    uint8_t GetSelectedColor4Option();
    uint8_t GetSelectedLedCount();
    bool GetPreviewColorMode();
    bool IsRgbwStrip();
    void GetSelected2Colors(uint32_t* color_a, uint32_t* color_b);
    void GetSelected4Colors(uint32_t* color_a, uint32_t* color_b, uint32_t* color_c, uint32_t* color_d);
    void SetSelectedColors(uint32_t color_a, uint32_t color_b, uint32_t color_c, uint32_t color_d);
    uint8_t GetColorSettingPhase();
    bool IsRenderingEnabled();
 
  private:
    void ProcessColorSettingMode(ActionType action, uint32_t* color_a, uint32_t* color_b, ModeType next_mode);
    void DecreaseColor(uint8_t color_setting_phase, uint32_t* color_x);
    void IncreaseColor(uint8_t color_setting_phase, uint32_t* color_x);
    void IncreaseColorStrongly(uint8_t color_setting_phase, uint32_t* color_x);

    enum ModeType current_mode;

    uint8_t selected_pattern;
    uint8_t selected_color2;
    uint8_t selected_color4;
    uint8_t selected_speed;
    uint8_t led_count;
    bool preview_colors;
    bool is_rgbw_strip;

    uint32_t color0;
    uint32_t color1;
    uint32_t color2;
    uint32_t color3;
    uint8_t color_setting_phase;
};

#define LPStateMachine StateMachine* 

#endif