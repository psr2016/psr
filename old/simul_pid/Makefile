

all: massa pid

massa: massa.c
	gcc massa.c -o massa

pid: pid.c
	gcc pid.c -o pid

plot:
	./massa | gnuplot -p -e "plot '<cat' "

pid-plot:
	./pid | gnuplot -p -e "plot '<cat' "
