#
# arm_control.py
#
import pylab
import math
from systems import *
from inputs import *
from controllers import *

delta_t = 0.001 # 1ms di tempo di campionamento
durata_simulazione = 20 # secondi

numero_punti = int(durata_simulazione / delta_t)

the_arm = RoboticArm(6, 5.0)
controller = PID_Controller(100, 80, 50)

target_theta = math.radians(10)
current_theta = 0

times = []
position = []
target = []
for k in range(0, numero_punti):
    controller_output = controller.evaluate(target_theta, current_theta, delta_t)
    the_arm.evaluate(controller_output, delta_t)

    times.append(k * delta_t)
    position.append(current_theta)
    target.append(target_theta)

    current_theta = the_arm.get_theta()

pylab.figure()
pylab.plot(times, position, 'b-+')
pylab.plot(times, target, 'r-+')
pylab.title("Posizione")


pylab.show()

