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

  private:
    LiquidCrystal* lcd;
};

#define LPLcd Lcd*

#endif