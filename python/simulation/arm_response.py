#
# robot_response.py
#
import pylab
from systems import *
from inputs import *

delta_t = 0.001 # 1ms di tempo di campionamento
durata_simulazione = 15 # secondi

numero_punti = int(durata_simulazione / delta_t)

the_arm = RoboticArm(6, 5.0)

costant = Step(10)
impulse = Impulse(10)

times = []
speed = []
position = []
for k in range(0, numero_punti):
    input_signal = costant.evaluate(delta_t)
    the_arm.evaluate(input_signal, delta_t)

    times.append(k * delta_t)
    speed.append(the_arm.get_omega())
    position.append(the_arm.get_theta())

pylab.figure(1)
pylab.plot(times, speed, 'b-+')
pylab.title("Velocita' - Ingresso Costante")
pylab.figure(2)
pylab.plot(times, position, 'b-+')
pylab.title("Posizione - Ingresso Costante")


times = []
speed = []
position = []
the_arm.reset()
for k in range(0, numero_punti):
    input_signal = impulse.evaluate(delta_t)
    the_arm.evaluate(input_signal, delta_t)

    times.append(k * delta_t)
    speed.append(the_arm.get_omega())
    position.append(the_arm.get_theta())

pylab.figure(3)
pylab.plot(times, speed, 'r-+')
pylab.title("Velocita' - Ingresso Impulsivo")
pylab.figure(4)
pylab.plot(times, position, 'r-+')
pylab.title("Posizione - Ingresso Impulsivo")

pylab.show()

