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

void initialize_peripherals()
{
    init_clock();

    IEC0 = 0;				// DISABLE ALL USER INTERRUPT
    IEC1 = 0;				
    IEC2 = 0;
    IEC3 = 0;	
    IEC4 = 0;	

    RCONbits.SWDTEN   = 0;	// disable Watchdog Timer

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
PathControl path_control(kinematics, speed_control);

int main()
{
    initialize_peripherals();
    flash_at_boot();

    kinematics.on();
    can_pose_sender.on();

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
