#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define IRQ0 2
#define IRQ1 3

#define LCD_RS 8
#define LCD_EN 11
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

#define LED_PREVIEW_ARRAY_PIN 9
#define LED_MAIN_ARRAY_PIN 10

#define LED_PREVIEW_ARRAY_PIXEL_NUM 2
#define LED_MAIN_ARRAY_PIXEL_NUM 186

#define LED_ARRAY_BRIGHTNESS 240 // Not 255

#define EEPROM0_ADDRESS 0x50
#define EEPROM1_ADDRESS 0x51

#define MAX_LED_NUM 225
#define MAX_IMG_LEN 225

#define PATTERN_MAX_COUNT_EEPROM0 4
#define PATTERN_MAX_INDEX 3 // Pattern count - 1

#define MODIFY_BUTTON_PIN A0
#define AMP_METER_PIN A1

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Adafruit_NeoPixel preview_led_array(LED_PREVIEW_ARRAY_PIXEL_NUM, LED_PREVIEW_ARRAY_PIN, NEO_GRB + NEO_KHZ800);

enum stateType {patternSelect, colorSelect, editMainRColor, editMainGColor, editMainBColor, editSecondaryRColor, editSecondaryGColor, editSecondaryBColor, validate, pause, resumeBack};
enum buttonType {selectButton, actionButton, modifyButton};

volatile enum stateType state = patternSelect; 

volatile int pattern_index = 0;
volatile int pattern_preview_line_offset = 0;

volatile char schema_name[11] = "N/A";

volatile char pattern_preview_line0[21];
volatile char pattern_preview_line1[21];
volatile char pattern_preview_line2[21];

volatile bool isSelectPressed = false;
volatile bool isActionPressed = false;

volatile byte main_red = 0xFF;
volatile byte main_green = 0xFF;
volatile byte main_blue = 0xFF;
volatile char main_color_hex[7];

volatile byte secondary_red = 0x00;
volatile byte secondary_green = 0x00;
volatile byte secondary_blue = 0x00;
volatile char secondary_color_hex[7];

volatile bool color_preview = false;
volatile int color_modify_reading_count = 0;
volatile int color_up_momentum = 0;
volatile int color_down_momentum = 0;
volatile byte color_index = 0;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  cli();
 
  pinMode(IRQ0, INPUT);
  pinMode(IRQ1, INPUT);
 
  attachInterrupt(0, selectInterrupt, FALLING);    
  attachInterrupt(1, actionInterrupt, FALLING);  

  sei();

  Wire.begin();
  
  lcd.begin(20, 4);
  lcd.clear();

  preview_led_array.begin();
  preview_led_array.setBrightness(LED_ARRAY_BRIGHTNESS);

  readCurrentSchemaName(pattern_index);
  readCurrentPatternPreview(pattern_index, pattern_preview_line_offset);
}

void loop() 
{
  if(isSelectPressed)
  {
    transitState(selectButton);
    isSelectPressed = false;
  }

  if(isActionPressed)
  {
    transitState(actionButton);
    isActionPressed = false;
  }

  processModifyButton();
  if(color_modify_reading_count == 10)
  {
    color_modify_reading_count = 0;
    transitState(modifyButton);
  }

  render();

  if(state == patternSelect)
    preview();

  if((state == patternSelect || state == validate || state == pause || state == resumeBack) && color_preview)
  {
    color_preview = false;
    cancelPreviewColors();
  }    

  delay(100);
}

void processModifyButton()
{
  int modify_button_reading = analogRead(MODIFY_BUTTON_PIN);
  if(modify_button_reading < 256)
    color_modify_reading_count++;
  else
    color_modify_reading_count = 0;
}

void preview()
{
  pattern_preview_line_offset = pattern_preview_line_offset == MAX_IMG_LEN - 1 ? 0 : pattern_preview_line_offset + 1;
  readCurrentPatternPreview(pattern_index, pattern_preview_line_offset);
}

