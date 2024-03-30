#ifndef __COMMAND_PROCESSOR__
#define __COMMAND_PROCESSOR__

#include "state_machine.hpp"
#include "eeprom.hpp"

class CommandProcessor
{
  public:
    CommandProcessor(LPStateMachine state_machine, LPEeprom eeprom);
    void ProcessStateChanges();

  private:
    void SaveSelection(StateMachine::ModeType current_mode);
    void InitialColorSelection(StateMachine::ModeType current_mode);
    void SaveColor(StateMachine::ModeType current_mode);
    void ColorReSelection(StateMachine::ModeType current_mode);

    LPStateMachine state_machine;
    LPEeprom eeprom;

    StateMachine::ModeType previous_mode;
    uint8_t previous_selected_color2;
    uint8_t previous_selected_color4;
};

#define LPCommandProcessor CommandProcessor*

#endif