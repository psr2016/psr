#ifndef __CONTROLLORE_H
#define __CONTROLLORE_H

#include "periodic_task.h"
#include "i2c.h"
#include "time_defines.h"
#include "speed_control.h"

class Controllore : public PeriodicTask
{
  public:
    //COSTRUTTORE
    Controllore(I2C & IMU, SpeedControlTask & Controllo_Vel, float kp, float ki, float Saturazione);

    //METODI DELLA CLASSE
    float Controller_PI(float Input);
    void ReadGyro();
    void InitGyro();
    void run();
    void on();

    void ReadRawData (short & x, short & y, short & z)
    {
        x = m_omega_x16;
        y = m_omega_y16;
        z = m_omega_z16;
    };

    void set_KP(float KP_ext);         //Metodo per fissare Kp da terminale
    void set_KI(float KI_ext);         //Metodo per fissare Ki da terminale
    void set_SAT(float SAT_ext);       //Metodo per fissare SAT da terminale
    void set_V(float V_ext);           //Metodo per fissare V da terminale
    void SPEED_CONTROL();              //Calcolo delle velocià delle ruote, usando il PI


  private:
    //ATTRIBUTI DELLA CLASSE
    I2C & IMU_comunication;                //Riferimento alla classe I2C
    SpeedControlTask & m_Controllo_Vel;    //Riferimento alla classe SpeedControlTask

    float Kp, Ki;
    float SAT;
    bool SAT_FLAG;
    float OUT_I;

    float OMEGA_X, OMEGA_Y, OMEGA_Z;
    float OFFSET_X, OFFSET_Y, OFFSET_Z;

    short int m_omega_x16;
    short int m_omega_y16;
    short int m_omega_z16;

    float V_lineare;
    float V_Left;
    float V_Right;

};

extern Controllore gyro_controller;

#endif