void selectInterrupt()
{
  if(!isSelectPressed)
    isSelectPressed = true;
}

void actionInterrupt()
{
  if(!isActionPressed)
    isActionPressed = true;
}

void transitState(enum buttonType button)
{
  switch(state)
  {
    case patternSelect:
      switch(button)
      {
        case selectButton:
          pattern_index = pattern_index == PATTERN_MAX_INDEX ? 0 : pattern_index + 1;
          readCurrentSchemaName(pattern_index);    
          readCurrentPatternPreview(pattern_index, pattern_preview_line_offset);
          break;
        case actionButton:
          state = colorSelect;
          readColors();
          break;
      }
      break;
    case colorSelect:
      switch(button)
      {
        case selectButton:
          color_index = color_index == 63 ? 0 : color_index + 1;
          readColors();
          break;
        case actionButton:
          state = validate;
          break;
        case modifyButton:
          state = editMainRColor;
          break;
      }
      break;
    case editMainRColor:
      switch(button)
      {
        case selectButton:
          main_red = colorDown(main_red);
          break;
        case actionButton:
          main_red = colorUp(main_red);
          break;
        case modifyButton:
          state = editMainGColor;
          break;
      }
      break;
    case editMainGColor:
      switch(button)
      {
        case selectButton:
          main_green = colorDown(main_green);
          break;
        case actionButton:
          main_green = colorUp(main_green);
          break;
        case modifyButton:
          state = editMainBColor;
          break;
      }
      break;
    case editMainBColor:
      switch(button)
      {
        case selectButton:
          main_blue = colorDown(main_blue);
          break;
        case actionButton:
          main_blue = colorUp(main_blue);
          break;
        case modifyButton:
          state = editSecondaryRColor;
          break;
      }
      break;
    case editSecondaryRColor:
      switch(button)
      {
        case selectButton:
          secondary_red = colorDown(secondary_red);
          break;
        case actionButton:
          secondary_red = colorUp(secondary_red);
          break;
        case modifyButton:
          state = editSecondaryGColor;
          break;
      }
      break;
    case editSecondaryGColor:
      switch(button)
      {
        case selectButton:
          secondary_green = colorDown(secondary_green);
          break;
        case actionButton:
          secondary_green = colorUp(secondary_green);
          break;
        case modifyButton:
          state = editSecondaryBColor;
          break;
      }
      break;
    case editSecondaryBColor:
      switch(button)
      {
        case selectButton:
          secondary_blue = colorDown(secondary_blue);
          break;
        case actionButton:
          secondary_blue = colorUp(secondary_blue);
          break;
        case modifyButton:
          state = colorSelect;
          writeColors();
          break;
      }
      break;
    case validate:
      switch(button)
      {
        case selectButton:
          //go ahead
          state = pause;
          break;
        case actionButton:
          //return back
          state = patternSelect;
          break;
      }
      break;
    case pause:
      switch(button)
      {
        case selectButton:
          //go ahead
          state = validate;
          break;
        case actionButton:
          //return back
          state = patternSelect;
          break;
      }
      break;
  }
}

byte colorUp(byte color)
{
  color_up_momentum++;
  color_down_momentum = 0;
  return color = color == 0xFF ? 0x00 : color + color_up_momentum;
}

byte colorDown(byte color)
{
  color_down_momentum++;
  color_up_momentum = 0;
  return color = color == 0x00 ? 0xFF : color - color_down_momentum;
}

