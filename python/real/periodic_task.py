#
# periodic_task.py
#

import threading
import time

class PeriodicTask:

    def __init__(self, period):
        self.__period = period
        self.__lasttime = None
        self.__stopped= False

    def start(self):
        self.__restart()

    def __restart(self):
        threading.Timer(self.__period, self.__callback).start()

    def __callback(self):
        if self.__stopped:
            return
        current = time.time()
        self.__restart()
        if self.__lasttime is None:
            self.cycletime = 0
        else:
            self.cycletime = current - self.__lasttime
        self.__lasttime = current
        self.run()

    def run(self):
        pass

    def stop(self):
        self.__stopped = True

if __name__ == "__main__":
    # se questo file e' lanciato come main, esegui qui
    class Test(PeriodicTask):

        def run(self):
            print "Ciao mondo! (%f secs)" % (self.cycletime)

    t = Test(0.25) # 250ms
    t.start()

