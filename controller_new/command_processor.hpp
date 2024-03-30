#ifndef __COMMAND_PROCESSOR__
#define __COMMAND_PROCESSOR__

#include "state_machine.hpp"
#include "eeprom.hpp"
#include "rendering_engine.hpp"

class CommandProcessor
{
  public:
    CommandProcessor(LPStateMachine state_machine, LPEeprom eeprom, LPRendringEngine rendering_engine);
    void ProcessStateChanges(bool just_started);

  private:
    void SaveSelection(StateMachine::ModeType current_mode);
    void InitialColorSelection(StateMachine::ModeType current_mode);
    void SaveColor(StateMachine::ModeType current_mode);
    void ColorReSelection(StateMachine::ModeType current_mode);
    void InitRendering(StateMachine::ModeType current_mode);

    LPStateMachine state_machine;
    LPEeprom eeprom;
    LPRendringEngine rendering_engine;

    StateMachine::ModeType previous_mode;
    uint8_t previous_selected_color2;
    uint8_t previous_selected_color4;
};

#define LPCommandProcessor CommandProcessor*

#endif