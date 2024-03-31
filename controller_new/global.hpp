#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

static const uint8_t IdleTimeUs = 50;

const static char MemTestFailedMessage[]     PROGMEM = "Memtest failed! ";
const static char CurrentTestFailedMessage[] PROGMEM = "Current > 5 Amps";
const static char RestartAndFixMessage[]     PROGMEM = "Fix & restart me";
const static char CurrentAmps0[]             PROGMEM = "Current: ";
const static char CurrentAmps1[]             PROGMEM =               " A";
const static char SelectColorPatternH[]      PROGMEM = "Select pattern  ";
const static char SelectColorPattern2[]      PROGMEM = "<   2 colors   >";
const static char SelectColorPattern4[]      PROGMEM = "<   4 colors   >";
const static char SelectLedCountH[]          PROGMEM = "Set LED count   ";
const static char SelectLedCount0[]          PROGMEM = "<     ";
const static char SelectLedCount1[]          PROGMEM =          "      >";
const static char ColorSettingModeA[]        PROGMEM =       " < Color A";
const static char ColorSettingModeB[]        PROGMEM = "Color B > ";
const static char ColorSettingModeC[]        PROGMEM =       " < Color C";
const static char ColorSettingModeD[]        PROGMEM = "Color D > ";
const static char SelectSpeed[]              PROGMEM = "Set play speed  ";
const static char ColorSettingRed0[]         PROGMEM = "   Set red";
const static char ColorSettingGreen0[]       PROGMEM = " Set green";
const static char ColorSettingBlue0[]        PROGMEM = "  Set blue";
const static char ColorSettingRed1[]         PROGMEM = "Set red   ";
const static char ColorSettingGreen1[]       PROGMEM = "Set green ";
const static char ColorSettingBlue1[]        PROGMEM = "Set blue  ";
const static char PreviewColorsH[]           PROGMEM = "Preview colors ?";
const static char PreviewColorsYes[]         PROGMEM = "<      Yes     >";
const static char PreviewColorsNo[]          PROGMEM = "<      No      >";
const static char SelectSpeed0[]             PROGMEM = "<    ";
const static char SelectSpeed1[]             PROGMEM =           "     >";

const char* const StringTable[] PROGMEM = 
{ 
  MemTestFailedMessage,      //0
  CurrentTestFailedMessage,  //1
  RestartAndFixMessage,      //2
  CurrentAmps0,              //3
  CurrentAmps1,              //4
  SelectColorPatternH,       //5
  SelectColorPattern2,       //6
  SelectColorPattern4,       //7
  SelectLedCountH,           //8
  SelectLedCount0,           //9
  SelectLedCount1,           //A
  ColorSettingModeA,         //B
  ColorSettingModeB,         //C
  ColorSettingModeC,         //D
  ColorSettingModeD,         //E
  SelectSpeed,               //F
  ColorSettingRed0,          //10
  ColorSettingGreen0,        //11
  ColorSettingBlue0,         //12 
  ColorSettingRed1,          //13
  ColorSettingGreen1,        //14
  ColorSettingBlue1,         //15 
  PreviewColorsH,            //16
  PreviewColorsYes,          //17
  PreviewColorsNo,           //18
  SelectSpeed0,              //19
  SelectSpeed1,              //1A
};

#endif
