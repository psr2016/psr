#
# inputs.py
#

class Impulse:

    def __init__(self, value):
        self.__value = value
        self.__first_time = True

    def evaluate(self, delta_t):
        if self.__first_time:
            self.__first_time = False
            return self.__value
        else:
            return 0.0

    def reset(self):
        self.__first_time = True


class Step:

    def __init__(self, value):
        self.__value = value

    def evaluate(self, delta_t):
        return self.__value

    def reset(self):
        pass


