
#ifndef __PATHCONTROL_H
#define __PATHCONTROL_H 

#include "periodic_task.h"
#include "speed_control.h"
#include "position_control.h"
#include "kinematics.h"

#define PATH_SIZE 10
#define DELTA 0.5
#define MAX_BLOCK 10

struct Command
{
	int typeOfCommand;
	int xCoord;
	int yCoord;
	int theata;
	int distance;
};

class PathControl : public PositionControl{

public:
	PathControl(Kinematics & kinem, SpeedControlTask & speed_ctrl,int size);
	void run();
	void addForward(int distance);
	void addGoToPoint(int x,int y);
	void addAbsRotation(int theta);
	void addRelRotation(int theta);
	void setCommand(int type);
	bool isStop();
	void abort();
private:
	int m_size;
	int m_executionIndex;//indice comando in esecuzione
	int m_insertIndex;//indice comando inserito
	bool m_path_finish;
	Command operation[PATH_SIZE];
	PeriodicTask * current_command;
	int m_block_cnt;
};

#endif
