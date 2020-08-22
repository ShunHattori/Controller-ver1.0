#pragma once
#include "Arduino.h"
#include "SoftwareSerial.h"

constexpr uint8_t ss_rx_pin = 2;
constexpr uint8_t ss_tx_pin = 3;
constexpr uint8_t switch_num = 10;
constexpr uint8_t ASCII_OFFSET = 48;

extern SoftwareSerial ss_port;

struct IM920Propaties
{
    SoftwareSerial &port;
    uint32_t baud;
    uint8_t busy_pin;
};

constexpr IM920Propaties IM920_PROPS = {
    ss_port,
    19200,
    A2,
};

struct SwitchPropaties
{
    bool is_analog_pin,
        is_external_pullupped;
    uint8_t pin;
};

constexpr uint16_t analog_switch_threshold = 900;

constexpr SwitchPropaties SWITCH_PROPS1 = {false, false, 5};
constexpr SwitchPropaties SWITCH_PROPS2 = {false, false, 6};
constexpr SwitchPropaties SWITCH_PROPS3 = {false, false, 7};
constexpr SwitchPropaties SWITCH_PROPS4 = {false, false, 8};
constexpr SwitchPropaties SWITCH_PROPS5 = {false, false, 9};
constexpr SwitchPropaties SWITCH_PROPS6 = {false, false, 10};
constexpr SwitchPropaties SWITCH_PROPS7 = {false, false, 11};
constexpr SwitchPropaties SWITCH_PROPS8 = {false, false, 12};
constexpr SwitchPropaties SWITCH_PROPS9 = {true, false, A6};
constexpr SwitchPropaties SWITCH_PROPS10 = {true, false, A7};

struct AnalogInPropaties
{
    uint8_t pin;
};

constexpr AnalogInPropaties ANALOGIN1_PROPS = {A0};