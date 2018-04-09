#
# speed_control_task.py
#
import sys
import threading
import pylab

from periodic_task import *
from simple_kinematics import *
from controllers import *
from motor_driver import *

class SpeedControlTask(PeriodicTask):

    def __init__(self, accel):
        PeriodicTask.__init__(self, 0.01) # 10ms
        self.__motor_driver = MotorDriver()
        self.__kinematics = Kinematics(self.__motor_driver)
        self.__target_speed = 0
        self.__new_target_speed = 0
        # KP = 50, KI = 400, Saturation = 4200
        self.__controller = PI_Sat_Controller(50, 400, 4200)
        self.__mutex = threading.Lock()
	self.__acquire_on = False
        self.__accel = accel

    def start(self):
        self.__motor_driver.open()
        PeriodicTask.start(self)

    def start_acquisition(self):
        self.__acquire_on = True
	self.__time_array = [0]
	self.__current_speed_array = []
	self.__target_speed_array = []

    def stop_acquisition(self):
        self.__acquire_on = False
        return (self.__time_array, self.__current_speed_array, self.__target_speed_array)

    def run(self):
        self.__mutex.acquire()
        # run the kinematics
        self.__kinematics.run(self.cycletime)
        # do the control
        controller_output = self.__controller.evaluate(self.__target_speed,
                                                       self.__kinematics.speed,
                                                       self.cycletime)
        if self.__acquire_on:
                self.__current_speed_array.append(self.__kinematics.speed)
                self.__target_speed_array.append(self.__target_speed)
                self.__time_array.append(self.__time_array[-1] + self.cycletime)

        if self.__target_speed != self.__new_target_speed:
                if self.__new_target_speed > self.__target_speed:
                    self.__target_speed += self.__accel * self.cycletime
                    # controllo se ho superato il nuovo target
                    if self.__target_speed > self.__new_target_speed:
                        self.__target_speed = self.__new_target_speed
                else:
                    self.__target_speed -= self.__accel * self.cycletime
                    # controllo se ho superato (in basso) il nuovo target
                    if self.__target_speed < self.__new_target_speed:
                        self.__target_speed = self.__new_target_speed

        self.__mutex.release()
        # output PWM to driver
        self.__motor_driver.pwm(controller_output)


    def current_speed(self):
        self.__mutex.acquire()
        v =  self.__kinematics.speed
        self.__mutex.release()
        return v

    def set_target(self, newtarget):
        self.__mutex.acquire()
        self.__new_target_speed = newtarget
        self.__mutex.release()



if __name__ == "__main__":

    controller = SpeedControlTask(10) # 10 RPM / secondo
    controller.start()

    while True:
        cmd_string = raw_input("Command> ")
        commands = cmd_string.split()
        if commands[0] == "speed":
            print controller.current_speed()
        elif commands[0] == "target":
            controller.set_target(float(commands[1]))
        elif commands[0] == "sample":
            controller.start_acquisition()
        elif commands[0] == "show":
            (tm, cur, targ) = controller.stop_acquisition()
            pylab.plot(tm[:-1], cur, 'b-')
            pylab.plot(tm[:-1], targ, 'r-')
            pylab.show()
        elif commands[0] == "quit":
            controller.stop()
            sys.exit(0)
        else:
            print "Invalid command"



