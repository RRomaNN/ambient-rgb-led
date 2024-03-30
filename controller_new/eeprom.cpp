#include "eeprom.hpp"

Eeprom::Eeprom()
{

}

uint8_t Eeprom::Ping()
{
  Wire.beginTransmission(EepromAddress);
  Wire.write(0x00);
  return Wire.endTransmission();
}

uint8_t Eeprom::ReadByte(int address)
{
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.endTransmission();                  
  Wire.requestFrom((int)EepromAddress, 0x01);
  while (!Wire.available()) 
    delayMicroseconds(IdleTimeUs);
  return Wire.read();
}

uint16_t Eeprom::ReadUint16(int address)
{
  uint16_t result;
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.endTransmission();                  
  Wire.requestFrom((int)EepromAddress, 0x02);
  while (!Wire.available()) 
    delayMicroseconds(IdleTimeUs);

  *((uint8_t*)&result) = Wire.read();
  *((uint8_t*)&result + 1) = Wire.read();

  return result;
}

uint32_t Eeprom::ReadUint24(int address)
{
  uint32_t result;
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.endTransmission();                  
  Wire.requestFrom((int)EepromAddress, 0x03);
  while (!Wire.available()) 
    delayMicroseconds(IdleTimeUs);

  *((uint8_t*)&result) = Wire.read();
  *((uint8_t*)&result + 1) = Wire.read();
  *((uint8_t*)&result + 2) = Wire.read();
  *((uint8_t*)&result + 3) = 0x00;

  return result;
}

void Eeprom::WriteByte(int address, uint8_t data)
{
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.write(data);
  Wire.endTransmission();  

  uint8_t busStatus = 0xFF;
  do       
  {
    Wire.beginTransmission(EepromAddress);
    busStatus = Wire.endTransmission();
  }
  while (busStatus != 0x00);
}

void Eeprom::WriteUint16(int address, uint16_t data)
{
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.write(*((uint8_t*)&data));
  Wire.write(*((uint8_t*)&data + 1));
  Wire.endTransmission();  

  uint8_t busStatus = 0xFF;
  do       
  {
    Wire.beginTransmission(EepromAddress);
    busStatus = Wire.endTransmission();
  }
  while (busStatus != 0x00);
}

void Eeprom::WriteUint24(int address, uint32_t data)
{
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.write(*((uint8_t*)&data));
  Wire.write(*((uint8_t*)&data + 1));
  Wire.write(*((uint8_t*)&data + 2));
  Wire.endTransmission();  

  uint8_t busStatus = 0xFF;
  do       
  {
    Wire.beginTransmission(EepromAddress);
    busStatus = Wire.endTransmission();
  }
  while (busStatus != 0x00);
}

void Eeprom::ReadData(int address, int size, byte* data)
{
  int lastBlockBytes = size % RwBlockSize;
  bool isLastBlockPartial = lastBlockBytes > 0;
  int block_count = (size / RwBlockSize) + (isLastBlockPartial ? 1 : 0);

  for (int i = 0; i < block_count; ++i)
  {
    int batchAddress = address + i * RwBlockSize;
    Wire.beginTransmission(EepromAddress);
    Wire.write((int)(batchAddress >> 8));   // MSB
    Wire.write((int)(batchAddress & 0xFF)); // LSB
    Wire.endTransmission();   
    if (i < block_count - 1 || !isLastBlockPartial)
    {
      Wire.requestFrom((int)EepromAddress, (int)RwBlockSize);
      while (!Wire.available()) 
        delayMicroseconds(IdleTimeUs);

      for(int j = 0; j < RwBlockSize; ++j)
        data[i * RwBlockSize + j] = Wire.read();
    }
    else
    {
      Wire.requestFrom((int)EepromAddress, (int)lastBlockBytes);
      while (!Wire.available()) 
        delayMicroseconds(IdleTimeUs);

      for(int j = 0; j < lastBlockBytes; ++j)
        data[i * RwBlockSize + j] = Wire.read();
    }
  }
}

void Eeprom::ReadSavedSettings(uint8_t* selected_pattern, uint8_t* selected_color2, uint8_t* selected_color4, uint16_t* selected_speed, uint16_t* selected_led_count)
{
  *selected_pattern = ReadByte(SelectedPatternAddress);
  *selected_color2 = ReadByte(Selected2ColorAddress);
  *selected_color4 = ReadByte(Selected4ColorAddress);
  *selected_speed = ReadUint16(SelectedSpeedAddress);
  *selected_led_count = ReadUint16(SelectedLedCount);
}

void Eeprom::SaveSettings(uint8_t selected_pattern, uint8_t selected_color2, uint8_t selected_color4, uint16_t selected_speed, uint16_t selected_led_count)
{
  WriteByte(SelectedPatternAddress, selected_pattern);
  WriteByte(Selected2ColorAddress, selected_color2);
  WriteByte(Selected4ColorAddress, selected_color4);
  WriteUint16(SelectedSpeedAddress, selected_speed);
  WriteUint16(SelectedLedCount, selected_led_count);
}

void Eeprom::ReadColor2Schema(uint8_t color_num, uint32_t* color_a, uint32_t* color_b)
{
  int address = Schemas2ColorAddress + color_num * 0x6;
  *color_a = ReadUint24(address);
  *color_b = ReadUint24(address + 0x3);
}

void Eeprom::ReadColor4Schema(uint8_t color_num, uint32_t* color_a, uint32_t* color_b, uint32_t* color_c, uint32_t* color_d)
{
  int address = Schemas4ColorAddress + color_num * 0xC;
  *color_a = ReadUint24(address);
  *color_b = ReadUint24(address + 0x3);
  *color_c = ReadUint24(address + 0x6);
  *color_d = ReadUint24(address + 0x9);
}

void Eeprom::SaveColor2Schema(uint8_t color_num, uint32_t color_a, uint32_t color_b)
{
  int address = Schemas2ColorAddress + color_num * 0x6;
  WriteUint24(address, color_a);
  WriteUint24(address + 0x3, color_b);
}
    
void Eeprom::SaveColor4Schema(uint8_t color_num, uint32_t color_a, uint32_t color_b, uint32_t color_c, uint32_t color_d)
{
  int address = Schemas4ColorAddress + color_num * 0xC;
  WriteUint24(address, color_a);
  WriteUint24(address + 0x3, color_b);
  WriteUint24(address + 0x6, color_c);
  WriteUint24(address + 0x9, color_d);
}
