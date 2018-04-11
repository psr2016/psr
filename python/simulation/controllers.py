#
# controllers.py
#


class P_Controller:

    def __init__(self, kp):
        self.__kp = kp

    def evaluate(self, target, measure, delta_t):
        self.__error = target - measure
        return self.__error * self.__kp

    def get_error(self):
        return self.__error


class P_Sat_Controller:

    def __init__(self, kp, sat):
        self.__kp = kp
        self.__saturation = sat

    def evaluate(self, target, measure, delta_t):
        self.__error = target - measure
        output = self.__error * self.__kp
        if output > self.__saturation:
            output = self.__saturation
        if output < -self.__saturation:
            output = -self.__saturation
        return output

    def get_error(self):
        return self.__error


class PI_Controller:

    def __init__(self, kp, ki):
        self.__kp = kp
        self.__ki = ki
        self.__intergral_term = 0

    def evaluate(self, target, measure, delta_t):
        self.__error = target - measure
        self.__intergral_term = self.__intergral_term + self.__error * delta_t
        output = self.__error * self.__kp + self.__intergral_term * self.__ki
        return output

    def get_error(self):
        return self.__error


class PI_Sat_Controller:

    def __init__(self, kp, ki, sat):
        self.__kp = kp
        self.__ki = ki
        self.__intergral_term = 0
        self.__saturation = sat

    def evaluate(self, target, measure, delta_t):
        self.__error = target - measure
        self.__intergral_term = self.__intergral_term + self.__error * delta_t
        output = self.__error * self.__kp + self.__intergral_term * self.__ki
        if output > self.__saturation:
            output = self.__saturation
        if output < -self.__saturation:
            output = -self.__saturation
        return output

    def get_error(self):
        return self.__error


class PI_Sat_AntiW_Controller:

    def __init__(self, kp, ki, sat):
        self.__kp = kp
        self.__ki = ki
        self.__intergral_term = 0
        self.__saturation = sat
        self.__saturation_flag = False

    def evaluate(self, target, measure, delta_t):
        self.__error = target - measure

        if not(self.__saturation_flag):
            self.__intergral_term = self.__intergral_term + self.__error * delta_t

        output = self.__error * self.__kp + self.__intergral_term * self.__ki

        if output > self.__saturation:
            self.__saturation_flag = True
            output = self.__saturation
        elif output < -self.__saturation:
            self.__saturation_flag = True
            output = -self.__saturation
        else:
            self.__saturation_flag = False

        return output

    def get_error(self):
        return self.__error

