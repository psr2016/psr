#
# speed_control_task.py
#
import sys
import threading

from periodic_task import *
from simple_kinematics import *
from controllers import *
from motor_driver import *

class SpeedControlTask(PeriodicTask):

    def __init__(self):
        PeriodicTask.__init__(self, 0.01) # 10ms
        self.__motor_driver = MotorDriver()
        self.__kinematics = Kinematics(self.__motor_driver)
        self.__target_speed = 0
        # KP = 50, KI = 400, Saturation = 4200
        self.__controller = PI_Sat_Controller(50, 400, 4200)
        self.__mutex = threading.Lock()

    def start(self):
        self.__motor_driver.open()
        PeriodicTask.start(self)

    def run(self):
        self.__mutex.acquire()
        # run the kinematics
        self.__kinematics.run(self.cycletime)
        # do the control
        controller_output = self.__controller.evaluate(self.__target_speed,
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

    def set_target(self, newtarget):
        self.__mutex.acquire()
        self.__target_speed = newtarget
        self.__mutex.release()



if __name__ == "__main__":

    controller = SpeedControlTask()
    controller.start()

    while True:
        cmd_string = raw_input("Command> ")
        commands = cmd_string.split()
        if commands[0] == "speed":
            print controller.current_speed()
        elif commands[0] == "target":
            controller.set_target(float(commands[1]))
        elif commands[0] == "quit":
            controller.stop()
            sys.exit(0)
        else:
            print "Invalid command"



