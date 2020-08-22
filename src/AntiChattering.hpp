#pragma once
#include <Arduino.h>
#include "ComponentPropaties.hpp"

/*
 *  Anti-chatter read for atmega328p controller
 */

class AntiChattering
{
private:
  SwitchPropaties SWITCH_PROPS;
  uint64_t sampled_time;
  uint16_t detect_flag_number, analog_detect_value, sampling_period, button_press_count;
  bool button_state, is_enable_updating, is_pin_analog;
  void analogProcess();
  void digitalProcess();
  void setStateFlag();

public:
  AntiChattering() = delete;
  AntiChattering(SwitchPropaties);
  void update();
  void setDetectCount(int);
  void setButtonState(bool);
  void disableStateUpdate();
  void enableStateUpdate();
  bool getState();

  virtual ~AntiChattering(){};
};

extern AntiChattering switch_1;
extern AntiChattering switch_2;
extern AntiChattering switch_3;
extern AntiChattering switch_4;
extern AntiChattering switch_5;
extern AntiChattering switch_6;
extern AntiChattering switch_7;
extern AntiChattering switch_8;
extern AntiChattering switch_9;
extern AntiChattering switch_10;
