
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
	m_path_status(PATH_FINISH),
	current_command(NULL),
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
					m_path_status=PATH_FINISH;
					m_executionIndex++;//then passo al comando successivo
					if(m_executionIndex<m_insertIndex)
						setCommand(operation[m_executionIndex].typeOfCommand);
					else
					{
						reset(); //ho finito i comandi
					}
				}
			}
		}
		else  //robot non sta eseguendo alcun comando
		{
			//if(m_executionIndex==-1;)
				//m_path_status=PATH_FINISH;
			m_executionIndex++;  //provo ad andare avanti
			if(m_executionIndex<m_insertIndex)  //se possibile
			{
				setCommand(operation[m_executionIndex].typeOfCommand);
			}
			else  //ho esaurito i comandi da esegure
			{
				// ho finito
				reset();
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
			m_path_status=PATH_BUSY;
			break;

		case RELATIVE_ROTATION:
			//comando di rotazione relativa richiamare il metodo
			//set_rotation_target(float angular_target)
			relative_rotation.set_rotation_target(operation[m_executionIndex].theta);
			current_command = &relative_rotation;
                        current_command->on();
			m_path_status=PATH_BUSY;
			break;
	
		case CIRCULAR_ROTATION:
			//comando di rotazione circolare richiamare il metodo
			//set_rotation_target(float angular_target, float radius)
			circular_rotation.set_rotation_target(operation[m_executionIndex].theta,
								operation[m_executionIndex].radius);
			current_command = &circular_rotation;						
                        current_command->on();
			m_path_status=PATH_BUSY;
			break;
		case FOLLOW_LINE:
			//comando di follow line richiamare il metodo
			//set_qualcosa.......................

			//metodo.....			
			//current_command=&follow_line;
			current_command->on();			
			m_path_status=PATH_BUSY;			
			break;
		default:
			m_path_status=PATH_FINISH;
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
        else 
	{
            led_off();
            m_block_cnt = 0;
        }
	return false;
}

void PathControl::reset()
{
	//reset del pathControll
	//spengo i motori
	m_speed_control.set_motors(0,0);
	
	//spengo il comando
	if(current_command!=NULL)	
		current_command->off();		

	//reset di executionIndex, inserIndex
	m_executionIndex=-1;	
	m_insertIndex=0;			
	//set status 
	if(m_block_cnt>=MAX_BLOCK)		
		m_path_status=PATH_BLOCKED;
	else
		m_path_status=PATH_FINISH;

	m_block_cnt = 0;			
	current_command=NULL;
}
