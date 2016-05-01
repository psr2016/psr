/*
 * Input.h
 */

#ifndef __INPUT_H
#define __INPUT_H

#include "World.h"

typedef enum {
    InputNone,
    InputConst,
    InputStep,
    InputRamp
} t_InputType;

class Input {
 public:
    Input(Core::World * world, float sampling_time);
    float value();
    float last_value() { return m_current_value; };
 private:
    Core::World * m_world;
    QSettings * m_settings;
    float m_sampling_time;
    float m_time;
    float m_current_value;

    int m_time_sequence;

    t_InputType m_current_event_input_type;
    float m_current_event_input_value;

    t_InputType m_event_input_type;
    float m_event_input_time;
    float m_event_input_value;

    void loadNext();
};

#endif
