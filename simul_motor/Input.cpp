/*
 * Input.cpp
 */

#include "Input.h"
#include <string.h>
#include <iostream>

Input::Input(Core::World * world, float sampling_time)
    : m_world(world), m_sampling_time(sampling_time)
{
    m_settings = world->settings();
    m_time = 0;
    m_time_sequence = 0;
    m_current_value = 0;
    m_current_event_input_type = InputConst;
    loadNext();
}

float Input::value()
{
    if (m_current_event_input_type != InputNone) {
        if (m_time >= m_event_input_time) {
            m_current_event_input_type = m_event_input_type;
            m_current_event_input_value = m_event_input_value;
            loadNext();
        }
    }
    switch (m_current_event_input_type) {
    case InputStep:
        m_current_value = m_current_event_input_value;
        break;
    case InputConst:
        break;
    case InputRamp:
        m_current_value = m_current_value + m_current_event_input_value;
        break;
    }
    m_time += m_sampling_time;
    return m_current_value;
}

void Input::loadNext()
{
    char s[40];

    sprintf(s, "Input/%d.time", m_time_sequence);
    m_event_input_time = m_settings->value(s, 0).toFloat();

    sprintf(s, "Input/%d.type", m_time_sequence);
    QString ty = m_settings->value(s, "").toString();
    if (ty == "ramp")
        m_event_input_type = InputRamp;
    else if (ty == "step")
        m_event_input_type = InputStep;
    else if (ty == "const")
        m_event_input_type = InputConst;
    else
        m_event_input_type = InputNone;

    sprintf(s, "Input/%d.value", m_time_sequence);
    m_event_input_value = m_settings->value(s, 0).toFloat();

    //std::cout << "Next event at " << m_event_input_time << ", type " << m_event_input_type << "(" << (char *)ty.data() << ")" << std::endl;

    ++m_time_sequence;
}
