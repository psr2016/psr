#
# test_kinematics.py
#

from motor_driver import *
from periodic_task import *
from simple_kinematics import *

class SpeedSampler(PeriodicTask):

    def __init__(self, driver):
        PeriodicTask.__init__(self, 0.01) # 10 ms of periodicity
        self.__kinematics = Kinematics(driver)

    def run(self):
        self.__kinematics.run(self.cycletime)
        print "Current Speed ", self.__kinematics.speed



if __name__ == "__main__":

    driver = MotorDriver()
    driver.open()

    sampler = SpeedSampler(driver)
    sampler.start()

    driver.pwm(0)

