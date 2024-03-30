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