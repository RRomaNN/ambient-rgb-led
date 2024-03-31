#ifndef __LCD__
#define __LCD__

#include "global.hpp"

static const uint8_t LcdRs = 8; 
static const uint8_t LcdEn = 11; 
static const uint8_t LcdD4 = 4; 
static const uint8_t LcdD5 = 5; 
static const uint8_t LcdD6 = 6; 
static const uint8_t LcdD7 = 7; 

class Lcd
{
  public:
    Lcd(); 
    ~Lcd();

    void Print(uint8_t row, const char* text);
    void PrintAll(const char* text0, const char* text1);
    void PrintMemTestError();
    void PrintCurrentTestError();
    void PrintCurrentAmps(float current_amps);
    void PrintPatternDescription(uint8_t selected_pattern);
    void PrintLedSettingWindow(char* led_count);
    void PrintColorSettingWindow(char* hex_string, char* pointer, uint8_t phase_index, uint8_t page, uint8_t sub_phase);
    void PrintPlaySpeedWindow(char* play_speed);
    void PrintPreviewColorsWindow(bool preview_colors);

  private:
    LiquidCrystal* lcd;
};

#define LPLcd Lcd*

#endif