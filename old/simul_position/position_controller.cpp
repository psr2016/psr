/*
 * position_controller.cpp
 */

#include "position_controller.h"

#include <math.h>

PositionController::PositionController(float accel, float v_max, float decel, float dt)
    : m_accel(accel), m_vmax(v_max), m_decel(decel), m_dt(dt), m_next_speed(0)
{
    m_accel_step = m_accel * m_dt;
    m_decel_distance = (m_vmax * v_max) / (2 * m_decel);
}

float PositionController::evaluate(float target, float current_pos, float current_speed)
{
    float distance = target - current_pos;
    float s;

    if (distance < 0) {
        s = -1;
        distance = -distance;
    }
    else
        s = 1;

    if (distance < m_decel_distance) {
        // fase 3
        float expected_speed = sqrt(m_vmax * m_vmax - 2 * m_decel * (m_decel_distance - distance));
        if (expected_speed > fabs(current_speed)) {
            // la vel da raggiungere risulta MAGGIORE della vel corrente
            // questo vuol dire che siamo ancora nella fase di accelerazione
            // pertanto acceleriamo
            m_next_speed += m_accel_step;
            if (m_next_speed > expected_speed) m_next_speed = expected_speed;
            if (m_next_speed > m_vmax) m_next_speed = m_vmax;
        }
        else {
            m_next_speed = expected_speed;
        }
    }
    else {
        // Fase 1, fase 2
        if (m_next_speed < m_vmax) {
            m_next_speed += m_accel_step;
            if (m_next_speed > m_vmax) m_next_speed = m_vmax;
        }
    }
    return s*m_next_speed;
}

