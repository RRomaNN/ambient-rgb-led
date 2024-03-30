#include <stdint.h>
#ifndef __EEPROM__
#define __EEPROM__

#include "global.hpp"

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
static const uint16_t SelectedLedCount       = 0x6245; //uint16_t

class Eeprom
{
  public:
    Eeprom();

    uint8_t Ping();
    void ReadSavedSettings(uint8_t* selected_pattern, uint8_t* selected_color2, uint8_t* selected_color4, uint16_t* selected_speed, uint16_t* selected_led_count);
    void SaveSettings(uint8_t selected_pattern, uint8_t selected_color2, uint8_t selected_color4, uint16_t selected_speed, uint16_t selected_led_count);
    void ReadColor2Schema(uint8_t color_num, uint32_t* color_a, uint32_t* color_b);
    void ReadColor4Schema(uint8_t color_num, uint32_t* color_a, uint32_t* color_b, uint32_t* color_c, uint32_t* color_d);
    void SaveColor2Schema(uint8_t color_num, uint32_t color_a, uint32_t color_b);
    void SaveColor4Schema(uint8_t color_num, uint32_t color_a, uint32_t color_b, uint32_t color_c, uint32_t color_d);

  private:
    uint8_t ReadByte(int address);
    uint16_t ReadUint16(int address);
    uint32_t ReadUint24(int address);
    void WriteByte(int address, uint8_t data);
    void WriteUint16(int address, uint16_t data);
    void WriteUint24(int address, uint32_t data);
    void ReadData(int address, int size, byte* data);
};

#define LPEeprom Eeprom*

#endif