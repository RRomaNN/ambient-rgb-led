#include "rendering_engine.hpp"

RendringEngine::RendringEngine(LPStateMachine state_machine, LPEeprom eeprom, LPNeoPixel neopixel)
{
  current_counter = 0;
  current_row = 0;

  this->state_machine = state_machine;
  this->eeprom = eeprom;
  this->neopixel = neopixel;
}

inline uint8_t RendringEngine::GetWhiteComponent(uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t rb = red < green ? red : green;
  return rb < blue ? rb : blue;
}

inline void RendringEngine::GetColorComponents(uint8_t* red, uint8_t* green, uint8_t* blue, uint32_t color)
{
  *red = color & (uint32_t)0x000000FF;
  *green = (color & (uint32_t)0x0000FF00) >> 8;
  *blue = (color & (uint32_t)0x00FF0000) >> 16;
}

inline void RendringEngine::Render2ColorsRgbw(uint8_t next_row, uint8_t led_count, float newFraction, float oldFraction)
{
  uint32_t colors[2];
  state_machine->GetSelected2Colors(colors, colors + 1);

  for (uint8_t block_number = 0; block_number < 2; ++block_number)
  {
    uint8_t current_row_data[RwBlockSize];
    uint8_t next_row_data[RwBlockSize];
    eeprom->ReadImageBlock(0, current_row, block_number, current_row_data);
    eeprom->ReadImageBlock(0, next_row, block_number, next_row_data);

    for (uint8_t led_index = block_number * PixelsPer2ColorBlock, i = 0; led_index < led_count && i < PixelsPer2ColorBlock; ++led_index, ++i)
    {
      uint8_t red, green, blue, white;
      uint8_t offset = i % 8;
      uint8_t index = i / 8;
      uint8_t mask = 0x1 << offset;
      uint8_t currentValue = (current_row_data[index] & mask) > 0 ? 1 : 0;
      uint8_t nextValue = (next_row_data[index] & mask) > 0 ? 1 : 0;

      if (currentValue == nextValue)
      {
        GetColorComponents(&red, &green, &blue, colors[currentValue]);
        white = GetWhiteComponent(red, green, blue);
        neopixel->SetNeoPixel4Color(led_index, red - white, green - white, blue - white, white);  
      }
      else
      {
        uint8_t red_a, green_a, blue_a;
        uint8_t red_b, green_b, blue_b;
        GetColorComponents(&red_a, &green_a, &blue_a, colors[currentValue]);
        GetColorComponents(&red_b, &green_b, &blue_b, colors[nextValue]);

        red = red_b * newFraction + red_a * oldFraction;
        green = green_b * newFraction + green_a * oldFraction;
        blue = blue_b * newFraction + blue_a * oldFraction; 
        white = GetWhiteComponent(red, green, blue);
        neopixel->SetNeoPixel4Color(led_index, red - white, green - white, blue - white, white);  
      }
    }
  }
  neopixel->Display();
}

inline void RendringEngine::Render4ColorsRgbw(uint8_t next_row, uint8_t led_count, float newFraction, float oldFraction)
{
  uint32_t colors[4];
  state_machine->GetSelected4Colors(colors, colors + 1, colors + 2, colors + 3);

  for (uint8_t block_number = 0; block_number < 4; ++block_number)
  {
    uint8_t current_row_data[RwBlockSize];
    uint8_t next_row_data[RwBlockSize];
    eeprom->ReadImageBlock(1, current_row, block_number, current_row_data);
    eeprom->ReadImageBlock(1, next_row, block_number, next_row_data);

    for (uint8_t led_index = block_number * PixelsPer4ColorBlock, i = 0; led_index < led_count && i < PixelsPer2ColorBlock; ++led_index, ++i)
    {
      uint8_t red, green, blue, white;
      uint8_t offset = (i * 2) % 8;
      uint8_t index = i / 4;
      uint8_t mask = 0x3 << offset;
      uint8_t currentValue = (current_row_data[index] & mask) >> offset;
      uint8_t nextValue = (next_row_data[index] & mask) >> offset;

      if (currentValue == nextValue)
      {
        GetColorComponents(&red, &green, &blue, colors[currentValue]);
        white = GetWhiteComponent(red, green, blue);
        neopixel->SetNeoPixel4Color(led_index, red - white, green - white, blue - white, white);  
      }
      else
      {
        uint8_t red_a, green_a, blue_a;
        uint8_t red_b, green_b, blue_b;
        GetColorComponents(&red_a, &green_a, &blue_a, colors[currentValue]);
        GetColorComponents(&red_b, &green_b, &blue_b, colors[nextValue]);

        red = red_b * newFraction + red_a * oldFraction;
        green = green_b * newFraction + green_a * oldFraction;
        blue = blue_b * newFraction + blue_a * oldFraction; 
        white = GetWhiteComponent(red, green, blue);
        neopixel->SetNeoPixel4Color(led_index, red - white, green - white, blue - white, white);  
      }
    }
  }
  neopixel->Display();
}

