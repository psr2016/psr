#!/bin/sh
./mass $* > tmp.txt
gnuplot -p -e "plot for[col=2:3] 'tmp.txt' using 1:col title columnheader(col)"
