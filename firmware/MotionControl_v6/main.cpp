/*
 * main.cpp
 */

#include "defines.h"

#include <p33FJ128MC802.h>

#include "geometry.h"
#include "kinematics.h"
#include "speed_control.h"
#include "can_interface.h"
#include "clocking.h"
#include "qei.h"
#include "pwm.h"
#include "timers.h"
#include "gpio.h"
#include "relative_rotation.h"
#include "absolute_rotation.h"
#include "circ_rotation.h"
#include "path_control.h"
#include "follow_line.h"
#include "goto_point.h"
//-----------------------
#include "i2c.h"
#include "Controllore.h"
//-----------------------

#pragma config FPWRT = PWR1   // Power-on timer: Disabled
#pragma config ALTI2C = OFF   // I2C mapped to SDA1/SCL1 pins
#pragma config ICS = PGD2     // Communicate on PGC2/EMUC2 and PGD2/EMUD2
#pragma config PWMPIN = ON    // PWM module pins controlled by I/O module at device Reset

#pragma config FNOSC = FRC    // Internal Fast RC (FRC)
#pragma config FCKSM = CSECMD // Clock switching is enabled, Fail-Safe Clock Monitor is disabled
#pragma config OSCIOFNC = ON  // OSC2 pin has digital I/O function
//#pragma config IOL1WAY = OFF  // Allow multiple PPS reconfigurations
#pragma config JTAGEN = OFF   // JTAG is Disabled
#pragma config FWDTEN = OFF   // Watchdog timer controlled by user software


void initialize_peripherals()
{
    init_clock();

    IEC0 = 0;				// DISABLE ALL USER INTERRUPT
    IEC1 = 0;				
    IEC2 = 0;
    IEC3 = 0;	
    IEC4 = 0;	

    RCONbits.SWDTEN   = 0;	// disable Watchdog Timer

    AD1PCFGL = 0xffff;

    init_gpio();
    init_can_interface();
    init_pwm();
    init_timer2();
    init_qei();

}

void flash_at_boot()
{
    /* led flashing to signal a reboot */
    for (int i = 0;i < 5;i++) {
        __delay_ms(100);
        led_on();
        __delay_ms(100);
        led_off();
    }
}


Pose current_robot_pose;
Kinematics kinematics(current_robot_pose, 27, 27, 303, 4000); // initial values
SpeedControlTask speed_control(kinematics);
CanCommandReceiver  can_receiver(kinematics, speed_control);
CanPoseSender  can_pose_sender(current_robot_pose);
CanSpeedSender  can_speed_sender(speed_control);
RelativeRotation relative_rotation(kinematics, speed_control, 600, 600, 600, 20, 0.5);
AbsoluteRotation absolute_rotation(kinematics, speed_control, 600, 600, 600, 20, 0.5);
CircularRotation circular_rotation(kinematics, speed_control, 600, 600, 600, 20, 0.5);
FollowLine follow_line(kinematics, speed_control, 600, 600, 600, 20);
GoTo_Point goto_point(kinematics, speed_control, 600, 600, 600, 20);
PathControl path_control(kinematics, speed_control);
//----------------------------------------------------------------------------------------------
I2C i2c_comunication_imu(I2C_FREQ_100KHZ);                             //Istanza oggetto i2c
Controllore controller(i2c_comunication_imu, 1, 0, 10);    //Istanza oggetto Controllore
//----------------------------------------------------------------------------------------------
CanGyroSender gyro_sender(controller);

int main()
{
    initialize_peripherals();
    flash_at_boot();

    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;

    ODCBbits.ODCB8 = 0;
    ODCBbits.ODCB9 = 0;

    LATBbits.LATB8 = 1;
    LATBbits.LATB9 = 1;

    kinematics.on();
    can_pose_sender.on();
    path_control.on();

    //----------------------------------------------------------------------------
    i2c_comunication_imu.open();      //Avvio dell'i2c
    controller.InitGyro();            //Inizializzazione IMU
    controller.on();                  //Avvio del task periodico Controllore
    // //----------------------------------------------------------------------------
    gyro_sender.on();

    for (;;) {
	
        can_receiver.run();

        // wait next timer tick
        if (IFS0bits.T2IF == 1) {
            IFS0bits.T2IF = 0;
            run_all_tasks();
        }

    }
    return 0;
}
