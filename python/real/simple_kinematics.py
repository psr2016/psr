#
# simple_kinematics.py
#

#
# attributes:
#    speed = current speed in RPM
#    position = current position in ticks
#    position_degreees = current position in degrees
#
class Kinematics:

    def __init__(self, motor_driver):
        self.__motor_driver = motor_driver
        self.position = None

    def run(self, delta_t):
        # get current encoder
        p = self.__motor_driver.encoder()
        if self.position is None:
            # if first time speed is 0
            self.speed = 0
        else:
            # compute speed
            s = (p - self.position) / delta_t
            # convert in RPM (84000 ticks per revolution + 60 revolutions in a minute)
            self.speed  = (s / 84000.0) * 60.0

        # update position
        self.position = p
        self.position_degrees = self.position * 360.0 / 84000.0


