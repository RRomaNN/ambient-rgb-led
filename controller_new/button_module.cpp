#include "button_module.hpp"

ButtonModule::ButtonModule()
{
  this->button_action = ButtonNone;
  this->clock_count = 0;
  this->active_button = NoneButton;
}

ButtonModule::ButtonAction ButtonModule::GetCurrentButtonAction()
{
  bool mode_button_pressed = analogRead(MODE_BUTTON_GPIO_PIN) > BUTTON_ADC_THRESHOLD ? false : true;
  bool select_button_pressed = analogRead(SELECT_BUTTON_GPIO_PIN) > BUTTON_ADC_THRESHOLD ? false : true;

  switch(button_action)
  {
    case ButtonNone:
      if (!mode_button_pressed && !select_button_pressed)
        return ButtonNone;
      else
      {
        active_button = mode_button_pressed ? ModeButton : SelectButton;
        button_action = ButtonPressed;        
        return button_action;
      }
      break;
    case ButtonPressed:
      if (mode_button_pressed || select_button_pressed)
      {
        ButtonType current_button = mode_button_pressed ? ModeButton : SelectButton;
        if (active_button == current_button)
        {
          if(clock_count < LONG_BUTTON_PRESS_MIN_COUNT)
          {
            clock_count++;
            return button_action;
          }
          else
          {
            //long press detected
            active_button = NoneButton;
            clock_count = 0;
            button_action = mode_button_pressed ? ButtonModeLong : ButtonSelectLong;
            return button_action;
          }
        }
        else 
        {
          //exceptional case when buttons were pressed simultaneously
          if (clock_count < SHORT_BUTTON_PRESS_MIN_COUNT)
          {
            //ignore the previous button, keep the state as is
            active_button = current_button;
            clock_count = 0;
            return button_action;
          }
          else 
          {
            //produce the event, start a new one
            ButtonAction action = active_button == ModeButton ? ButtonMode : ButtonSelect;  

            active_button = current_button;   
            clock_count = 0; 

            return action;
          }
        }
      }
      else 
      {
        if (clock_count < SHORT_BUTTON_PRESS_MIN_COUNT)
        {
          //nothing happened
          clock_count = 0;
          active_button = NoneButton;
          button_action = ButtonNone;

          return button_action;
        }
        else 
        {
          //short press detected
          ButtonAction action = active_button == ModeButton ? ButtonMode : ButtonSelect;  

          clock_count = 0;
          active_button = NoneButton;
          button_action = ButtonNone;

          return action;
        }
      }      
      break;
    case ButtonSelectLong:
    case ButtonModeLong:
      if (!mode_button_pressed && !select_button_pressed)
        button_action = ButtonNone;
      return ButtonNone;
      break;
  }

  return ButtonNone;
}