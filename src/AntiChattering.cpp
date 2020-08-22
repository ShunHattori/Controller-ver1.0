#include "AntiChattering.hpp"

AntiChattering::AntiChattering(SwitchPropaties SWITCH_PROPS_)
    : SWITCH_PROPS(SWITCH_PROPS_),
      sampled_time(millis()),
      detect_flag_number(3),
      analog_detect_value(analog_switch_threshold), //10 bit
      sampling_period(5),
      button_state(0),
      is_enable_updating(1),
      is_pin_analog(0)
{
    if (A0 <= SWITCH_PROPS.pin && SWITCH_PROPS.pin <= A7) //analogPin
    {
        is_pin_analog = 1;
        pinMode(SWITCH_PROPS.pin, INPUT);
        return;
    }

    if (SWITCH_PROPS.is_external_pullupped) //digitalPin
    {
        pinMode(SWITCH_PROPS.pin, INPUT_PULLUP);
    }
    else
    {
        pinMode(SWITCH_PROPS.pin, INPUT);
    }
}

void AntiChattering::update()
{
    if (!is_enable_updating)
        return;

    if (is_pin_analog)
        analogProcess();
    else
        digitalProcess();

    setStateFlag();
    return;
}

void AntiChattering::analogProcess()
{
    if (sampling_period < (millis() - sampled_time))
    {
        if (uint16_t(analogRead(SWITCH_PROPS.pin)) > analog_detect_value)
        {
            button_press_count++;
        }
        else
            button_press_count = 0;
        sampled_time = millis();
    }
}

void AntiChattering::digitalProcess()
{
    if (sampling_period < (millis() - sampled_time))
    {
        switch (uint8_t(SWITCH_PROPS.is_external_pullupped))
        {
        case true:
            if (!digitalRead(SWITCH_PROPS.pin))
            {
                button_press_count++;
            }
            else
                button_press_count = 0;
            break;

        default:
            if (digitalRead(SWITCH_PROPS.pin))
            {
                button_press_count++;
            }
            else
                button_press_count = 0;
            break;
        }
        sampled_time = millis();
    }
}

void AntiChattering::setStateFlag()
{
    if (detect_flag_number < button_press_count)
    {
        button_state = 1;
    }
    else
    {
        button_state = 0;
    }
}

void AntiChattering::setDetectCount(int count)
{
    detect_flag_number = count;
}
void AntiChattering::setButtonState(bool state)
{
    button_state = state;
}
void AntiChattering::disableStateUpdate()
{
    is_enable_updating = 0;
}
void AntiChattering::enableStateUpdate()
{
    is_enable_updating = 1;
}

bool AntiChattering::getState()
{
    return button_state;
}