#pragma once
#include "Arduino.h"
#include "ComponentPropaties.hpp"
#include "FlagSet.hpp"
#include "Observer.hpp"

class RE_Trans
{
private:
    FlagSet &flag_set;
    Observer<bool> obs1, obs2, obs3, obs4, obs5, obs6, obs7, obs8, obs9, obs10;
    Observer<int> obs11, obs12;
    char dataFrame[13];
    uint8_t queue = 0;
    bool data_send_flag = 0;

    void clearBuffer();
    void queueProcess();
    void generateFrameData();
    void updateTargetLEDs(uint8_t);
    bool waitMs(uint8_t);
    uint8_t transmit_interval;
    uint16_t power(uint16_t, uint16_t);
    uint8_t rescaling_analog_val(uint16_t, uint8_t);

public:
    RE_Trans(FlagSet &);
    virtual ~RE_Trans() {}

    void initialize();
    uint8_t transmit();
};

inline RE_Trans::RE_Trans(FlagSet &flag_set_) : flag_set(flag_set_)
{
    transmit_interval = 500;
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
    dataFrame[10] = '0';
    dataFrame[11] = '\r';
    dataFrame[12] = '\n';
};

inline void RE_Trans::initialize()
{
    IM920_PROPS.port.begin(IM920_PROPS.baud);
    IM920_PROPS.port.listen();
    pinMode(IM920_PROPS.busy_pin, INPUT);
    pinMode(userLED1.pin, OUTPUT);
    pinMode(userLED2.pin, OUTPUT);
    pinMode(userLED3.pin, OUTPUT);
    pinMode(userLED4.pin, OUTPUT);
}

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

    queueProcess();

    if (!(queue > 0) || !data_send_flag)
        return 0;
    queue--;

    generateFrameData();

    for (uint8_t i = 0; i < sizeof(dataFrame) / sizeof(char); i++)
    {
        //Serial.print(char(dataFrame[i]));
        IM920_PROPS.port.print((char)dataFrame[i]);
    }

    this->clearBuffer();
    return 1;
}

inline void RE_Trans::queueProcess()
{
    if (digitalRead(IM920_PROPS.busy_pin))
    {
        data_send_flag = false;
    }
    if (obs1.isChanged(flag_set.sw1_state) ||
        obs2.isChanged(flag_set.sw2_state) ||
        obs3.isChanged(flag_set.sw3_state) ||
        obs4.isChanged(flag_set.sw4_state) ||
        obs5.isChanged(flag_set.sw5_state) ||
        obs6.isChanged(flag_set.sw6_state) ||
        obs7.isChanged(flag_set.sw7_state) ||
        obs8.isChanged(flag_set.sw8_state) ||
        obs9.isChanged(flag_set.sw9_state) ||
        obs10.isChanged(flag_set.sw10_state) ||
        obs11.isChanged(rescaling_analog_val(analogRead(ANALOGIN1_PROPS.pin), 6)))
    {
        data_send_flag = true;
        queue++;
    }
    if (waitMs(transmit_interval))
    {
        queue++;
        data_send_flag = true;
    }
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
    uint8_t target_machine_num = rescaling_analog_val(potentiometer_val, 6);
    updateTargetLEDs(target_machine_num);
    dataFrame[9] = target_machine_num + ASCII_OFFSET;
    // Serial.print(log10(potentiometer_val));
    // Serial.print('\t');
    // Serial.println(target_machine_num);
    return;
}

inline uint8_t RE_Trans::rescaling_analog_val(uint16_t analog_val, uint8_t divide_num)
{
    const double step_val = 3.0 / divide_num; //log10(1023) = 3.0
    if (analog_val < 3)
    {
        return 1;
    }
    for (uint8_t i = 1; i < divide_num + 1; i++)
    {
        if (log10(analog_val) <= (step_val * i) + ((divide_num + 1) - i) * 0.2)
        {
            return i;
        }
    }
    return divide_num;
}

inline void RE_Trans::updateTargetLEDs(uint8_t machine_Num)
{
    if (!obs12.isChanged(machine_Num))
    {
        return;
    }

    bool LEDstate[4];
    switch (machine_Num)
    {
    case 1:
        LEDstate[0] = HIGH;
        LEDstate[1] = LOW;
        LEDstate[2] = LOW;
        LEDstate[3] = LOW;
        break;
    case 2:
        LEDstate[0] = LOW;
        LEDstate[1] = HIGH;
        LEDstate[2] = LOW;
        LEDstate[3] = LOW;
        break;
    case 3:
        LEDstate[0] = LOW;
        LEDstate[1] = LOW;
        LEDstate[2] = HIGH;
        LEDstate[3] = LOW;
        break;
    case 4:
        LEDstate[0] = LOW;
        LEDstate[1] = LOW;
        LEDstate[2] = LOW;
        LEDstate[3] = HIGH;
        break;
    case 5:
        LEDstate[0] = HIGH;
        LEDstate[1] = HIGH;
        LEDstate[2] = LOW;
        LEDstate[3] = LOW;
        break;
    case 6:
        LEDstate[0] = LOW;
        LEDstate[1] = LOW;
        LEDstate[2] = HIGH;
        LEDstate[3] = HIGH;
        break;
    default:
        break;
    }
    digitalWrite(userLED1.pin, LEDstate[0]);
    digitalWrite(userLED2.pin, LEDstate[1]);
    digitalWrite(userLED3.pin, LEDstate[2]);
    digitalWrite(userLED4.pin, LEDstate[3]);
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