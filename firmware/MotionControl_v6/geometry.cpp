/*
 * geometry.cpp
 */

#include <math.h>
#include <stdlib.h>
#include "geometry.h"

float Point::getDistance(float x, float y)
{
    return sqrt(pow(m_x - x, 2) + pow(m_y - y, 2));
}

void Pose::local_to_global(Point & robot_local, Point & global_point)
{
    float cos_t = cos(m_theta);
    float sin_t = sin(m_theta);
    global_point.x(x() + robot_local.x() * cos_t - robot_local.y() * sin_t);
    global_point.y(y() + robot_local.x() * sin_t + robot_local.y() * cos_t);
}

void Pose::global_to_local(Point & global_point, Point & robot_local)
{
    float cos_t = cos(m_theta);
    float sin_t = sin(m_theta);
    float dx = global_point.x() - x();
    float dy = global_point.y() - y();
    robot_local.x(dx * cos_t + dy * sin_t);
    robot_local.y(- dx * sin_t + dy * cos_t);
}

void Line::set_line(Point & target, Point & pointForLine)
{
    a = target.y() - pointForLine.y();
    b = target.x() - pointForLine.x();
    c = (target.x() * pointForLine.y()) - (pointForLine.x() * target.y());
}

float Line::getDistance(float x, float y)
{
    float nm = (a * x + b * y + c );
    float dnm = sqrt(a*a + b*b);
    return nm/dnm;
}

float Line::getDTheta()
{
    return atan2(-a,b);
}
