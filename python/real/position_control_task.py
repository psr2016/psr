#
# position_control_task.py
#
import sys
import threading

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
        self.__position_controller = P_Sat_Controller(5, 60) # KP = 5, 60 rpm max
        self.__mutex = threading.Lock()

    def start(self):
        self.__motor_driver.open()
        PeriodicTask.start(self)

    def run(self):
        self.__mutex.acquire()
        # run the kinematics
        self.__kinematics.run(self.cycletime)
        # do the control
        target_speed = self.__position_controller.evaluate(self.__target_pos,
                                                      self.__kinematics.position_degrees,
                                                        self.cycletime)

        controller_output = self.__speed_controller.evaluate(target_speed,
                                                       self.__kinematics.speed,
                                                       self.cycletime)
        self.__mutex.release()
        # output PWM to driver
        self.__motor_driver.pwm(controller_output)


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



if __name__ == "__main__":

    controller = PositionControlTask()
    controller.start()

    while True:
        cmd_string = raw_input("Command> ")
        commands = cmd_string.split()
        if commands[0] == "pos":
            print controller.current_position()
        elif commands[0] == "target":
            controller.set_target(float(commands[1]))
        elif commands[0] == "quit":
            controller.stop()
            sys.exit(0)
        else:
            print "Invalid command"



