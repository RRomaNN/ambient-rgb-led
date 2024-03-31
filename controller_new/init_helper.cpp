#include <stdint.h>
#include "init_helper.hpp"

void InitHelper::InitI2C()
{
  Wire.begin(); 
  Wire.setClock(I2cSpeed);
}

void InitHelper::InitDigitalOutput(uint8_t gpio, uint8_t init_state)
{
  pinMode(gpio, OUTPUT);
  digitalWrite(gpio, init_state); 
}

void InitHelper::InitGpioPins()
{
  pinMode(MODE_BUTTON_GPIO_PIN, INPUT);
  pinMode(SELECT_BUTTON_GPIO_PIN, INPUT);

  InitDigitalOutput(SYS_LED_GPIO_PIN, LOW);

  pinMode(CurrentMeterPin, INPUT);
}