#
# systems.py
#

class Robot:

    def __init__(self, massa, attrito):
        self.__massa = massa
        self.__attrito = attrito
        self.__vel = 0
        self.__pos = 0

    def reset(self):
        self.__vel = 0
        self.__pos = 0

    def evaluate(self, f, delta_t):
        self.__pos = self.__pos + self.__vel * delta_t
        self.__vel = (-self.__attrito * delta_t / self.__massa + 1) * self.__vel + \
                      delta_t / self.__massa * f

    def get_speed(self):
        return self.__vel

    def get_pos(self):
        return self.__pos

    def set_pos(self, pos):
        self.__pos = pos





G = 9.81

class RoboticArm:

    def __init__(self, m, b):
        self.__mass = m
        self.__friction = b
        self.__theta = 0.0 # condizione iniziale
        self.__omega = 0.0 # condizione iniziale

    def reset(self):
        self.__theta = 0.0 # condizione iniziale
        self.__omega = 0.0 # condizione iniziale

    def evaluate(self, _input, delta_t):
        new_theta = self.__theta + delta_t * self.__omega
        new_omega = -G * delta_t * self.__theta + \
          (1 - self.__friction * delta_t / self.__mass) * self.__omega + \
          delta_t/self.__mass * _input

        self.__theta = new_theta
        self.__omega = new_omega

    def get_theta(self):
        return self.__theta

    def get_omega(self):
        return self.__omega

