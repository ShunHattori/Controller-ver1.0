
/*  Robocon 2020 C-team "controller"
 *  Author: Hattori Shun 
 *  説明:統合基板コントローラ用プログラム
 *  開発時期:2020/08/13 ~ 2020/08/15
 *  機能:遠隔緊急停止,スイッチ読み取り
 */

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "AntiChattering.hpp"
#include "RE_Trans.hpp"
#include "FlagSet.hpp"

FlagSet controller_flag;
SoftwareSerial ss_port(ss_rx_pin, ss_tx_pin);
RE_Trans re_trans(controller_flag);

AntiChattering switch_1(SWITCH_PROPS1);
AntiChattering switch_2(SWITCH_PROPS2);
AntiChattering switch_3(SWITCH_PROPS3);
AntiChattering switch_4(SWITCH_PROPS4);
AntiChattering switch_5(SWITCH_PROPS5);
AntiChattering switch_6(SWITCH_PROPS6);
AntiChattering switch_7(SWITCH_PROPS7);
AntiChattering switch_8(SWITCH_PROPS8);
AntiChattering switch_9(SWITCH_PROPS9);
AntiChattering switch_10(SWITCH_PROPS10);

void updateSwitches();
void setSwitchStates();

void debugPrint();

void setup()
{
  Serial.begin(256000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  updateSwitches();
  setSwitchStates();
  re_trans.transmit();

  //debugPrint();
}

void setSwitchStates()
{
  controller_flag.sw1_state = switch_1.getState();
  controller_flag.sw2_state = switch_2.getState();
  controller_flag.sw3_state = switch_3.getState();
  controller_flag.sw4_state = switch_4.getState();
  controller_flag.sw5_state = switch_5.getState();
  controller_flag.sw6_state = switch_6.getState();
  controller_flag.sw7_state = switch_7.getState();
  controller_flag.sw8_state = switch_8.getState();
  controller_flag.sw9_state = switch_9.getState();
  controller_flag.sw10_state = switch_10.getState();
}

void updateSwitches()
{
  switch_1.update();
  switch_2.update();
  switch_3.update();
  switch_4.update();
  switch_5.update();
  switch_6.update();
  switch_7.update();
  switch_8.update();
  switch_9.update();
  switch_10.update();
}

void debugPrint()
{
  Serial.print(controller_flag.sw1_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw2_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw3_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw4_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw5_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw6_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw7_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw8_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw9_state);
  Serial.print('\t');
  Serial.print(controller_flag.sw10_state);
  Serial.print("\r\n");
}