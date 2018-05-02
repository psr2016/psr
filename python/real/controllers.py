#
# controllers.py
#

import math

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



class PostionControlWithProfile:

    def __init__(self, accel, max_speed, decel):
        self.__accel = accel
        self.__max_speed = max_speed
        self.__decel = decel
        self.__decel_distance = max_speed * max_speed / (2.0 * decel)
        self.__output_speed = 0 # la velocita' a cui andremo

    def evaluate(self, target_position, current_position, current_speed, delta_t):
        distance = target_position - current_position

        # calcoliamo il segno e usiamo distanze sempre positive
        if distance >= 0:
            s = 1
        else:
            s = -1
            distance = -distance

        if distance < self.__decel_distance:
            # ok siamo nella fase di decelerazione
            vel_attesa = math.sqrt(self.__max_speed * self.__max_speed - \
                                   2 * self.__decel * (self.__decel_distance - distance))
            if vel_attesa > self.__output_speed:
                # uhm... strana condizione,
                # vuol dire che siamo ancora in accelerazione (fase 1)
                # continuiamo ad accelerare
                self.__output_speed += self.__accel * delta_t
                # controlliamo se abbiamo comunque raggiunto (e superato) la velocita' attesa
                if self.__output_speed > vel_attesa:
                    self.__output_speed = vel_attesa
                # evitiamo anche di superare la velocita' massima
                if self.__output_speed > self.__max_speed:
                    self.__output_speed = self.__max_speed
            else:
                # qui siamo effettivamente in decelerazione
                self.__output_speed = vel_attesa
        else:
            # non siamo nella fase di decelerazione quindi...
            if self.__output_speed < self.__max_speed:
                # se non siamo gia' a velocita' massima, acceleriamo
                self.__output_speed += self.__accel * delta_t
                # ma evitiamo sempre di superare la velocita' massima
                if self.__output_speed > self.__max_speed:
                    self.__output_speed = self.__max_speed

        # applichiamo il segno
        return s * self.__output_speed
