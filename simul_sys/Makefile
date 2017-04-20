#
#
#

all: massa massa_molla test_ex test_pi

massa: massa_attrito.o main1.o
	g++ $^ -o $@

massa_molla: massa_molla.o input_sin.o main2.o
	g++ $^ -o $@

test_ex: ex1.o test_ex.o
	g++ $^ -o $@

test_pi: massa_attrito.o pi_controller.o pid_controller.o second_order.o test_pi.o
	g++ $^ -o $@

%.o : %.cpp
	g++ $< -c

clean:
	rm -f *.o massa massa_molla test_ex test_pi

