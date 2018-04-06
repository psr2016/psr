#
# motor_speed_plot.py
#
import pylab
import time
from motor_driver import *

driver = MotorDriver()
driver.open()

last_encoder_value = driver.encoder()
last_current_time = time.time()
time_array = []
omega_array = []
t = 0
driver.pwm(500) # accende il motore
while t < 5:
  # calcolo di delta_t
  current_time = time.time()
  delta_t = current_time - last_current_time
  last_current_time = current_time

  # calcolo di omega (vel. angolare)
  encoder_value = driver.encoder()
  current_speed = (encoder_value - last_encoder_value) / delta_t
  last_encoder_value = encoder_value
  # convert in RPM (84000 ticks per revolution + 60 revolutions in a minute)
  current_speed = (current_speed / 84000.0) * 60.0

  time_array.append(t)
  omega_array.append(current_speed)
  t = t + delta_t

driver.pwm(0) # spegne il motore
pylab.plot(time_array, omega_array)
pylab.show()

