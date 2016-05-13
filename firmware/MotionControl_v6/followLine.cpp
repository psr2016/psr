/*
 * followLine.cpp
 */

#include "followLine.h"

bool FollowLine::target_reached() 
{
    return false;//TODO
}

float FollowLine::evaluate()
{
    return 0;//TODO
}

float FollowLine::calcGamma()
{
    float distance = m_line.getDistance(m_kinematics.pose().x(), m_kinematics.pose().y());
    return (-kd * distance + kh) * (m_line.getDTheta() - m_kinematics.pose().theta());
}

void FollowLine::run()
{
 //TODO 
}



