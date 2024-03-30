#include "eeprom.hpp"

Eeprom::Eeprom()
{

}

byte Eeprom::Ping()
{
  Wire.beginTransmission(EepromAddress);
  Wire.write(0x00);
  return Wire.endTransmission();
}

byte Eeprom::ReadByte(int address)
{
  Wire.beginTransmission(EepromAddress);
  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));
  Wire.endTransmission();                  
  Wire.requestFrom(EepromAddress, 0x01);
  while (!Wire.available()) 
    delayMicroseconds(IdleTimeUs);
  return Wire.read();
}

void Eeprom::WriteData(int address, int size, byte* data)
{
  for (int i = 0; i < size / RwBlockSize; ++i)
  {
    int batchAddress = address + i * RwBlockSize;
    Wire.beginTransmission(EepromAddress);
    Wire.write((int)(batchAddress >> 8));
    Wire.write((int)(batchAddress & 0xFF));

    for(int j = 0; j < RwBlockSize; ++j)
      Wire.write(data[i * RwBlockSize + j]);

    Wire.endTransmission();  

    byte busStatus = 0xFF;
    do       
    {
      Wire.beginTransmission(EepromAddress);
      busStatus = Wire.endTransmission();
    }
    while(busStatus != 0x00);
  }
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
      Wire.requestFrom(EepromAddress, RwBlockSize);
      while (!Wire.available()) 
        delayMicroseconds(IdleTimeUs);

      for(int j = 0; j < RwBlockSize; ++j)
        data[i * RwBlockSize + j] = Wire.read();
    }
    else
    {
      Wire.requestFrom(EepromAddress, lastBlockBytes);
      while (!Wire.available()) 
        delayMicroseconds(IdleTimeUs);

      for(int j = 0; j < lastBlockBytes; ++j)
        data[i * RwBlockSize + j] = Wire.read();
    }
  }
}