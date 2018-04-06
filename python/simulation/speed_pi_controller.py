import pylab
from systems import *
from controllers import *
from inputs import *

delta_t = 0.01 # 10ms di tempo di campionamento
durata_simulazione = 400 # secondi

numero_punti = int(durata_simulazione / delta_t)

the_robot = Robot(5, 0.6)
controller = PI_Controller(0.05, 0.03)

times = []
speed = []
target = 15 # metri/s

for k in range(0, numero_punti):
    current = the_robot.get_speed()
    f = controller.evaluate(target, current, delta_t) 
    the_robot.evaluate(f, delta_t)

    times.append(k * delta_t)
    speed.append(the_robot.get_speed())

pylab.plot(times, speed, 'b-')
pylab.title("Velocita'")
pylab.show()


