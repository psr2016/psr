#!/bin/sh
scp pi@192.168.70.20:/home/pi/git/CICCIO_v2/RaspberryPI/speed_tester/data.txt .
gnuplot -p -e "plot for[col=2:4] 'data.txt' using 1:col title columnheader(col) with lines"
gnuplot -p -e "plot for[col=5:6] 'data.txt' using 1:col title columnheader(col) with lines"

