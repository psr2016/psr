
#include "defines.h"

#include <p33FJ128MC802.h>


#include "path_control.h"
#include "absolute_rotation.h"
#include "relative_rotation.h"
#include "circ_rotation.h"
#include "gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>


PathControl::PathControl(Kinematics & kinem, SpeedControlTask & speed_ctrl)
	:PeriodicTask("path_control", PATH_CONTROL_PERIOD, TIME_UNIT, PATH_CONTROL_JITTER),
	m_executionIndex(-1),
	m_insertIndex(0),
	m_path_finish(0),
	m_block_cnt(0),
	m_kinematics(kinem),
        m_speed_control(speed_ctrl)
{
	
}

void PathControl::run()
{
	if(m_insertIndex>0)//controllo se ci sono comandi inseriti in struct
	{
		//comandi presenti nella struct
		
		if(current_command!=NULL) //robot in funzione, sta eseguendo un comando
		{
			if(isStop())//controllo se ci sono le ruote bloccate
			{
                            led_on();
                            reset();//then ruote bloccate passo al comando successivo
			}
			else
			{	if(current_command->target_reached())// controllo il terget del comando se è stato raggiunto
				{	current_command->off();//spengo il comando
					m_executionIndex++;//then passo al comando successivo
					if(m_executionIndex<m_insertIndex)
						setCommand(operation[m_executionIndex].typeOfCommand);
					else
					{
						m_path_finish=1;
						reset();
					}
				}
			}
		}
		else  //robot non sta eseguendo alcun comando
		{
			if(m_executionIndex==-1)
				m_path_finish=0;
			m_executionIndex++;  //provo ad andare avanti
			if(m_executionIndex<m_insertIndex)  //se possibile
			{
				setCommand(operation[m_executionIndex].typeOfCommand);
			}
			else  //ho esaurito i comandi da esegure
			{
				m_path_finish=1;  // ho finito
				reset();	  //spengo i motori eresetto gli indici
			}
		}
	}//else bypass del metodo run
}

//add specifico per ogni tipo di comando
void PathControl::addForward(int distance){}

void PathControl::addGoToPoint(int x,int y){}

void PathControl::addAbsRotation(float theta)
{
	if(m_insertIndex<PATH_SIZE)
	{
		operation[m_insertIndex].typeOfCommand=ABSOLUTE_ROTATION;
		operation[m_insertIndex].xCoord=0;
		operation[m_insertIndex].yCoord=0;
		operation[m_insertIndex].distance=0;
		operation[m_insertIndex].theta=theta;
		operation[m_insertIndex].radius=0;
		m_insertIndex++;
	}
}

void PathControl::addRelRotation(float theta)
{
	if(m_insertIndex<PATH_SIZE)
	{
		operation[m_insertIndex].typeOfCommand=RELATIVE_ROTATION;
		operation[m_insertIndex].xCoord=0;
		operation[m_insertIndex].yCoord=0;
		operation[m_insertIndex].distance=0;
		operation[m_insertIndex].theta=theta;
		operation[m_insertIndex].radius=0;
		m_insertIndex++;
	}
}

void PathControl::addCircularRotation(float theta, float radius)
{
	if(m_insertIndex<PATH_SIZE)
	{
		operation[m_insertIndex].typeOfCommand=CIRCULAR_ROTATION;
		operation[m_insertIndex].xCoord=0;
		operation[m_insertIndex].yCoord=0;
		operation[m_insertIndex].distance=0;
		operation[m_insertIndex].theta=theta;
		operation[m_insertIndex].radius=radius;
		m_insertIndex++;
	}
}
void PathControl::setCommand(int type)
{
	switch(type)
	{
		case ABSOLUTE_ROTATION:
			//comando di rotazione assoluta richiamare il metodo
			//evaluate_absolute_rotation(float target_angle)
			absolute_rotation.evaluate_absolute_rotation(operation[m_executionIndex].theta);
			current_command = &absolute_rotation;
                        current_command->on();
			break;

		case RELATIVE_ROTATION:
			//comando di rotazione relativa richiamare il metodo
			//set_rotation_target(float angular_target)
			relative_rotation.set_rotation_target(operation[m_executionIndex].theta);
			current_command = &relative_rotation;
                        current_command->on();
			break;
	
		case CIRCULAR_ROTATION:
			//comando di rotazione circolare richiamare il metodo
			//set_rotation_target(float angular_target, float radius)
			circular_rotation.set_rotation_target(operation[m_executionIndex].theta,
								operation[m_executionIndex].radius);
			current_command = &circular_rotation;						
                        current_command->on();
			break;
		default:
			break;
	}
	
}

bool PathControl::isStop()
{
	if(fabs(m_kinematics.speed_left()-m_speed_control.get_target_left())>DELTA ||
		fabs(m_kinematics.speed_right()-m_speed_control.get_target_right())>DELTA)
	{
            m_block_cnt++;
            if(m_block_cnt>=MAX_BLOCK) {
                return true;
            }
	}
        else {
            led_off();
            m_block_cnt = 0;
        }

	return false;
}

void PathControl::reset()
{
	//reset del pathControll
	m_speed_control.set_motors(0,0);	//spengo i motori
	if(current_command!=NULL)	
		current_command->off();		//spengo il comando
	m_executionIndex=-1;			//reset di executionIndex
	m_insertIndex=0;			//reset di insertIndex
	current_command=NULL;
}

void PathControl::abort_and_getNext()
{
	m_speed_control.set_motors(0,0);	//spegnere i motori
	if(current_command!=NULL)	
		current_command->off();		
	m_executionIndex++;
	if(m_executionIndex<m_insertIndex)
		setCommand(operation[m_executionIndex].typeOfCommand);
	else
	{
		m_path_finish=1;
		reset();
	}
}

/*if(m_insertIndex>0)
	{
		if(isStop())
		{
			abort();
		}
		else
		{
			if(m_executionIndex == -1)
			{

				m_executionIndex++;
				setCommand(operation[m_executionIndex].typeOfCommand);
				if (current_command!=NULL)
					current_command->on();
				else
					abort();
			
			}
			else
			{
				if(current_command!=NULL)
				{
					if(current_command->target_reached())
					{
						current_command->off();
						m_executionIndex++;
						if(m_executionIndex<m_insertIndex)
						{

							setCommand(operation[m_executionIndex].typeOfCommand);
							if(current_command!=NULL)
								current_command->on();
							else
								abort();

						}
						else
						{
                                                    m_path_finish=1;
                                                    reset();
                                                }
					}
				}
				else	
				{
					if(m_executionIndex<m_insertIndex)
					{
						setComand(operation[m_executionIndex].typeOfCommand);
						current_command->on();
					}
					else
						reset();
				}
			}
		}
	}*/
