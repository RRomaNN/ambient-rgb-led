#ifndef __INIT_HELPER__
#define __INIT_HELPER__

#include "global.hpp"
#include "button_module.hpp"
#include "led_indicator.hpp"
#include "power_controller.hpp"

static const int32_t I2cSpeed = 400000;

class InitHelper
{
  public:
    static void InitI2C();
    static void InitGpioPins();

    InitHelper() = delete;

  private:
    static void InitDigitalOutput(uint8_t gpio, uint8_t init_state);
};

#endif
