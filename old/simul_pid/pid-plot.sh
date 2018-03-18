#!/bin/sh
./pid $* | gnuplot -p -e "plot '<cat' "
