import pylab
from systems import *
from controllers import *
from inputs import *

delta_t = 0.01 # 10ms di tempo di campionamento
durata_simulazione = 5 # secondi

numero_punti = int(durata_simulazione / delta_t)

the_robot = Robot(1, 0.5) # M = 1Kg, friction = 0.5
#controller = PI_Controller(6, 3)
#controller = PI_Sat_Controller(6, 3, 10)
controller = PI_Sat_AntiW_Controller(6, 3, 10)

times = []
speed = []
output = []
target = 8 # metri/s

for k in range(0, numero_punti):
    current = the_robot.get_speed()
    f = controller.evaluate(target, current, delta_t)
    the_robot.evaluate(f, delta_t)

    times.append(k * delta_t)
    speed.append(the_robot.get_speed())
    output.append(f)

pylab.plot(times, speed, 'r-+')
pylab.ylim([0, 10])
pylab.xlim([-0.5, 5])
pylab.title("Velocita'")
pylab.show()


