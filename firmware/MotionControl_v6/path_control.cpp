#include "path_control.h"
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
void PathControl::addAbsRotation(int theta){}
void PathControl::addRelRotation(int theta){}
void PathControl::setCommand(int type)
{
	/*switch(operation[executionIndex].typeOfCommand)
	{
	
	}*/
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
