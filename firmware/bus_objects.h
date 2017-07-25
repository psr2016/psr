/*
 * bus_objects.h
 */

#ifndef __BUS_OBJECTS
#define __BUS_OBJECTS

// -------------- ROBOT POSITION ---------------------
#define ROBOT_POSITION_CAN_ID        0x3E3
#define OTHER_ROBOT_POSITION_CAN_ID  0x3E5

typedef struct {
    short x;
    short y;
    short deg100;
    unsigned char flags;
    unsigned char bumpers;
}  __attribute__((packed)) t_can_robot_position;


typedef struct {
    float L;
    float R;
}  __attribute__((packed)) t_can_robot_motion;

// -------------- ROBOT VELOCITY ---------------------
#define ROBOT_VELOCITY_CAN_ID   0x3E4

typedef struct {
    float linear_speed;
    char padding[4];
}  __attribute__((packed)) t_can_robot_velocity;

// -------------- ROBOT WHEELS VELOCITY ---------------------
#define ROBOT_WHEELS_VELOCITY_CAN_ID   0x3E7

typedef struct {
    short left_speed;
    short right_speed;
    short left_pwm;
    short right_pwm;
}  __attribute__((packed)) t_can_robot_wheels_velocity;


// -------------- GYRO RAW DATA ---------------------
#define GYRO_RAW_DATA_CAN_ID   0x3EF

typedef struct {
    short x;
    short y;
    short z;
    short padding;
}  __attribute__((packed)) t_can_gyro_raw_data;


// -------------- OBSTACLE_AVOIDANCE ---------------------
#define OBSTACLE_AVOIDANCE_CAN_ID   0x3D0

#define CAN_OBSTACLE_AVOIDANCE_MSG_OBSTACLEDETECTED	1
#define CAN_OBSTACLE_AVOIDANCE_MSG_OBSTACLEPROCESSED	2
#define CAN_OBSTACLE_AVOIDANCE_MSG_RESET		3
#define CAN_OBSTACLE_AVOIDANCE_MSG_ENABLE		4
#define CAN_OBSTACLE_AVOIDANCE_MSG_DISABLE		5
#define CAN_OBSTACLE_AVOIDANCE_MSG_ACKOBSTACLEPROCESSED	6
#define CAN_OBSTACLE_AVOIDANCE_MSG_SET_NEARONLY_FLAG	7
#define CAN_OBSTACLE_AVOIDANCE_MSG_CLR_NEARONLY_FLAG	8

typedef struct {
    unsigned char msg_type;
    char padding[3];
    float obstacle_direction; // [-180, 180], 0 = front
}  __attribute__((packed)) t_can_obstacle_avoidance;

// -------------- OBSTACLE_AVOIDANCE_QUERY ---------------------
#define OBSTACLE_AVOIDANCE_QUERY_CAN_ID   0x7E5

#define CAN_OBSTACLE_AVOIDANCE_QUERY_MSG_REQUEST	0
#define CAN_OBSTACLE_AVOIDANCE_QUERY_MSG_OBSTFOUND	1
#define CAN_OBSTACLE_AVOIDANCE_QUERY_MSG_OBSTNOTFOUND	2

typedef struct {
    unsigned char msg_type;
    char padding[3];
    float obstacle_direction; // [-180, 180], 0 = front
}  __attribute__((packed)) t_can_obstacle_avoidance_query;

// -------------- MOTION_COMMANDS ---------------------
#define MOTION_COMMAND_CAN_ID   0x3E1

typedef struct {
    unsigned char _cmd;
    char parameters[7];
}  __attribute__((packed)) t_can_motion_command;


// -------------- TELEMETRY DATA ---------------------
// TO BE REMOVED -----------------------------------------
#define TELEMETRY_DATA_CAN_ID   0x3E7

typedef struct {
    unsigned char frame_id;
    unsigned char sub_id;
    short index;
    short value_left;
    short value_right;
}  __attribute__((packed)) t_can_telemetry_data;


