#include <stdint.h>
#ifndef __EEPROM__
#define __EEPROM__

static const uint8_t EepromAddress = 0x50;
static const uint8_t RwBlockSize  =  0x10;

static const uint16_t Pattern2ColorAddress   = 0x0000;
static const uint16_t Pattern2ColorLen       = 0x2000;
static const uint16_t Pattern4ColorAddress   = 0x2000;
static const uint16_t Pattern4ColorLen       = 0x4000;
static const uint16_t Schemas2ColorAddress   = 0x6000;
static const uint16_t Schemas2ColorLen       = 0x00C0;
static const uint16_t Schemas4ColorAddress   = 0x60C0;
static const uint16_t Schemas4ColorLen       = 0x0180;
static const uint16_t SelectedPatternAddress = 0x6240;
static const uint16_t Selected2ColorAddress  = 0x6241;
static const uint16_t Selected4ColorAddress  = 0x6242;
static const uint16_t SelectedSpeedAddress   = 0x6243; //uint16_t
static const uint16_t SelectedLedCount       = 0x6245; 

#include "global.hpp"

class Eeprom
{
  public:
    Eeprom();

    byte Ping();
    byte ReadByte(int address);
    void ReadData(int address, int size, byte* data);
    void WriteData(int address, int size, byte* data);
};

#define LPEeprom Eeprom*

#endif