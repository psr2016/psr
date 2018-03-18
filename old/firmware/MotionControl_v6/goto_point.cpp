/*
 * goto_point.cpp
 */

#include "geometry.h"
#include "goto_point.h"
#include <math.h>

GoTo_Point::GoTo_Point(Kinematics & kinem, SpeedControlTask & speed_ctrl,
                       float linear_accel,
                       float linear_vmax,
                       float linear_decel,
                       float linear_vmin)
      : PositionControl(kinem, speed_ctrl),
      m_linear_accel(linear_accel),
      m_linear_vmax(linear_vmax),
      m_linear_decel(linear_decel),
      m_linear_vmin(linear_vmin),
      m_target_reached(false),
      m_next_speed(0),
      m_angular_next_speed(0),
      m_xT(0),
      m_yT(0),
      kd(1.0),
      kh(75.0),
      m_alfa(0),
      m_rho(0),
      m_delta_x(0),
      m_delta_y(0),
      m_direction(0),
      m_linear_speed(0),
      m_angular_speed(0),
      m_vl(0),
      m_vr(0)
    {
        //trasformazione da lineare ad angolare
        float wheelbase = m_kinematics.wheelbase(); //wheelbase = 303
        m_angular_accel = 2*m_linear_accel / wheelbase; //linear_accel = accelerazione singola ruota  //3,96
        m_angular_decel = 2*m_linear_decel / wheelbase; //linear_decel = decelerazione singola ruota  //3,96
        m_angular_vmax = 2*m_linear_vmax / wheelbase; //linear_vmax = velocità massima singola ruota  //3,96

        m_angular_accel_step = m_angular_accel * m_real_time_period;
        m_angular_decel_distance = (m_angular_vmax * m_angular_vmax) / (2 * m_angular_decel);  //1,98
         //conversione da gradi a radianti
        // m_angular_target_range : PI = angular_target_range : 180


        m_accel_step = m_linear_accel * m_real_time_period;
        m_decel_distance = (m_linear_vmax * m_linear_vmax) / (2 * m_linear_decel); //300
    }

    //supponiamo xT = 100 yT=500  xS=300 yS=100  theta= 0.785398 rad
    //m_delta_x = -200 m_delta_y = 400
    //m_alfa = -1.89255   m_rho = 346.40

    void GoTo_Point::setAlpha(float delta_x,float delta_y)
    {
        m_alfa = normalizeAngle( atan2(delta_y,delta_x) - m_kinematics.pose().theta() ); 
    }
    
    void GoTo_Point::setDistance(float delta_x,float delta_y)
    {
        m_rho  = sqrt(pow(delta_x,2)+pow(delta_y,2));
    }

    void GoTo_Point::set_target(float x,float y)
    {
        m_xT = x;
        m_yT = y;

        m_delta_x = m_xT - m_kinematics.pose().x();
        m_delta_y = m_yT - m_kinematics.pose().y();

        setAlpha(m_delta_x,m_delta_y);
        setDistance(m_delta_x,m_delta_y);

        m_direction = 1;
        
        m_target_reached = false;

    }

    //supponiamo xT = 100 yT=500  xS=300 yS=100  theta= 0.785398 rad
    //m_delta_x = -200 m_delta_y = 400
    //m_alfa = -1.89255   m_rho = 346.40
    