typedef struct {
    unsigned char frame_id;
    unsigned char sub_id;
    float value;
    unsigned char bumper;
    unsigned char _padding;
}  __attribute__((packed)) t_can_telemetry_enconder_data;
// ------------------------------------------------------------


// -------------- LIFT TELEMETRY DATA ---------------------
#define LIFT_TELEMETRY_DATA_CAN_ID   0x3E8

typedef struct {
    unsigned char frame_id;
    unsigned char sub_id;
    float value;
    unsigned char bumper;
    unsigned char _padding;
}  __attribute__((packed)) t_can_lift_telemetry_encoder_data;


// -------------- SERVO POSITION ---------------------
#define SERVO_POSITION_CAN_ID   0x77F

typedef struct {
    unsigned char servo_num;
    unsigned short position;
    unsigned char _padding[5];
}  __attribute__((packed)) t_servo_position;

#define SERVO_POSITION_AUTOMATION_COMMAND	0xFF

#define SERVO_POSITION_GRANDE_CUP_OFF_SX		0x00
#define SERVO_POSITION_GRANDE_CUP_RITRAI_SX		0x01
#define SERVO_POSITION_GRANDE_CUP_RILASCIA_SX		0x02
#define SERVO_POSITION_GRANDE_CUP_STARTGRAB_SX		0x03
#define SERVO_POSITION_GRANDE_CUP_ABBASSA_SX		0x04
#define SERVO_POSITION_GRANDE_CUP_SOLLEVA_SX		0x05
#define SERVO_POSITION_GRANDE_CUP_SOLLEVA_POCO_SX	0x06
#define SERVO_POSITION_GRANDE_CUP_INIT_SX		0X07
#define SERVO_POSITION_GRANDE_CUP_OFF_DX		0x08
#define SERVO_POSITION_GRANDE_CUP_RITRAI_DX		0x09
#define SERVO_POSITION_GRANDE_CUP_RILASCIA_DX		0x0A
#define SERVO_POSITION_GRANDE_CUP_STARTGRAB_DX		0x0B
#define SERVO_POSITION_GRANDE_CUP_ABBASSA_DX		0x0C
#define SERVO_POSITION_GRANDE_CUP_SOLLEVA_DX		0x0D
#define SERVO_POSITION_GRANDE_CUP_SOLLEVA_POCO_DX	0x0E
#define SERVO_POSITION_GRANDE_CUP_INIT_DX		0X0F

#define SERVO_POSITION_GRANDE_STANDGRAB_OFF		0x20 // 32
#define SERVO_POSITION_GRANDE_STANDGRAB_HOME		0x21 // 33
#define SERVO_POSITION_GRANDE_STANDGRAB_START_SX	0x22 // 34
#define SERVO_POSITION_GRANDE_STANDGRAB_START_DX	0x23 // 35
#define SERVO_POSITION_GRANDE_STANDGRAB_STOP_SX		0x24 // 36
#define SERVO_POSITION_GRANDE_STANDGRAB_STOP_DX		0x25 // 37
#define SERVO_POSITION_GRANDE_STANDGRAB_LOAD_SX		0x26 // 38
#define SERVO_POSITION_GRANDE_STANDGRAB_LOAD_DX		0x27 // 39
#define SERVO_POSITION_GRANDE_STANDGRAB_POSITIONMID_SX	0x28 // 40
#define SERVO_POSITION_GRANDE_STANDGRAB_POSITIONMID_DX	0x29 // 41
#define SERVO_POSITION_GRANDE_STANDGRAB_RELEASE_SX	0x2A // 42
#define SERVO_POSITION_GRANDE_STANDGRAB_RELEASEDOWN_SX	0x2B // 43
#define SERVO_POSITION_GRANDE_STANDGRAB_RELEASE_DX	0x2C // 44
#define SERVO_POSITION_GRANDE_STANDGRAB_RELEASEDOWN_DX	0x2D // 45
#define SERVO_POSITION_GRANDE_STANDGRAB_CHIUDIPARETE_SX	0x2E // 46
#define SERVO_POSITION_GRANDE_STANDGRAB_CHIUDIPARETE_DX	0x2F // 47
#define SERVO_POSITION_GRANDE_STANDGRAB_NBA_POSITION_SX 0x30 // 48
#define SERVO_POSITION_GRANDE_STANDGRAB_NBA_POSITION_DX 0x31 // 49

