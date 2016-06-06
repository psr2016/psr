
#ifndef __PATHCONTROL_H
#define __PATHCONTROL_H

#include "position_control.h"
#include "speed_control.h"
#include "periodic_task.h"
#include "kinematics.h"

#define PATH_SIZE 10
#define DELTA 100  // 100 mm/s
#define MAX_BLOCK 10

//define per i comandi
#define ABSOLUTE_ROTATION 1
#define RELATIVE_ROTATION 2
#define CIRCULAR_ROTATION 3

struct Command
{
	int typeOfCommand;
	int xCoord;
	int yCoord;
	int distance;
	float theta;
	float radius;
};

class PathControl : public PeriodicTask{

public:
	PathControl(Kinematics & kinem, SpeedControlTask & speed_ctrl);
	void run();
	void addForward(int distance);
	void addGoToPoint(int x,int y);
	void addAbsRotation(float theta);
	void addRelRotation(float theta);
	void addCircularRotation(float theta, float radius);
	void setCommand(int type);
	bool isStop();
	void reset();
	void abort_and_getNext();
private:
	int m_executionIndex;//indice comando in esecuzione
	int m_insertIndex;//indice comando inserito
	bool m_path_finish;
	Command operation[PATH_SIZE];
	PositionControl * current_command;
	int m_block_cnt;
	Kinematics m_kinematics;
        SpeedControlTask m_speed_control;
};

extern PathControl path_control;

#endif
