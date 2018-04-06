#
# robot_response.py
#
import pylab
from systems import *
from inputs import *

delta_t = 0.01 # 10ms di tempo di campionamento
durata_simulazione = 120 # secondi

numero_punti = int(durata_simulazione / delta_t)

the_robot = Robot(5, 0.6)

costant = Step(10)
impulse = Impulse(10)

times = []
speed = []
position = []
for k in range(0, numero_punti):
    input_signal = costant.evaluate(delta_t)
    the_robot.evaluate(input_signal, delta_t)

    times.append(k * delta_t)
    speed.append(the_robot.get_speed())
    position.append(the_robot.get_pos())

pylab.figure(1)
pylab.plot(times, speed, 'b-+')
pylab.title("Velocita' - Ingresso Costante")
pylab.figure(2)
pylab.plot(times, position, 'b-+')
pylab.title("Posizione - Ingresso Costante")


times = []
speed = []
position = []
the_robot.reset()
for k in range(0, numero_punti):
    input_signal = impulse.evaluate(delta_t)
    the_robot.evaluate(input_signal, delta_t)

    times.append(k * delta_t)
    speed.append(the_robot.get_speed())
    position.append(the_robot.get_pos())

pylab.figure(3)
pylab.plot(times, speed, 'r-+')
pylab.title("Velocita' - Ingresso Impulsivo")
pylab.figure(4)
pylab.plot(times, position, 'r-+')
pylab.title("Posizione - Ingresso Impulsivo")

pylab.show()

