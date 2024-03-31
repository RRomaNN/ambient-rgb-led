#include "command_processor.hpp"

CommandProcessor::CommandProcessor(LPStateMachine state_machine, LPEeprom eeprom, LPRendringEngine rendering_engine, LPLedIndicator led_indicatior)
{
  this->state_machine = state_machine;
  this->eeprom = eeprom;
  this->rendering_engine = rendering_engine;
  this->led_indicatior = led_indicatior;

  previous_mode = StateMachine::BackgroundMode;
}

void CommandProcessor::SaveSelection(StateMachine::ModeType current_mode)
{
  if (current_mode == StateMachine::BackgroundMode && (previous_mode == StateMachine::SpeedSelectMode || previous_mode == StateMachine::PreviewColorMode)
    || current_mode == StateMachine::RestartNeededMode && previous_mode == StateMachine::SelectStripTypeMode)
  {
    eeprom->SaveSettings(
      state_machine->GetSelectedPattern(), 
      state_machine->GetSelectedColor2Option(), 
      state_machine->GetSelectedColor4Option(), 
      state_machine->GetSelectedSpeed(), 
      state_machine->GetSelectedLedCount(),
      state_machine->GetPreviewColorMode(),
      state_machine->IsRgbwStrip());
      
    led_indicatior->Blink(2, SHORT_SIGNAL_MS, SHORT_SIGNAL_MS);
  }
}

void CommandProcessor::InitialColorSelection(StateMachine::ModeType current_mode)
{
  if (current_mode == StateMachine::ColorSelectMode)
  {
    uint32_t color_a, color_b;
    if (state_machine->GetSelectedPattern() == 0)
    {
      previous_selected_color2 = state_machine->GetSelectedColor2Option();
      eeprom->ReadColor2Schema(previous_selected_color2, &color_a, &color_b);
      state_machine->SetSelectedColors(color_a, color_b, 0x0, 0x0);
    }
    else 
    {
      uint32_t color_c, color_d;
      previous_selected_color4 = state_machine->GetSelectedColor4Option();
      eeprom->ReadColor4Schema(previous_selected_color4, &color_a, &color_b, &color_c, &color_d);
      state_machine->SetSelectedColors(color_a, color_b, color_c, color_d);
    }
  }
}

void CommandProcessor::SaveColor(StateMachine::ModeType current_mode)
{
  if (current_mode == StateMachine::SpeedSelectMode)
  {
    uint32_t color_a, color_b, color_c, color_d;
    state_machine->GetSelected4Colors(&color_a, &color_b, &color_c, &color_d);

    if (state_machine->GetSelectedPattern() == 0)
      eeprom->SaveColor2Schema(state_machine->GetSelectedColor2Option(), color_a, color_b);
    else 
      eeprom->SaveColor4Schema(state_machine->GetSelectedColor4Option(), color_a, color_b, color_c, color_d);
  }
}

void CommandProcessor::ColorReSelection(StateMachine::ModeType current_mode)
{
  if (current_mode == StateMachine::ColorSelectMode)
  {
    if (state_machine->GetSelectedPattern() == 0)
    {
      uint8_t selected_color2 = state_machine->GetSelectedColor2Option();  
      if (previous_selected_color2 != selected_color2)
      {     
        uint32_t color_a, color_b;
        eeprom->ReadColor2Schema(selected_color2, &color_a, &color_b);
        state_machine->SetSelectedColors(color_a, color_b, 0x0, 0x0);
        previous_selected_color2 = selected_color2;
      }
    }
    else 
    {
      uint8_t selected_color4 = state_machine->GetSelectedColor4Option();  
      if (previous_selected_color4 != selected_color4)
      {  
        uint32_t color_a, color_b, color_c, color_d;
        eeprom->ReadColor4Schema(state_machine->GetSelectedColor4Option(), &color_a, &color_b, &color_c, &color_d);
        state_machine->SetSelectedColors(color_a, color_b, color_c, color_d);
        previous_selected_color4 = selected_color4;
      }
    }
  }
}

void CommandProcessor::ProcessStateChanges()
{
  StateMachine::ModeType current_mode = state_machine->GetCurrentMode();
  if (current_mode != previous_mode)
  {
    SaveSelection(current_mode);
    InitialColorSelection(current_mode);
    SaveColor(current_mode);

    previous_mode = current_mode;
    led_indicatior->Blink(1, SHORT_SIGNAL_MS, SHORT_SIGNAL_MS);
  }
  else
  {
    ColorReSelection(current_mode);
  }
}