#define SERVO_POSITION_GRANDE_VALVOLA_SX_OFF    0x80   // 128
#define SERVO_POSITION_GRANDE_VALVOLA_SX_ON     0x81   // 129
#define SERVO_POSITION_GRANDE_VALVOLA_DX_OFF    0x82   // 130
#define SERVO_POSITION_GRANDE_VALVOLA_DX_ON     0x83   // 131
#define SERVO_POSITION_GRANDE_VACUUM_OFF        0x84   // 132
#define SERVO_POSITION_GRANDE_VACUUM_ON         0x85   // 133
#define SERVO_POSITION_GRANDE_CUPGRAB_HOME      0x86   // 134
#define SERVO_POSITION_GRANDE_CUPGRAB_READY     0x87   // 135
#define SERVO_POSITION_GRANDE_CUPGRAB_PICK      0x88   // 136
#define SERVO_POSITION_GRANDE_CUPGRAB_RELEASE   0x89   // 137
#define SERVO_POSITION_GRANDE_CUPGRAB_OFF       0x8A   // 138

#define SERVO_POSITION_GRANDE_PALETTA_HOME       0x90   // 144
#define SERVO_POSITION_GRANDE_PALETTA_PRERELEASE 0x91   // 145
#define SERVO_POSITION_GRANDE_PALETTA_CENTER     0x92   // 146
#define SERVO_POSITION_GRANDE_PALETTA_SX         0x93   // 147
#define SERVO_POSITION_GRANDE_PALETTA_DX         0x94   // 148
#define SERVO_POSITION_GRANDE_PALETTA_OFF        0x95   // 149

// -------------- SERVO FSM STATUS ---------------------
#define SERVO_STATUS_GRANDE1_CAN_ID   0x408 // updates from RCServoCAN-grande1
#define SERVO_STATUS_GRANDE2_CAN_ID   0x409 // updates from RCServoCAN-grande2

typedef struct {
    unsigned char busy_flags; // 1=left_cup_busy, 2=right_cup_busy
    unsigned char nonempty_flags; // 1=left_cup_nonempty, 2=right_cup_nonempty
    unsigned char _padding[6];
}  __attribute__((packed)) t_servo_status_grande1;

typedef struct {
    unsigned char busy_flags; // 1=center_cup_busy, 2=left_stand_busy, 4=right_stand_busy
    unsigned char nonempty_flags; // 1=center_cup_nonempty, 2=left_ball_present, 4=right_ball_present
    unsigned char left_stand_count, right_stand_count;
    unsigned char _padding[4];
}  __attribute__((packed)) t_servo_status_grande2;

// -------------- SERVO GPIO ---------------------
#define SERVO_GPIO_CAN_ID   0x3EC

typedef struct {
    unsigned char board_num;
    unsigned char s1;
    unsigned char s2;
    unsigned char o1;
    unsigned char o2;
    unsigned char o3;
    unsigned char _padding[2];
}  __attribute__((packed)) t_servo_gpio;

// --------------------------------------------------------------------------
// ------------------ SPECIFIC STRUCTURES FOR MOTION COMMANDS ---------------
// --------------------------------------------------------------------------

