#pragma once
#include "Arduino.h"
#include "ComponentPropaties.hpp"
#include "FlagSet.hpp"

class RE_Trans
{
private:
    FlagSet &flag_set;
    char dataFrame[12];

    void clearBuffer();
    void generateFrameData();
    bool waitMs(uint8_t);
    uint8_t transmit_interval;
    uint16_t power(uint16_t, uint16_t);
    uint8_t rescaling_analog_val(uint16_t, uint8_t);

public:
    RE_Trans(FlagSet &);
    virtual ~RE_Trans() {}

    uint8_t transmit();
};

inline RE_Trans::RE_Trans(FlagSet &flag_set_) : flag_set(flag_set_)
{
    IM920_PROPS.port.begin(IM920_PROPS.baud);
    IM920_PROPS.port.listen();
    pinMode(IM920_PROPS.busy_pin, INPUT);
    transmit_interval = 15;
    dataFrame[0] = 'T';
    dataFrame[1] = 'X';
    dataFrame[2] = 'D';
    dataFrame[3] = 'A';
    dataFrame[4] = ' ';
    dataFrame[5] = '0';
    dataFrame[6] = '0';
    dataFrame[7] = '0';
    dataFrame[8] = '0';
    dataFrame[9] = '0';
    dataFrame[10] = '\r';
    dataFrame[11] = '\n';
};

inline uint8_t RE_Trans::transmit()
{
    /********TEST CODE HERE*********/
    // for (uint16_t i = 0; i < 1024; i++)
    // {
    //     for (int k = 0; k < 10; k++)
    //     {
    //         *flag_set.sw_state_array[k] = (uint16_t(i >> k) & 0x1);
    //         Serial.print(*flag_set.sw_state_array[k] == 1 ? (char)178 : (char)177);
    //         if (k == 9)
    //             Serial.println();
    //     }
    //     generateFrameData();
    // }
    // while (1)
    //     ;
    /********TEST CODE HERE*********/

    if (!waitMs(transmit_interval))
    {
        return 0;
    }

    generateFrameData();

    if (!digitalRead(IM920_PROPS.busy_pin))
    {
        for (uint8_t i = 0; i < sizeof(dataFrame) / sizeof(char); i++)
        {
            Serial.print(char(dataFrame[i]));
            IM920_PROPS.port.print(dataFrame[i]);
        }
    }

    this->clearBuffer();
    return 1;
}

inline void RE_Trans::generateFrameData()
{
    /* 
     * 0 ~ 1023(2^10)の接点パターンを持つ。
     * 接点パターンを各桁のASCII(4桁→4bytes)に分割
     * データフレームに各桁を格納→送信
     */
    uint16_t switch_data_shifted = 0;
    for (uint8_t i = 0; i < switch_num; i++)
    {
        switch_data_shifted |= uint16_t(uint16_t(*flag_set.sw_state_array[i]) << i);
    }

    // Serial.println(switch_data_shifted);
    uint16_t left_data = switch_data_shifted;
    for (uint8_t i = 0; i < 4; i++)
    {
        uint16_t digit_shift = power(10, 3 - i);
        dataFrame[i + 5] = left_data / digit_shift + ASCII_OFFSET;
        left_data %= digit_shift;
        // Serial.print(left_data);
        // Serial.print('\t');
        // Serial.print(digit_shift);
        // Serial.print('\t');
        // Serial.println(byte(dataFrame[i + 5]));
    }

    uint16_t potentiometer_val = analogRead(ANALOGIN1_PROPS.pin);
    dataFrame[9] = rescaling_analog_val(potentiometer_val, 6) + ASCII_OFFSET;
    // Serial.print(potentiometer_val);
    // Serial.print('\t');
    // Serial.println(rescaling_analog_val(potentiometer_val, 6));
    return;
}

inline uint8_t RE_Trans::rescaling_analog_val(uint16_t analog_val, uint8_t divide_num)
{
    const double step_val = 1023.0 / divide_num;
    for (uint8_t i = 1; i < divide_num + 1; i++)
    {
        if (analog_val <= step_val * i)
        {
            return i;
        }
    }
    return 1;
}

inline void RE_Trans::clearBuffer()
{
    while (IM920_PROPS.port.available())
    {
        IM920_PROPS.port.read();
    }
}

/*
 *  return true when (interval time ) has passed, return false if not  
 */
inline bool RE_Trans::waitMs(uint8_t interval_time)
{
    static long long int period_from_prev_true;
    if ((millis() - period_from_prev_true) > interval_time)
    {
        period_from_prev_true = millis();
        return true;
    }
    return false;
}

inline uint16_t RE_Trans::power(uint16_t base, uint16_t times)
{
    uint32_t result = 1;
    for (uint8_t i = 0; i < times; i++)
    {
        result *= base;
    }
    return result;
}