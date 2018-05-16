#
# position_control_task_with_profile.py
#
import sys
import threading
import pylab

from periodic_task import *
from simple_kinematics import *
from controllers import *
from motor_driver import *

class PositionControlTask(PeriodicTask):

    def __init__(self):
        PeriodicTask.__init__(self, 0.01) # 10ms
        self.__motor_driver = MotorDriver()
        self.__kinematics = Kinematics(self.__motor_driver)
        self.__target_pos = 0
        # KP = 50, KI = 400, Saturation = 4200
        self.__speed_controller = PI_Sat_Controller(50, 400, 4200)
        self.__position_controller = PostionControlWithProfile(180, 90, 90)
        self.__mutex = threading.Lock()
        self.__acquire_on = False
        self.__time_array = [0]
        self.__data = [ [], [], [], [] ]

    def start(self):
        self.__motor_driver.open()
        PeriodicTask.start(self)

    def run(self):
        self.__mutex.acquire()
        # run the kinematics
        self.__kinematics.run(self.cycletime)

        # convert current speed from RPM to degrees per seconds
        current_speed = (self.__kinematics.speed / 60.0) * 360.0

        # do the control
        target_speed = self.__position_controller.evaluate(self.__target_pos,
                                                 self.__kinematics.position_degrees,
                                                            current_speed,
                                                            self.cycletime)

        controller_output = self.__speed_controller.evaluate(target_speed,
                                                             current_speed,
                                                            self.cycletime)
        self.__mutex.release()
        # output PWM to driver
        self.__motor_driver.pwm(controller_output)

        if self.__acquire_on:
            self.__time_array.append(self.__time_array[-1] + self.cycletime)
            self.__data[0].append(target_speed)
            self.__data[1].append(current_speed)
            self.__data[2].append(self.__target_pos)
            self.__data[3].append(self.__kinematics.position_degrees)


    def current_speed(self):
        self.__mutex.acquire()
        v =  self.__kinematics.speed
        self.__mutex.release()
        return v

    def current_position(self):
        self.__mutex.acquire()
        v =  self.__kinematics.position_degrees
        self.__mutex.release()
        return v

    def set_target(self, newtarget):
        self.__mutex.acquire()
        self.__target_pos = newtarget
        self.__mutex.release()

    def start_acquisition(self):
        self.__acquire_on = True
        self.__time_array = [0]
        self.__data = [ [], [], [], [] ]

    def stop_acquisition(self):
        self.__acquire_on = False
        return (self.__time_array, self.__data)



if __name__ == "__main__":

    controller = PositionControlTask()
    controller.start()

    while True:
        cmd_string = raw_input("Command> ")
        commands = cmd_string.split()
        if len(commands) == 0:
            continue
        if commands[0] == "pos":
            print controller.current_position()
        elif commands[0] == "target":
            controller.set_target(float(commands[1]))
        elif commands[0] == "sample":
            controller.start_acquisition()
        elif commands[0] == "show":
            (tm, data) = controller.stop_acquisition()
            pylab.figure(1)
            pylab.plot(tm[:-1], data[0], 'b-')
            pylab.plot(tm[:-1], data[1], 'r-')
            pylab.title("Velocita'")
            pylab.figure(2)
            pylab.plot(tm[:-1], data[2], 'b-')
            pylab.plot(tm[:-1], data[3], 'r-')
            pylab.title("Posizione")
            pylab.show()
        elif commands[0] == "quit":
            controller.stop()
            sys.exit(0)
        else:
            print "Invalid command"



