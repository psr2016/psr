import pylab
from systems import *
from inputs import *

delta_t = 0.01 # 10ms di tempo di campionamento
durata_simulazione = 240 # secondi

numero_punti = int(durata_simulazione / delta_t)

the_robot = Robot(5, 0.6)

times = []
position = []
target = 15 # metri
KP = 0.025

for k in range(0, numero_punti):
    current = the_robot.get_pos()
    error = target - current
    f = KP * error
    the_robot.evaluate(f, delta_t)

    times.append(k * delta_t)
    position.append(the_robot.get_pos())

pylab.plot(times, position, 'b-')
pylab.title("Posizione")
pylab.show()


