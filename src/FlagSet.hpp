#pragma once
#include "Arduino.h"

struct FlagSet
{
    uint8_t sw1_state,
        sw2_state,
        sw3_state,
        sw4_state,
        sw5_state,
        sw6_state,
        sw7_state,
        sw8_state,
        sw9_state,
        sw10_state;
    uint8_t *sw_state_array[10] = {
        &sw1_state,
        &sw2_state,
        &sw3_state,
        &sw4_state,
        &sw5_state,
        &sw6_state,
        &sw7_state,
        &sw8_state,
        &sw9_state,
        &sw10_state,
    };
};