#define MOTION_COMMAND_SPEED_PID               0x01
#define MOTION_COMMAND_DISTANCE_CONTROLLER_1   0x02
#define MOTION_COMMAND_DISTANCE_CONTROLLER_2   0x03
#define MOTION_COMMAND_HEADING_CONTROLLER_1    0x04
#define MOTION_COMMAND_HEADING_CONTROLLER_2    0x05
#define MOTION_COMMAND_POINT_CONTROLLER_1      0x06
#define MOTION_COMMAND_POINT_CONTROLLER_2      0x07
#define MOTION_COMMAND_POINT_CONTROLLER_3      0x08
#define MOTION_COMMAND_LINE_CONTROLLER_1       0x09
#define MOTION_COMMAND_LINE_CONTROLLER_2       0x0a
#define MOTION_COMMAND_LINE_CONTROLLER_3       0x0b
#define MOTION_COMMAND_CIRCULAR_ROTATION_CONTROLLER_1   0x0c
#define MOTION_COMMAND_CIRCULAR_ROTATION_CONTROLLER_2   0x0d
#define MOTION_COMMAND_CLEAR_POSITION_VALID_FLAG    0x0e
#define MOTION_COMMAND_SET_POSITION_VALID_FLAG      0x0f

#define MOTION_COMMAND_CLEAR_SAMPLE            0x40
#define MOTION_COMMAND_START_SAMPLE            0x41
#define MOTION_COMMAND_STOP_SAMPLE             0x42
#define MOTION_COMMAND_SEND_SAMPLE             0x43

#define MOTION_COMMAND_GYRO_CONTROLLER         0x50

#define MOTION_COMMAND_SET_SPEED_NO_LOCK       0x7f
#define MOTION_COMMAND_SET_PMW                 0x80
#define MOTION_COMMAND_SET_SPEED               0x81
#define MOTION_COMMAND_STOP_AND_FREE           0x82
#define MOTION_COMMAND_STOP_AND_BRAKE          0x83
#define MOTION_COMMAND_SET_CURRENT_POSITION    0x84

#define MOTION_COMMAND_FORWARD_TO_DISTANCE     0x85
#define MOTION_COMMAND_FORWARD_TO_POINT        0x86
#define MOTION_COMMAND_LINE_TO_POINT           0x87
#define MOTION_COMMAND_ROTATE_RELATIVE         0x88
#define MOTION_COMMAND_ROTATE_ABSOLUTE         0x89
#define MOTION_COMMAND_ROTATE_CIRCULAR         0x8a
#define MOTION_COMMAND_HEADING_TO              0x8b
#define MOTION_COMMAND_BUMP_AND_SET_X          0x8c
#define MOTION_COMMAND_BUMP_AND_SET_Y          0x8d
#define MOTION_COMMAND_SIMPLE_BUMP             0x8e
#define MOTION_COMMAND_GO_WITH_OFFSET          0x8f
#define MOTION_COMMAND_HEADING_TO_WITH_OFFSET  0x90

#define MOTION_COMMAND_SET_ERROR_TO_MINIMAL_SPEED 0xf6
#define MOTION_COMMAND_SET_ANTICIPATION_GAIN   0xf7
#define MOTION_COMMAND_SET_ERROR_TOLERANCE     0xf8
#define MOTION_COMMAND_SET_MINIMAL_SPEED       0xf9
#define MOTION_COMMAND_SET_KX                  0xfa
#define MOTION_COMMAND_SET_KY                  0xfb
#define MOTION_COMMAND_SET_TELEMETRY_MODE      0xfc
#define MOTION_COMMAND_SET_WHEEL_RADIUS_LEFT   0xfd
#define MOTION_COMMAND_SET_WHEEL_RADIUS_RIGHT  0xfe
#define MOTION_COMMAND_SET_WHEEL_DISTANCE      0xff

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    char kp_m;
    char kp_e;
    char ki_m;
    char ki_e;
    char kd_m;
    char kd_e;
    unsigned char _padding;
} __attribute__((packed)) t_command_speed_pid;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    char kp_m;
    char kp_e;
    char ki_m;
    char ki_e;
    char v_m;
    char v_e;
    unsigned char _padding;
} __attribute__((packed)) t_command_gyro_controller;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    short  max_speed;
    short  accel;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_distance_controller_1;

