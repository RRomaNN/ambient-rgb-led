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
uint8_t led_count;

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
  state_machine = new StateMachine(selected_pattern, selected_color2, selected_color4, selected_speed, led_count);
  neopixel = new NeoPixel(led_count);
  menu = new Menu(lcd, state_machine);
}

void loop() 
{
  if (global_lock)
  {
    delay(1000);
    return;
  }

  delay(1000);
  led_indicatior->Blink(3, 50, 50);


  float current_amps = pwr_controller->AuditPowerStatusAndAct();

  delay(1000);
  led_indicatior->Blink(3, 50, 50);

  menu->RenderCurrentState(current_amps);

  delay(1000);
  led_indicatior->Blink(3, 50, 50);

  delayMicroseconds(IdleTimeUs);
}
