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
const static char CurrentAmps1[]             PROGMEM = " A";

const char* const StringTable[] PROGMEM = 
{ 
  MemTestFailedMessage,      //0
  CurrentTestFailedMessage,  //1
  RestartAndFixMessage,      //2
  CurrentAmps0,              //3
  CurrentAmps1               //4
};

#endif
