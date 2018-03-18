/*
 * geometry.h
 */

#ifndef __GEOMETRY_H
#define __GEOMETRY_H

#define PI       3.14159265358979
#define TWO_PI   6.28318530717959
#define HALF_PI  1.570796326794895

#define DEGREES_60 1.0471975511965976
#define DEGREES_75 1.309

#define LINE_TO_POINT_HDG_THRESHOLD   DEGREES_75

#define TO_DEGREES(a)  ((a) * (180.0/PI))
#define TO_RADIANS(a)  ((a) * (PI/180.0))


class Point {
 public:
 Point(float x = 0, float y = 0)
     : m_x(x), m_y(y) { };
    ~Point() {};
    float x() { return m_x; };
    float y() { return m_y; };
    void x(float _x) { m_x = _x; };
    void y(float _y) { m_y = _y; };
    float getDistance( float x, float y );
    void operator+=(Point & p) { m_x += p.m_x; m_y += p.m_y;};

 private:
    float m_x, m_y;
};

class Pose : public Point {
 public:
 Pose(float x = 0, float y = 0, float theta = 0)
     : Point(x, y), m_theta(theta) { };
    ~Pose() {};
    float theta() { return m_theta;};
    void theta(float t) { m_theta = t; };

    void local_to_global(Point & robot_local, Point & global_point);
    void global_to_local(Point & global_point, Point & robot_local);

 private:
    float m_theta;
};

class Line {
 public:
 Line() {};
    float getDistance(float x, float y);
    float getDTheta();
    void set_line(Point & target, Point & pointForLine);
 private:
    float a, b ,c, dTheta;
};

float normalize_angle(float a);
float hypot(float a, float b);

#endif