void render() 
{
  lcd.setCursor(0, 0);
  
  switch(state)
  {
    case patternSelect:
      lcd.print((char*)pattern_preview_line0);
      lcd.setCursor(0, 1);
      lcd.print((char*)pattern_preview_line1);
      lcd.setCursor(0, 2);
      lcd.print((char*)pattern_preview_line2);
      lcd.setCursor(0, 3);
      lcd.print((char*)schema_name);
      printIterationNumber();
      lcd.print(" Next");
      break;
    case colorSelect:
      setColorStrings();
      previewColors();
      lcd.print("Choose colors   [");
      printNumber(color_index);
      lcd.print("]");
      displayColorStrings();
      renderChoice("Select  Modify  Next");
      break;
    case editMainRColor:
      setColorStrings();
      previewColors();
      lcd.print("Set main red        ");
      displayColorStrings();
      renderChoice("-    Main green    +");
      break;
    case editMainGColor:
      setColorStrings();
      previewColors();
      lcd.print("Set main green      ");
      displayColorStrings();
      renderChoice("-    Main blue     +");    
      break;
    case editMainBColor:
      setColorStrings();
      previewColors();
      lcd.print("Set main blue       ");
      displayColorStrings();
      renderChoice("-  Secondary red   +");    
      break;
    case editSecondaryRColor:
      setColorStrings();
      previewColors();
      lcd.print("Set secondary red   ");
      displayColorStrings();
      renderChoice("- Secondary green  +");
      break;
    case editSecondaryGColor:
      setColorStrings();
      previewColors();
      lcd.print("Set secondary green ");
      displayColorStrings();
      renderChoice("-  Secondary blue  +");    
      break;
    case editSecondaryBColor:
      setColorStrings();
      previewColors();
      lcd.print("Set secondary blue  ");
      displayColorStrings();
      renderChoice("-      Apply       +");    
      break;
    case validate:
      lcd.print("PATTERN: ");
      lcd.print((char*)schema_name);
      lcd.setCursor(0, 1);
      lcd.print("RGB1: ");
      lcd.print("ABCDEF");
      lcd.setCursor(0, 2);
      lcd.print("RGB2: ");
      lcd.print("789ABC");
      renderChoice("APPLY         CANCEL");
      break;
    case pause:
      lcd.print("PATTERN: ");
      lcd.print((char*)schema_name);
      lcd.setCursor(0, 1);
      lcd.print("ITERATION: ");
      lcd.print(123);
      lcd.setCursor(0, 2);
      lcd.print("CURRENT: ");
      lcd.print("3.5A");
      renderChoice("PAUSE            OFF");
      break;
    case resumeBack:
      lcd.print("PATTERN: ");
      lcd.print((char*)schema_name);
      lcd.setCursor(0, 1);
      lcd.print("ITERATION: ");
      lcd.print(123);
      lcd.setCursor(0, 2);
      lcd.print("CURRENT: ");
      lcd.print("3.5A");
      renderChoice("RESUME           OFF");
      break;
  } 
}

void printNumber(byte number)
{
  if(number < 10)
    lcd.print(" ");
  lcd.print(number);
}

void displayColorStrings()
{
  lcd.setCursor(0, 1);
  lcd.print("RGB M: ");
  lcd.print((char*)main_color_hex);
  lcd.print("       ");
  lcd.setCursor(0, 2);
  lcd.print("RGB S: ");
  lcd.print((char*)secondary_color_hex);
  lcd.print("       ");
}

void previewColors()
{
  preview_led_array.setPixelColor(0, preview_led_array.Color(main_red, main_green, main_blue));
  preview_led_array.setPixelColor(1, preview_led_array.Color(secondary_red, secondary_green, secondary_blue));
  preview_led_array.show();
  color_preview = true;
}

void cancelPreviewColors()
{
  preview_led_array.setPixelColor(0, preview_led_array.Color(0x00, 0x00, 0x00));
  preview_led_array.setPixelColor(1, preview_led_array.Color(0x00, 0x00, 0x00));
  preview_led_array.show();
}

