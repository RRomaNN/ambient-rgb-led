#include "lcd.hpp"

Lcd::Lcd()
{
  lcd = new LiquidCrystal(LcdRs, LcdEn, LcdD4, LcdD5, LcdD6, LcdD7);
  lcd->begin(16, 2);
  lcd->clear();    //TODO: potential optimization
}

void Lcd::PrintMemTestError()
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[2]));
}

void Lcd::PrintCurrentTestError()
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[1]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[2]));
}

void Lcd::PrintCurrentAmps(float current_amps)
{
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[3]));
  lcd->print(current_amps, 3);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[4]));
}

void Lcd::PrintPatternDescription(uint8_t selected_pattern)
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[5]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[(selected_pattern == 0) ? 6 : 7]));
}

void Lcd::PrintLedSettingWindow(char* led_count)
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[8]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[9]));
  lcd->print(led_count);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0xA]));
}

void Lcd::PrintColorSettingWindow(char* hex_string, char* pointer, uint8_t phase_index, uint8_t page, uint8_t sub_phase)
{
  if (phase_index == 0)
  {
    lcd->setCursor(0, 0);
    lcd->print(hex_string);
    lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0xB + page * 2]));
    lcd->setCursor(0, 1);
    lcd->print(pointer);
    lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x10 + sub_phase]));
  }
  else 
  {
    lcd->setCursor(0, 0);
    lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x13 + sub_phase]));
    lcd->print(pointer);
    lcd->setCursor(0, 1);
    lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0xC + page * 2]));
    lcd->print(hex_string);
  }
}

void Lcd::PrintPlaySpeedWindow(char* play_speed)
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0xF]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x19]));
  lcd->print(play_speed);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x1A]));
}

void Lcd::PrintPreviewColorsWindow(bool preview_colors)
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x16]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[preview_colors ? 0x17 : 0x18]));
}

void Lcd::PrintStripTypeSelection(bool is_rgbw_strip)
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x1B]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[is_rgbw_strip ? 0x1D : 0x1C]));
}

void Lcd::PrintRestartIsNeeded()
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x1E]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x1F]));
}

void Lcd::PrintRePlugIsNeeded()
{
  lcd->setCursor(0, 0);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x20]));
  lcd->setCursor(0, 1);
  lcd->print((__FlashStringHelper*)pgm_read_word(&StringTable[0x21]));
}

void Lcd::Print(uint8_t row, const char* text)
{
  uint8_t buffer[0x11];
  memset(buffer, '\0', sizeof(buffer));
  memccpy(buffer, text, '\0', sizeof(buffer) - 1); 

  lcd->setCursor(0, row);
  lcd->print((char*)buffer);
}

void Lcd::PrintAll(const char* text0, const char* text1)
{
  Print(0, text0);
  Print(1, text1);
}

Lcd::~Lcd()
{
  delete lcd;
}