inline void RendringEngine::Render2ColorsRgb(uint8_t next_row, uint8_t led_count, float newFraction, float oldFraction)
{
  uint32_t colors[2];
  state_machine->GetSelected2Colors(colors, colors + 1);

  for (uint8_t block_number = 0; block_number < 2; ++block_number)
  {
    uint8_t current_row_data[RwBlockSize];
    uint8_t next_row_data[RwBlockSize];
    eeprom->ReadImageBlock(0, current_row, block_number, current_row_data);
    eeprom->ReadImageBlock(0, next_row, block_number, next_row_data);

    for (uint8_t led_index = block_number * PixelsPer2ColorBlock, i = 0; led_index < led_count && i < PixelsPer2ColorBlock; ++led_index, ++i)
    {
      uint8_t red, green, blue;
      uint8_t offset = i % 8;
      uint8_t index = i / 8;
      uint8_t mask = 0x1 << offset;
      uint8_t currentValue = (current_row_data[index] & mask) > 0 ? 1 : 0;
      uint8_t nextValue = (next_row_data[index] & mask) > 0 ? 1 : 0;

      if (currentValue == nextValue)
      {
        GetColorComponents(&red, &green, &blue, colors[currentValue]);
        neopixel->SetNeoPixel3Color(led_index, red, green, blue);  
      }
      else
      {
        uint8_t red_a, green_a, blue_a;
        uint8_t red_b, green_b, blue_b;
        GetColorComponents(&red_a, &green_a, &blue_a, colors[currentValue]);
        GetColorComponents(&red_b, &green_b, &blue_b, colors[nextValue]);

        red = red_b * newFraction + red_a * oldFraction;
        green = green_b * newFraction + green_a * oldFraction;
        blue = blue_b * newFraction + blue_a * oldFraction; 
        neopixel->SetNeoPixel3Color(led_index, red, green, blue);  
      }
    }
  }
  neopixel->Display();
}

inline void RendringEngine::Render4ColorsRgb(uint8_t next_row, uint8_t led_count, float newFraction, float oldFraction)
{
  uint32_t colors[4];
  state_machine->GetSelected4Colors(colors, colors + 1, colors + 2, colors + 3);

  for (uint8_t block_number = 0; block_number < 4; ++block_number)
  {
    uint8_t current_row_data[RwBlockSize];
    uint8_t next_row_data[RwBlockSize];
    eeprom->ReadImageBlock(1, current_row, block_number, current_row_data);
    eeprom->ReadImageBlock(1, next_row, block_number, next_row_data);

    for (uint8_t led_index = block_number * PixelsPer4ColorBlock, i = 0; led_index < led_count && i < PixelsPer2ColorBlock; ++led_index, ++i)
    {
      uint8_t red, green, blue;
      uint8_t offset = (i * 2) % 8;
      uint8_t index = i / 4;
      uint8_t mask = 0x3 << offset;
      uint8_t currentValue = (current_row_data[index] & mask) >> offset;
      uint8_t nextValue = (next_row_data[index] & mask) >> offset;

      if (currentValue == nextValue)
      {
        GetColorComponents(&red, &green, &blue, colors[currentValue]);
        neopixel->SetNeoPixel3Color(led_index, red, green, blue);  
      }
      else
      {
        uint8_t red_a, green_a, blue_a;
        uint8_t red_b, green_b, blue_b;
        GetColorComponents(&red_a, &green_a, &blue_a, colors[currentValue]);
        GetColorComponents(&red_b, &green_b, &blue_b, colors[nextValue]);

        red = red_b * newFraction + red_a * oldFraction;
        green = green_b * newFraction + green_a * oldFraction;
        blue = blue_b * newFraction + blue_a * oldFraction; 
        neopixel->SetNeoPixel3Color(led_index, red, green, blue);  
      }
    }
  }
  neopixel->Display();
}

void RendringEngine::Render()
{
  uint16_t speed = state_machine->GetLogarithmicSpeed();
  uint16_t led_count = state_machine->GetSelectedLedCount();
  uint8_t next_row = (current_row == ImageRowCount - 1) ? 0x00 : current_row + 1;
  float newFraction = (float)current_counter / speed;
  float oldFraction = 1 - newFraction;
  
  if (state_machine->IsRgbwStrip())
  {
    if (state_machine->GetSelectedPattern() == 0)
      Render2ColorsRgbw(next_row, led_count, newFraction, oldFraction);
    else 
      Render4ColorsRgbw(next_row, led_count, newFraction, oldFraction);
  }
  else
  {
    if (state_machine->GetSelectedPattern() == 0)
      Render2ColorsRgb(next_row, led_count, newFraction, oldFraction);
    else 
      Render4ColorsRgb(next_row, led_count, newFraction, oldFraction);   
  }

  current_counter = (current_counter >= speed) ? 0 : current_counter + 1;
  if (current_counter == 0)
    current_row = (current_row == ImageRowCount - 1) ? 0x00 : current_row + 1;
}
