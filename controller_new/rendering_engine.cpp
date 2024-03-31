#include "rendering_engine.hpp"

RendringEngine::RendringEngine(LPStateMachine state_machine, LPEeprom eeprom, LPNeoPixel neopixel)
{
  current_counter = 0;
  current_row = 0;

  this->state_machine = state_machine;
  this->eeprom = eeprom;
  this->neopixel = neopixel;
}

uint8_t RendringEngine::GetWhiteComponent(uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t rb = red < green ? red : green;
  return rb < blue ? rb : blue;
}

void RendringEngine::GetColorComponents(uint8_t* red, uint8_t* green, uint8_t* blue, uint32_t color)
{
  *red = color & (uint32_t)0x000000FF;
  *green = (color & (uint32_t)0x0000FF00) >> 8;
  *blue = (color & (uint32_t)0x00FF0000) >> 16;
}

void RendringEngine::Render()
{
  uint8_t pattern = state_machine->GetSelectedPattern();
  uint16_t speed = state_machine->GetSelectedSpeed();
  uint16_t led_count = state_machine->GetSelectedLedCount();
  uint8_t next_row = (current_row == ImageRowCount - 1) ? 0x00 : current_row + 1;
  float newFraction = (float)current_counter / speed;
  float oldFraction = 1 - newFraction;

  uint32_t colors[4];
  state_machine->GetSelectedColors(colors, colors + 1, colors + 2, colors + 3);

  if (pattern == 0)
  {
    for (uint8_t block_number = 0; block_number < 2; ++block_number)
    {
      uint8_t current_row_data[RwBlockSize];
      uint8_t next_row_data[RwBlockSize];
      eeprom->ReadImageBlock(pattern, current_row, block_number, current_row_data);
      eeprom->ReadImageBlock(pattern, next_row, block_number, next_row_data);

      for (uint8_t led_index = block_number * PixelsPer2ColorBlock, i = 0; led_index < led_count && i < PixelsPer2ColorBlock; ++led_index, ++i)
      {
        uint8_t red, green, blue, white;
        uint8_t currentValue = (current_row_data[i / 8] & (1 << i % 8)) > 0 ? 1 : 0;
        uint8_t nextValue = (next_row_data[i / 8] & (1 << i % 8)) > 0 ? 1 : 0;

        if (currentValue == nextValue)
        {
          GetColorComponents(&red, &green, &blue, colors[currentValue]);
          white = GetWhiteComponent(red, green, blue);
          neopixel->SetNeoPixelColor(led_index, red - white, green - white, blue - white, white);  
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
          neopixel->SetNeoPixelColor(led_index, red - white, green - white, blue - white, white);  
        }
      }
    }
    neopixel->Display();
  }
  else 
  {
    for (uint8_t block_number = 0; block_number < 4; ++block_number)
    {
      uint8_t current_row_data[RwBlockSize];
      uint8_t next_row_data[RwBlockSize];
      eeprom->ReadImageBlock(pattern, current_row, block_number, current_row_data);
      eeprom->ReadImageBlock(pattern, next_row, block_number, next_row_data);

      for (uint8_t led_index = block_number * PixelsPer4ColorBlock, i = 0; led_index < led_count && i < PixelsPer2ColorBlock; ++led_index, ++i)
      {
        uint8_t red, green, blue, white;
        uint8_t currentValue = (current_row_data[i / 4] & (0x3 << (i * 2) % 8)) >> ((i * 2) % 8);
        uint8_t nextValue = (next_row_data[i / 4] & (0x3 << (i * 2) % 8)) >> ((i * 2) % 8);

        if (currentValue == nextValue)
        {
          GetColorComponents(&red, &green, &blue, colors[currentValue]);
          white = GetWhiteComponent(red, green, blue);
          neopixel->SetNeoPixelColor(led_index, red - white, green - white, blue - white, white);  
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
          neopixel->SetNeoPixelColor(led_index, red - white, green - white, blue - white, white);  
        }
      }
    }
    neopixel->Display();
  }

  current_counter = (current_counter == speed) ? 0 : current_counter + 1;
  if (current_counter == 0)
    current_row = (current_row == ImageRowCount - 1) ? 0x00 : current_row + 1;
}

void RendringEngine::InitRendering()
{
  //current_counter = 0;
  //current_row = 0;
}