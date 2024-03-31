#include <Wire.h>

#define EEPROM0_ADDRESS 0x50

#define MAX_IMG_LEN 225

static const uint16_t Pattern2ColorAddress   = 0x0000;
static const uint16_t Pattern4ColorAddress   = 0x2000;
static const uint16_t Schemas2ColorAddress   = 0x6000;
static const uint16_t SelectedPatternAddress = 0x6240;
static const uint16_t Selected2ColorAddress  = 0x6241;
static const uint16_t Selected4ColorAddress  = 0x6242;
static const uint16_t SelectedSpeedAddress   = 0x6243;
static const uint16_t LedCountAddress        = 0x6244; 
static const uint16_t PreviewLedAddress      = 0x6245; 
static const uint16_t IsRgbwTypeLedAddress   = 0x6246; 

//volatile int writeIndex = Pattern2ColorAddress;
volatile int writeIndex = Pattern4ColorAddress;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  wipeOutChip();
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    String command = Serial.readStringUntil('\n');
    //saveSchema32(writeIndex, command);
    saveSchema64(writeIndex, command);
    //writeIndex += 0x20;
    writeIndex += 0x40;
  }

  delay(50);
}

void wipeOutChip()
{
  for (int i = Schemas2ColorAddress; i < SelectedPatternAddress; i++)
  {
    writeEEPROM(EEPROM0_ADDRESS, i, 0);
    if (i % 0x100 == 0)
    {
      digitalWrite(LED_BUILTIN, HIGH); 
      delay(500);                     
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  writeEEPROM(EEPROM0_ADDRESS, SelectedPatternAddress, 0);
  writeEEPROM(EEPROM0_ADDRESS, Selected2ColorAddress, 0);
  writeEEPROM(EEPROM0_ADDRESS, Selected4ColorAddress, 0);
  writeEEPROM(EEPROM0_ADDRESS, SelectedSpeedAddress, 1);
  writeEEPROM(EEPROM0_ADDRESS, LedCountAddress, 1);
  writeEEPROM(EEPROM0_ADDRESS, PreviewLedAddress, 0);
  writeEEPROM(EEPROM0_ADDRESS, IsRgbwTypeLedAddress, 1);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void saveSchema64(int baseAddress, String row)
{  
  uint8_t line[64];
  memset(line, 0, sizeof(line));
  for(int i = 0; i < MAX_IMG_LEN; i++)
  {
    uint8_t color;
    switch(row[i])
    {
      case '0':
        color = 0;
        break;
      case '1':
        color = 1;
        break;
      case '2':
        color = 2;
        break;
      case '3':
        color = 3;
        break;
    } 
    line[i/4] = line[i/4] | color << (i * 2) % 8;
  }
  for(int i = 0; i < 0x40; i++)
    writeEEPROM(EEPROM0_ADDRESS, baseAddress + i, (byte)line[i]);
}

void saveSchema32(int baseAddress, String row)
{  
  uint8_t line[32];
  memset(line, 0, sizeof(line));
  for(int i = 0; i < MAX_IMG_LEN; i++)
  {
    unsigned int color = row[i] == '0' ? 0 : 1; 
    line[i/8] = line[i/8] | color << i % 8;
  }
  for(int i = 0; i < 0x20; i++)
    writeEEPROM(EEPROM0_ADDRESS, baseAddress + i, (byte)line[i]);
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  uint8_t busStatus = 0xFF;
  do       
  {
    Wire.beginTransmission(deviceaddress);
    busStatus = Wire.endTransmission();
  }
  while (busStatus != 0x00);
}
