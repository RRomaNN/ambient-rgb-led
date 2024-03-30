#ifndef __BUTTON_MODULE__
#define __BUTTON_MODULE__

#define SELECT_BUTTON_GPIO_PIN         A2
#define MODE_BUTTON_GPIO_PIN           A3

#define SHORT_BUTTON_PRESS_MIN_COUNT    3
#define LONG_BUTTON_PRESS_MIN_COUNT   500

#define BUTTON_ADC_THRESHOLD          512

#include "global.hpp"

class ButtonModule
{
  public:
    ButtonModule();

    enum ButtonAction 
    {
      ButtonNone = 0, 
      ButtonPressed = 1, 
      ButtonSelect = 2,
      ButtonSelectLong = 3,
      ButtonMode = 4,
      ButtonModeLong = 5
    };

    ButtonAction GetCurrentButtonAction();

  private:
    enum ButtonType 
    {
      NoneButton = 0,
      SelectButton = 1,
      ModeButton = 2
    };

    enum ButtonAction button_action;

    //How long the button has been pressed
    int clock_count;
    enum ButtonType active_button;
};

#define LPButtonModule ButtonModule*

#endif