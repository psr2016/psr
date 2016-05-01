

all: mass

mass: mass.cpp mass.h pid.cpp pid.h main.cpp position_controller.h position_controller.cpp
	g++  mass.cpp pid.cpp  position_controller.cpp main.cpp -o mass

clean:
	rm -f mass
