#include "button_module.hpp"
#include "init_helper.hpp"
#include "state_machine.hpp"
#include "neopixel.hpp"
#include "lcd.hpp"
#include "eeprom.hpp"
#include "menu.hpp"

LPLedIndicator led_indicatior;
LPStateMachine state_machine;
LPButtonModule button_module;
LPEeprom eeprom;
LPPowerController pwr_controller;
LPNeoPixel neopixel;
LPLcd lcd;
LPMenu menu;

bool global_lock;

uint8_t selected_pattern;
uint8_t selected_color2;
uint8_t selected_color4;
uint16_t selected_speed;
uint16_t strip_led_count;

void setup() 
{
  global_lock = false;

  InitHelper::InitGpioPins();
  InitHelper::InitI2C();

  //Init objects...
  led_indicatior = new LedIndicator();
  eeprom = new Eeprom();
  lcd = new Lcd();
  button_module = new ButtonModule();
  pwr_controller = new PowerController(led_indicatior, &global_lock, neopixel, lcd);

  pwr_controller->AuditPowerStatusAndAct();

  if (eeprom->Ping())
  {
    lcd->PrintMemTestError();
    led_indicatior->Blink(3, LONG_SIGNAL_MS, LONG_SIGNAL_MS);
    global_lock = true;
  }
  else
    led_indicatior->Blink(3, SHORT_SIGNAL_MS, SHORT_SIGNAL_MS);

  //TODO: Load initial values from EEPROM here
  state_machine = new StateMachine(selected_pattern, selected_color2, selected_color4, selected_speed, strip_led_count);
  neopixel = new NeoPixel(strip_led_count);
  menu = new Menu(lcd, state_machine);
}

void loop() 
{
  if (global_lock)
  {
    delay(1000);
    return;
  }

  float current_amps = pwr_controller->AuditPowerStatusAndAct();
    menu->RenderCurrentState(current_amps, 0x7F010203, 0x0012FFED, 0x7F010203, 0x00AABBCD); //Fix me!!! Load from eeprom

  ButtonModule::ButtonAction action = button_module->GetCurrentButtonAction();
  if (action == ButtonModule::ButtonMode || action == ButtonModule::ButtonModeLong 
    || action == ButtonModule::ButtonSelect || action == ButtonModule::ButtonSelectLong)
  {
    //Convert enum to enum with shift
    StateMachine::ActionType action_type = (StateMachine::ActionType)((int)action - 2);
    state_machine->TransitState(action_type);
  }
    //StateMachine::ModeType mode_type = state_machine->GetCurrentMode();

    /*if (state_machine->IsModeActive())
    {
      switch(mode_type)
      {
        case StateMachine::ActiveMode:
          is_data_transfer_on = false;
          is_settings_transfer_on = false;
          rendering_engine->SetSequenceNum(selected_option);
          is_image_rendering_on = true;
          break;
        case StateMachine::LighterMode:
          is_data_transfer_on = false;
          is_settings_transfer_on = false;
          is_image_rendering_on = false;
          lighter->SetBrightness(selected_option * 0x30);
          break;
        case StateMachine::SettingsMode:
          is_image_rendering_on = false;
          is_data_transfer_on = false;
          is_settings_transfer_on = true;
          break;
        case StateMachine::PasswordMode:
          is_image_rendering_on = false;
          is_settings_transfer_on = false;
          is_data_transfer_on = true;
          break;
      }
      activatedMode = mode_type;
    }*/

  delayMicroseconds(IdleTimeUs);
}