void GoTo_Point::run()
{
    if (m_target_reached) return;
    
    //target superato!
    if(fabs(m_xT) - m_kinematics.pose().x() < 0 && fabs(m_yT) - m_kinematics.pose().y() < 0 )
    {
        m_direction = -m_direction;
    }

    if (fabs(m_rho) <= 30) //da stabilire il threshold
    {
        m_speed_control.set_targets(0, 0);//Fermo il robot
        m_target_reached = true;
        return;
    }

    m_delta_x = m_xT - m_kinematics.pose().x();
    m_delta_y = m_yT - m_kinematics.pose().y();

    setAlpha(m_delta_x,m_delta_y);

    setDistance(m_delta_x,m_delta_y);

    m_linear_speed = linear_speed_controller(m_rho);
    m_angular_speed = angular_speed_controller(m_alfa); //-3.87
    if(m_rho >=30 && m_rho <=40) //da controllare queste soglie messe a muzzu
    {

        m_vl =  m_direction * m_linear_speed;
        m_vr =  m_direction * m_linear_speed;
    }
    
    //(m_angular_speed * m_kinematics.wheelbase() / 2.0 ) = -586.3
    
    //m_vl = m_linear_speed + (m_angular_speed * m_kinematics.wheelbase() / 2.0 );
    //m_vr = m_linear_speed - (m_angular_speed * m_kinematics.wheelbase() / 2.0 );
    
    else
    {
        m_vl = m_direction * ( m_linear_speed - (m_angular_speed * m_kinematics.wheelbase() / 2.0 ));
        m_vr = m_direction * ( m_linear_speed + (m_angular_speed * m_kinematics.wheelbase() / 2.0 ));
    }
    m_speed_control.set_targets(m_vl,m_vr);

}


//rho = 346.40  m_decel_distance = 300
float GoTo_Point::linear_speed_controller(float rho)
{
    float current_speed = m_kinematics.linear_speed();

        if (rho < m_decel_distance)
        {
            // fase 3                     sqrt( 360000 - 2*600*(300-346.40)) = 644.73
            float expected_speed = sqrt(m_linear_vmax * m_linear_vmax - 2 * m_linear_decel * (m_decel_distance - rho));
            if (expected_speed > fabs(current_speed))
            {
                // la vel da raggiungere risulta MAGGIORE della vel corrente
                // questo vuol dire che siamo ancora nella fase di accelerazione
                // pertanto acceleriamo
                m_next_speed += m_accel_step;
                if (m_next_speed > expected_speed)
                    m_next_speed = expected_speed;

                if (m_next_speed > m_linear_vmax)
                    m_next_speed = m_linear_vmax;
            }
            else
            {
                m_next_speed = expected_speed;
            }
        }
        else
        {
            // Fase 1, fase 2
            if (m_next_speed < m_linear_vmax)
            {
                m_next_speed += m_accel_step;

                if (m_next_speed > m_linear_vmax)
                    m_next_speed = m_linear_vmax;
            }
        }

        return m_next_speed;
}

//alfa = -1.89255 m_angular_decel_distance = 1,98
float GoTo_Point::angular_speed_controller(float alfa)
{
    float angular_speed = m_kinematics.angular_speed();
    float distance = fabs(alfa);

    if (distance < m_angular_decel_distance) {
        //fase 3
        //velocità attesa della frenata
        float expected_angular_speed = sqrt(m_angular_vmax * m_angular_vmax - 2 * m_angular_decel * (m_angular_decel_distance - distance)); //sqrt((3.96^2) - (2*3.96)*(1.98-1.89255)) = 3.87
        if (expected_angular_speed > fabs(angular_speed)) {
            // la vel da raggiungere risulta MAGGIORE della vel corrente
            // questo vuol dire che siamo ancora nella fase di accelerazione
            // pertanto acceleriamo
            m_angular_next_speed += m_angular_accel_step;
            if (m_angular_next_speed > expected_angular_speed)
                m_angular_next_speed = expected_angular_speed;

            if (m_angular_next_speed > m_angular_vmax)
                m_angular_next_speed = m_angular_vmax;
        }
        else {
            m_angular_next_speed = expected_angular_speed;
        }
    }
    else {
        //fase 1
        if (m_angular_next_speed < m_angular_vmax) {
            m_angular_next_speed += m_angular_accel_step;
            //fase 2
            if (m_angular_next_speed > m_angular_vmax)
                m_angular_next_speed = m_angular_vmax;
        }
    }
    if(alfa >= 0)
        return m_angular_next_speed;
    else
        return -m_angular_next_speed;
}

float GoTo_Point::normalizeAngle(float a)
{
    if (a > PI)
        a = a - TWO_PI;
    if (a < -PI)
        a = a + TWO_PI;
    return a;
}
