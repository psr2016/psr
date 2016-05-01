/*
 * fsm.h
 */

#ifndef __FSM_H
#define __FSM_H

template <class T>
class FiniteStateMachine
{
	public:
		typedef void (T::*StateHandler)();

		// Initialization, start and stop
		FiniteStateMachine(StateHandler initialState = &T::nopState);
		void start();
		void stop();

		// Run current state handler once
		void schedule();

	protected:
		// Change state
		void setNextState(StateHandler nextState, int delay = 0);

		// Get current state
		StateHandler getCurrentState() const;

		// Default state
		void nopState();

	private:
		bool running;
		StateHandler next_state, current_state;
		int counter, max_counter;
};

template <class T>
FiniteStateMachine<T>::FiniteStateMachine(StateHandler initialState)
: running(false), next_state(initialState), current_state(NULL)
{
}

template <class T>
void FiniteStateMachine<T>::start()
{
	counter = max_counter = 0;
	running = true;
}

template <class T>
void FiniteStateMachine<T>::stop()
{
	running = false;
}

template <class T>
void FiniteStateMachine<T>::setNextState(StateHandler nextState, int delay)
{
	next_state = nextState;
	max_counter = delay;
	counter = 0;
}

template <class T>
typename FiniteStateMachine<T>::StateHandler FiniteStateMachine<T>::getCurrentState() const
{
	// durante l'esecuzione di un handler restituiamo lo stato corrente,
	// altrimenti restituiamo il prossimo stato
	if (current_state != NULL)
		return current_state;
	else
		return next_state;
}

template <class T>
void FiniteStateMachine<T>::schedule()
{
	if (running)
	{
		++counter; //Ogni tic vale 20 ms
		if (counter >= max_counter)
		{
			counter = 0;
			current_state = next_state;
			(static_cast<T*>(this)->*current_state)();
			current_state = NULL;
		}
	}
}

template <class T>
void FiniteStateMachine<T>::nopState()
{
}

#endif
