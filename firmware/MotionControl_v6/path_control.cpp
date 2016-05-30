#include "path_control.h"
#include "absolute_rotation.h"
#include "relative_rotation.h"
#include "circ_rotation.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>


PathControl::PathControl(Kinematics & kinem, SpeedControlTask & speed_ctrl,int size)
	:PositionControl(kinem,speed_ctrl),m_size(size),m_executionIndex(-1),m_insertIndex(0),m_path_finish(0),m_block_cnt(0)
{
	
}

void PathControl::run()
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
			current_command->on();
			
		}
		else
		{
			if(current_command!=NULL)
			{
				if(target_reached())
				{
					current_command->off();
					m_executionIndex++;
					if(m_executionIndex<PATH_SIZE)
					{

						setCommand(operation[m_executionIndex].typeOfCommand);
						current_command->on();

					}
					else	m_path_finish=1;
				}
			}
		}
	}
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
	switch(operation[m_executionIndex].typeOfCommand)
	{
		case ABSOLUTE_ROTATION: 
			//comando di rotazione assoluta richiamare il metodo 
			//evaluate_absolute_rotation(float target_angle)
			absolute_rotation.evaluate_absolute_rotation(operation[m_executionIndex].theta);
			current_command= &absolute_rotation;
			break;

		case RELATIVE_ROTATION:
			//comando di rotazione relativa richiamare il metodo
			//set_rotation_target(float angular_target)
			relative_rotation.set_rotation_target(operation[m_executionIndex].theta);
			current_command=&relative_rotation;
			break;
	
		case CIRCULAR_ROTATION:
			//comando di rotazione circolare richiamare il metodo
			//set_rotation_target(float angular_target, float radius)
			circular_rotation.set_rotation_target(operation[m_executionIndex].theta,
								operation[m_executionIndex].radius);
			current_command=&circular_rotation;						
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
		if(m_block_cnt==MAX_BLOCK)
		{
			//m_block_cnt=0;
			return true;
		}
	}
	return false;
}

void PathControl::abort()
{
	//ferma il current command, set motors(0,0),riazzerare gli index per rifare gli add
	m_speed_control.set_motors(0,0);	
	current_command->off();
	m_executionIndex=-1;
	m_insertIndex=0;
}
