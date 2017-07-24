#ifndef __CONTROLLORE_H
#define __CONTROLLORE_H

#include "periodic_task.h"
#include "i2c.h"
#include "time_defines.h"

class Controllore : public PeriodicTask
{
  public:
    //COSTRUTTORE
    Controllore(I2C & IMU, float kp, float ki, float Saturazione);

    //METODI DELLA CLASSE
    float Controller_PI(float Input);
    void ReadGyro();
    void InitGyro();
    void run();
    void ReadRawData (short & x, short & y, short & z) {
        x = m_omega_x16;
        y = m_omega_y16;
        z = m_omega_z16;
    };

  private:
    //ATTRIBUTI DELLA CLASSE
    I2C & IMU_comunication;
    float Kp, Ki;
    float SAT;
    bool SAT_FLAG;
    float OUT_I;

    float OMEGA_X, OMEGA_Y, OMEGA_Z;
    float OFFSET_X, OFFSET_Y, OFFSET_Z;

    short int m_omega_x16;
    short int m_omega_y16;
    short int m_omega_z16;
};
#endif