void readColors()
{
  int baseAddress = PATTERN_MAX_COUNT_EEPROM0 * 32 * (MAX_IMG_LEN + 1) + 6 * color_index; // 4 x schema + RGBRGB x index
  main_red = readEEPROM(EEPROM0_ADDRESS, baseAddress++);
  main_green = readEEPROM(EEPROM0_ADDRESS, baseAddress++);
  main_blue = readEEPROM(EEPROM0_ADDRESS, baseAddress++);
  secondary_red = readEEPROM(EEPROM0_ADDRESS, baseAddress++);
  secondary_green = readEEPROM(EEPROM0_ADDRESS, baseAddress++);
  secondary_blue = readEEPROM(EEPROM0_ADDRESS, baseAddress++);
}

void writeColors()
{
  int baseAddress = PATTERN_MAX_COUNT_EEPROM0 * 32 * (MAX_IMG_LEN + 1) + 6 * color_index; // see above ^^
  
  writeEEPROM(EEPROM0_ADDRESS, baseAddress++, main_red);
  writeEEPROM(EEPROM0_ADDRESS, baseAddress++, main_green);
  writeEEPROM(EEPROM0_ADDRESS, baseAddress++, main_blue);
  writeEEPROM(EEPROM0_ADDRESS, baseAddress++, secondary_red);
  writeEEPROM(EEPROM0_ADDRESS, baseAddress++, secondary_green);
  writeEEPROM(EEPROM0_ADDRESS, baseAddress++, secondary_blue);
}

void printIterationNumber()
{
  if(pattern_preview_line_offset < 10) 
  {
    lcd.print("   ");
    lcd.print(pattern_preview_line_offset);
  } 
  else if(pattern_preview_line_offset < 100)
  {
    lcd.print("  ");
    lcd.print(pattern_preview_line_offset);
  }
  else
  {
    lcd.print(" ");
    lcd.print(pattern_preview_line_offset);
  }
}

void setColorStrings()
{  
  byte color_main_bytes[3] = {main_red, main_green, main_blue};
  byte color_secondary_bytes[3] = {secondary_red, secondary_green, secondary_blue};
  arrayToString(color_main_bytes, 3, main_color_hex);
  arrayToString(color_secondary_bytes, 3, secondary_color_hex);
}

void arrayToString(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len*2] = '\0';
}

void renderChoice(char* bottomLine)
{
  lcd.setCursor(0, 3);
  lcd.print(bottomLine);
}

void readCurrentSchemaName(int index)
{
  int nameBaseAddress = index * 32 * (MAX_IMG_LEN + 1); //32 (256/8) * (225 + 1 for header);
  for(int i = 0; i < 11; i++)
    schema_name[i] = (char)readEEPROM(EEPROM0_ADDRESS, nameBaseAddress + i);
}

void readCurrentPatternPreview(int index, int lineOffset)
{
  int patternBaseAddress = index * 32 * (MAX_IMG_LEN + 1);
  int patternAddress = patternBaseAddress + 32 * (1 + lineOffset); //32 (256/8) * (225 + 1 for header) + header row;
  for(int i = 0; i < 20; i++)
    pattern_preview_line0[i] = readPatternLineChar(patternAddress, i);
  pattern_preview_line0[20] = '\0';
  patternAddress = patternBaseAddress + 32 * (1 + (lineOffset + 1) % MAX_IMG_LEN);
  for(int i = 0; i < 20; i++)
    pattern_preview_line1[i] = readPatternLineChar(patternAddress, i);
  pattern_preview_line1[20] = '\0';
  patternAddress = patternBaseAddress + 32 * (1 + (lineOffset + 2) % MAX_IMG_LEN);
  for(int i = 0; i < 20; i++)
    pattern_preview_line2[i] = readPatternLineChar(patternAddress, i);
  pattern_preview_line2[20] = '\0';
}

byte readPatternLineChar(int patternBaseAddress, int bitIndex)
{
  return (readEEPROM(EEPROM0_ADDRESS, (patternBaseAddress + bitIndex/8)) & (1 << bitIndex % 8)) ? char(255) : ' ';
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

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(10);
}