typedef struct {
    unsigned char _cmd;
    short  decel;
    char kd_m;
    char kd_e;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_distance_controller_2;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    short  max_speed;
    short  accel;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_heading_controller_1;

typedef struct {
    unsigned char _cmd;
    short  decel;
    char kd_m;
    char kd_e;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_heading_controller_2;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    short  max_speed;
    short  accel;
    short  decel;
    unsigned char _padding[1];
} __attribute__((packed)) t_command_point_controller_1;

typedef struct {
    unsigned char _cmd;
    short  max_speed_h;
    short  accel_h;
    short  decel_h;
    unsigned char _padding[1];
} __attribute__((packed)) t_command_point_controller_2;

typedef struct {
    unsigned char _cmd;
    char kd_m;
    char kd_e;
    char kd_h_m;
    char kd_h_e;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_point_controller_3;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    short  max_speed;
    short  accel;
    short  decel;
    unsigned char _padding[1];
} __attribute__((packed)) t_command_line_controller_1;

typedef struct {
    unsigned char _cmd;
    char kd_m;
    char kd_e;
    char kp_h_m;
    char kp_h_e;
    char kp_line_m;
    char kp_line_e;
    unsigned char _padding[1];
} __attribute__((packed)) t_command_line_controller_2;

typedef struct {
    unsigned char _cmd;
    short change_threshold;
    unsigned char _padding[5];
} __attribute__((packed)) t_command_line_controller_3;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    short  max_speed;
    short  accel;
    short  decel;
    unsigned char _padding[1];
} __attribute__((packed)) t_command_circular_rotation_controller_1;

