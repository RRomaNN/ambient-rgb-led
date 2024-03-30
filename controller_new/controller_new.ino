#include "button_module.hpp"
#include "init_helper.hpp"
#include "neopixel.hpp"
#include "lcd.hpp"
#include "menu.hpp"
#include "command_processor.hpp"

LPLedIndicator led_indicatior;
LPStateMachine state_machine;
LPButtonModule button_module;
LPEeprom eeprom;
LPPowerController pwr_controller;
LPNeoPixel neopixel;
LPLcd lcd;
LPMenu menu;
LPCommandProcessor command_processor;

bool global_lock;

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

  uint8_t selected_pattern, selected_color2, selected_color4;
  uint16_t selected_speed, strip_led_count;
  eeprom->ReadSavedSettings(&selected_pattern, &selected_color2, &selected_color4, &selected_speed, &strip_led_count);

  state_machine = new StateMachine(selected_pattern, selected_color2, selected_color4, selected_speed, strip_led_count);
  command_processor = new CommandProcessor(state_machine, eeprom);
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

  command_processor->ProcessStateChanges();

  float current_amps = pwr_controller->AuditPowerStatusAndAct();
  menu->RenderCurrentState(current_amps);

  ButtonModule::ButtonAction action = button_module->GetCurrentButtonAction();
  if (action == ButtonModule::ButtonMode || action == ButtonModule::ButtonModeLong 
    || action == ButtonModule::ButtonSelect || action == ButtonModule::ButtonSelectLong)
  {
    //Convert enum to enum with shift
    StateMachine::ActionType action_type = (StateMachine::ActionType)((int)action - 2);
    state_machine->TransitState(action_type);
  }

  delayMicroseconds(IdleTimeUs);
}
