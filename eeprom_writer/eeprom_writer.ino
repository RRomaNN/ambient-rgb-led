#include <Wire.h>
#include <LiquidCrystal.h>

#define LCD_RS 8
#define LCD_EN 11
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

#define EEPROM0_ADDRESS 0x50

#define MAX_LED_NUM 225
#define MAX_IMG_LEN 225

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

String command;
volatile int writeIndex = 0x0;
volatile int schemaIndex = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

  lcd.begin(20, 4);
  lcd.clear();
}

void loop() {

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    lcd.setCursor(0, 0);
    lcd.print(command.substring(0, 19));
    if(writeIndex == MAX_IMG_LEN + 1)
    {
      schemaIndex++;
      writeIndex = 0;
    }
    lcd.setCursor(0, 1);
    lcd.print(writeIndex);
    lcd.print(" ");
    lcd.print(schemaIndex);
    saveSchema(schemaIndex, writeIndex++, command);
  }
  delay(50);
}

void saveSchema(int schemaIndex, int schemaRow, String row)
{  
  int baseAddress = schemaIndex * 32 * (MAX_IMG_LEN + 1); //32 (256/8) * (225 + 1 for header);
  if(schemaRow == 0) //Schema header
    for(int i = 0; i < 20; i++)
    {
      writeRow(2, "value ");
      lcd.print((byte)row[i]);     
      lcd.print(" ");
      lcd.print(baseAddress + i);           
      writeEEPROM(EEPROM0_ADDRESS, baseAddress + i, (byte)row[i]);
      byte readValue = readEEPROM(EEPROM0_ADDRESS, baseAddress + i);
      if(readValue != (byte)row[i])
        writeRow(3, "VALIDATION ERROR");
    }
  else
  {
    byte line[32];
    memset(line, 0, sizeof(line));
    for(int i = 0; i < MAX_LED_NUM; i++)
    {
      unsigned int color = row[i] == '0' ? 0 : 1; 
      line[i/8] = line[i/8] | color << i % 8;
    }
    baseAddress += 32 * schemaRow; 
    for(int i = 0; i < 32; i++)
    {
      writeRow(2, "A "); 
      lcd.print(" ");
      lcd.print(baseAddress + i);   
      lcd.print("   ");  
      writeEEPROM(EEPROM0_ADDRESS, baseAddress + i, (byte)line[i]);
      byte readValue = readEEPROM(EEPROM0_ADDRESS, baseAddress + i);
      if(readValue != (byte)line[i])
        writeRow(3, "VALIDATION ERROR");
    }
  } 
}

void writeRow(int rownum, String text)
{
  lcd.setCursor(0, rownum);
  lcd.print(text);
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(10);
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress) 
{
  byte rdata = 0x00;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) 
    rdata = Wire.read();

  return rdata;
}