typedef struct {
    unsigned char _cmd;
    char kd_m;
    char kd_e;
    char kp_h_m;
    char kp_h_e;
    char kp_dist_m;
    char kp_dist_e;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_circular_rotation_controller_2;

// -----------------------------------------------------------------
// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    short  left, right;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_set_pwm_speed;

typedef struct {
    unsigned char _cmd;
    short  x, y, deg100;
    unsigned char _padding[1];
} __attribute__((packed)) t_command_set_current_position;

typedef struct {
    unsigned char _cmd;
    short  distance;
    unsigned char _padding[5];
} __attribute__((packed)) t_command_forward_to_distance;

typedef struct {
    unsigned char _cmd;
    short  degrees;
    unsigned char _padding[5];
} __attribute__((packed)) t_command_rotate;

typedef struct {
    unsigned char _cmd;
    short  x;
    short  y;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_forward_to_point;

typedef struct {
    unsigned char _cmd;
    short  x;
    short  y;
    unsigned char backward;
    unsigned char _padding[2];
} __attribute__((packed)) t_command_line_to_point;

typedef struct {
    unsigned char _cmd;
    short  degrees;
    short  x;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_rotate_circular;

typedef struct {
    unsigned char _cmd;
    short  x;
    short  y;
    unsigned char with_back;
    unsigned char _padding[2];
} __attribute__((packed)) t_command_heading_to;

typedef struct {
    unsigned char _cmd;
    short  x;
    short  hdg;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_bump_and_set_x;

typedef struct {
    unsigned char _cmd;
    short  y;
    short  hdg;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_bump_and_set_y;

typedef struct {
    unsigned char _cmd;
    unsigned char _padding[7];
} __attribute__((packed)) t_command_simple_bump;

typedef struct {
    unsigned char _cmd;
    short x;
    short y;
    signed char high_offs_x;
    signed char high_offs_y;
    unsigned char lowbits_offs_xy;
} __attribute__((packed)) t_command_go_with_offset;

// --------------------------------------------------------------------------

typedef struct {
    unsigned char _cmd;
    float K;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_set_K;

typedef struct {
    unsigned char _cmd;
    float wheel_radius;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_set_wheel_radius;

typedef struct {
    unsigned char _cmd;
    float wheel_distance;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_set_wheel_distance;

typedef struct {
    unsigned char _cmd;
    unsigned char odometry_mode;
    unsigned char speed_telemetry;
    unsigned char _padding[5];
} __attribute__((packed)) t_command_set_telemetry_mode;

typedef struct {
    unsigned char _cmd;
    unsigned int linear_speed;
    unsigned int rotation_10_speed;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_set_minimal_speed;

typedef struct {
    unsigned char _cmd;
    unsigned int linear_10_error;
    unsigned int heading_10_error;
    unsigned int heading_10_to_minimal_speed;
    unsigned char _padding;
} __attribute__((packed)) t_command_set_error_tolerance;

typedef struct {
    unsigned char _cmd;
    unsigned int linear_gain;
    unsigned int heading_gain;
    unsigned char _padding[3];
} __attribute__((packed)) t_command_set_anticipation_gain;

// -------------- POSITION_COMMANDS ---------------------
#define POSITION_COMMAND_CAN_ID   0x7E1

#define POSITION_PID_1          0x01
#define POSITION_PID_2          0x02
#define POSITION_OFF            0x10
#define POSITION_HOME           0x11
#define POSITION_GO             0x12
#define POSITION_TELEMETRY      0x13
#define POSITION_PWM            0x14

typedef struct {
    unsigned char _cmd;
    unsigned char axis;
    int position;
    unsigned char _padding[4];
} __attribute__((packed)) t_command_position_generic;

// -------------- VALVE_COMMANDS ---------------------
#define VALVE_COMMAND_CAN_ID   0x7F9

typedef struct {
    unsigned char valve;
    unsigned char cmd;
    unsigned char _padding[6];
} __attribute__((packed)) t_command_valve;

// -------------- REMOTE_RTSP (programming over CAN) ---------------------
#define REMOTE_RTSP_MAX_NODES		16 // numero massimo di nodi (0-15, il nodo 0 è il coordinatore)
#define REMOTE_RTSP_CAN_ID(node_id)	(0x6F0 + (int)(node_id))
#define REMOTE_RTSP_COORD_ID		0

#define REMOTE_RTSP_COMMAND_GET_INFO		0
#define REMOTE_RTSP_COMMAND_PAGE_CRC		1
#define REMOTE_RTSP_COMMAND_PAGE_ERASE		2
#define REMOTE_RTSP_COMMAND_ROW_SELECT		3
#define REMOTE_RTSP_COMMAND_ROW_PUTDATA		4
#define REMOTE_RTSP_COMMAND_ROW_FLUSH		5
#define REMOTE_RTSP_COMMAND_ROW_GETDATA		6
#define REMOTE_RTSP_COMMAND_RESET		7
typedef struct
{
	unsigned char cmd : 4;
	unsigned char seqnum : 4;
	union
	{
		struct
		{
			char _padding;
			unsigned int data;
			unsigned long target_address;
		} __attribute__((packed));
		unsigned char payload[7];
	};
}  __attribute__((packed)) t_can_remote_rtsp_command;

typedef struct
{
	unsigned char _padding : 4;
	unsigned char seqnum_echo : 4;
	char _padding2;
	union
	{
		struct
		{
			unsigned int data;
			unsigned int data2;
		} __attribute__((packed));
		char payload[6];
	};
}  __attribute__((packed)) t_can_remote_rtsp_reply;

// -------------- REMOTE_STDIO (stdio over CAN) ---------------------
#define REMOTE_STDIO_MAX_NODES		16 // numero massimo di nodi (0-15, il nodo 0 è il coordinatore)
#define REMOTE_STDIO_CAN_ID(node_id)	(0x700 + (int)(node_id))
#define REMOTE_STDIO_COORD_ID		0

typedef struct
{
	char data_len;	// lunghezza dei dati (3 bit bassi) e ~CTS (bit alto), oppure -1 se l'endpoint è disattivato, -2 per richiesta di passaggio a rtsp
	char seqnum_ack; // seqnum dell'ultimo pacchetto device2coord ricevuto
	char data[6];
}  __attribute__((packed)) t_can_remote_stdio_coord2endpoint;

typedef struct
{
	char data_len;	// lunghezza dei dati (3 bit bassi), node_id dell'endpoint (4 bit alti)
	char seqnum;
	char data[6];
}  __attribute__((packed)) t_can_remote_stdio_endpoint2coord;

// -------------- STRATEGY_COMMAND -----------------------------------
#define STRATEGY_COMMAND_CAN_ID			0x710
#define STRATEGY_COMMAND_ALIGN_GRANDE1		0x01
#define STRATEGY_COMMAND_ALIGN_GRANDE2		0x02
#define STRATEGY_COMMAND_ALIGN_PICCOLO		0x03
#define STRATEGY_COMMAND_ENABLE_STARTER		0x04
#define STRATEGY_COMMAND_DISABLE_STARTER	0x05
#define STRATEGY_COMMAND_START_PICCOLO		0x06
#define STRATEGY_FLAG_COLOR			0x01 /* 0 = GREEN, 1 = YELLOW */

typedef struct
{
	unsigned char cmd;
	unsigned char flags;
	unsigned int elapsed_time;
	char padding[4];
}  __attribute__((packed)) t_can_strategy_command;

// -------------- ROBOT STATUS UPDATE ---------------------
#define ROBOT_STATUS_UPDATE_CAN_ID        0x402

typedef enum
{
	CAN_ROBOT_STATUS_UPDATE_ROBOT_GRANDE,
	CAN_ROBOT_STATUS_UPDATE_ROBOT_PICCOLO
} t_can_robot_status_update_robot;

typedef enum
{
	CAN_ROBOT_STATUS_UPDATE_STATUS_NO_INFO,
	CAN_ROBOT_STATUS_UPDATE_STATUS_IDLE,
	CAN_ROBOT_STATUS_UPDATE_STATUS_ALIGNING,
	CAN_ROBOT_STATUS_UPDATE_STATUS_WAIT_STARTER_IN,
	CAN_ROBOT_STATUS_UPDATE_STATUS_WAIT_STARTER_OUT,
	CAN_ROBOT_STATUS_UPDATE_STATUS_RUNNING
} t_can_robot_status_update_status;

typedef struct {
	int robot_selected;
	int status_display;
	char padding[4];
}  __attribute__((packed)) t_can_robot_status_update;

// --------------- SIMPLE_VAR_OUTPUT ----------------------
#define SIMPLE_VAR_OUTPUT_ID		0x200 // priorità alta, per non perdere troppo tempo nell'invio

// Ad eccezione del tipo speciale "FLUSH", i seguenti codici corrispondo a quelli
// del modulo python struct
#define SIMPLE_VAR_TYPE_FLUSH		'\0'
#define SIMPLE_VAR_TYPE_NULL		'x'
#define SIMPLE_VAR_TYPE_CHAR		'c'
#define SIMPLE_VAR_TYPE_INT8		'b'
#define SIMPLE_VAR_TYPE_UINT8		'B'
#define SIMPLE_VAR_TYPE_BOOL		'?'
#define SIMPLE_VAR_TYPE_INT16		'h'
#define SIMPLE_VAR_TYPE_UINT16		'H'
#define SIMPLE_VAR_TYPE_INT32		'i'
#define SIMPLE_VAR_TYPE_UINT32		'I'
#define SIMPLE_VAR_TYPE_FLOAT		'f'
#define SIMPLE_VAR_TYPE_STRING		'p'

typedef struct {
	char type, ctr;
	char data[6];
}  __attribute__((packed)) t_can_simple_var_packet;

// --------------- NBA_COORDINATION_COMMAND ----------------------
#define NBA_COORDINATION_COMMAND_ID	0x798 // inviati dal grande al piccolo

typedef struct {
	char _padding[8];
}  __attribute__((packed)) t_can_nba_coordination_command;

// --------------- NBA_COORDINATION_STATUS ----------------------
#define NBA_COORDINATION_STATUS_ID		0x799 // inviati dal piccolo al grande

typedef struct {
	char status;
	char _padding[7];
}  __attribute__((packed)) t_can_nba_coordination_status;

#define NBA_COORDINATION_STATUS_PREPARING	0x00
#define NBA_COORDINATION_STATUS_INPROGRESS	0x01
#define NBA_COORDINATION_STATUS_IDLE		0x02

#endif
