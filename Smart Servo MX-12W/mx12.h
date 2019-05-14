/*
Mar 19 2019
*/

#ifndef _MX12_H_
#define _MX12_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#ifndef WIRING_LIB
#define WIRING_LIB
#endif

//ROM
#define MX_MODEL_NUMBER_L 0
#define MX_MODEL_NUMBER_H 1
#define MX_VERSION 2
#define MX_ID 3
#define MX_BAUD_RATE 4
#define MX_RETURN_DELAY_TIME 5
#define MX_CW_ANGLE_LIMIT_L 6
#define MX_CW_ANGLE_LIMIT_H 7
#define MX_CCW_ANGLE_LIMIT_L 8
#define MX_CCW_ANGLE_LIMIT_H 9
#define MX_LIMIT_TEMPERATURE 11
#define MX_LOWEST_LIMIT_VOLTAGE 12
#define MX_HIGHEST_LIMIT_VOLTAGE 13
#define MX_MAX_TORQUE_L 14
#define MX_MAX_TORQUE_H 15
#define MX_STATUS_RETURN_LEVEL 16
#define MX_ALARM_LED 17
#define MX_ALARM_SHUTDOWN 18
#define MX_MULTI_TURN_OFFSET_L 20
#define MX_MULTI_TURN_OFFSET_H 21
#define MX_RESOLUTION_DIVIDER 22

//RAM
#define MX_TORQUE_ENABLE 24
#define MX_LED 25
#define MX_D_GAIN 26
#define MX_I_GAIN 27
#define MX_P_GAIN 28
#define MX_GOAL_POSITION_L 30
#define MX_GOAL_POSITION_H 31
#define MX_MOVING_SPEED_L 32
#define MX_MOVING_SPEED_H 33
#define MX_TORQUE_LIMIT_L 34
#define MX_TORQUE_LIMIT_H 35
#define MX_PRESENT_POSTITION_L 36
#define MX_PRESENT_POSTITION_H 37
#define MX_PRESENT_SPEED_L 38
#define MX_PRESENT_SPEED_H 39
#define MX_PRESENT_LOAD_L 40
#define MX_PRESENT_LOAD_H 41
#define MX_PRESENT_VOLTAGE 42
#define MX_PRESENT_TEMPERATURE 43
#define MX_REGISTED_INSTRUCTION 44
#define MX_MOVING 46
#define MX_LOCK 47
#define MX_PUNCH_L 48
#define MX_PUNCH_H 49
#define MX_GOAL_ACCELERATION 73

/*
**STATUS RETURN LEVEL
*It decides how to return Status Packet
*0 = No return against all commands (Except PING Command)
*1 = Return only for READ command
*2 = Return for all commands
*/
#define MX_RETURN_NONE 0
#define MX_RETURN_READ 1
#define MX_RETURN_ALL 2

//Instruction Set
#define MX_PING 1
#define MX_READ_DATA 2
#define MX_WRITE_DATA 3
#define MX_REG_WRITE 4
#define MX_ACTION 5
#define MX_FACTORY_RESET 6
#define MX_SYNC_WRITE 131

// LENGTH
#define MX_PING_LENGTH 2
#define MX_RESET_LENGTH 2
#define MX_ACTION_LENGTH 2
#define MX_MODEL_LENGTH 4
#define MX_FW_LENGTH 4
#define MX_ID_LENGTH 4
#define MX_LR_LENGTH 4
#define MX_SRL_LENGTH 4
#define MX_RDT_LENGTH 4
#define MX_LEDALARM_LENGTH 4
#define MX_SHUTDOWNALARM_LENGTH 4
#define MX_RESOLUTION_LENGTH 4
#define MX_TL_LENGTH 4
#define MX_VL_LENGTH 6
#define MX_AL_LENGTH 7
#define MX_P_GAIN_LENGTH 4
#define MX_I_GAIN_LENGTH 4
#define MX_D_GAIN_LENGTH 4
#define MX_BD_LENGTH 4
#define MX_MX_TEM_LENGTH 4
#define MX_MOVING_LENGTH 4
#define MX_RWS_LENGTH 4
#define MX_VOLT_LENGTH 4
#define MX_LOAD_LENGTH 4
#define MX_LED_LENGTH 4
#define MX_TORQUE_LENGTH 4
#define MX_POS_LENGTH 4
#define MX_GOAL_LENGTH 5
#define MX_MT_LENGTH 5
#define MX_PUNCH_LENGTH 5
#define MX_SPEED_LENGTH 4
#define MX_GOAL_SP_LENGTH 7

//SPECIAL
#define MX_BYTE_READ 1
#define MX_INT_READ 2
#define MX_ACTION_CHECKSUM 250
#define MX_BROADCAST_ID 254
#define MX_START 255
#define MX_CCW_AL_L 255
#define MX_CCW_AL_H 3
#define MX_LOCK_VALUE 1
#define LEFT 0
#define RIGHT 1
#define RX_TIMEOUT 10

//RPi constants
#define RPI_DIRECTION_PIN 18   
#define RPI_DIRECTION_TX  0
#define RPI_DIRECTION_RX 1
#define RPI_DIRECTION_SWITCH_DELAY 0.0002


class Mx12
{
	public:
		int fd;

		Mx12();
		
		/*
		Change the direction of UART
		*/
		void direction(bool direction);

		/*
		Read the value from Status Packet
		*/
		int readData(int id);

		/*
		Instruction to check the existence of a Device and basic information
		Regardless of the Status Return Level of the Device, the Status Packet is always sent to Ping Instruction.
		When the Packet ID field is 0xFE(Broadcast ID) : All devices send their Status Packet according to their arranged order.
		*/
		int ping(int id);

		/*
		Instruction that resets the Control Table to its initial factory default settings.
		In case of when Packet ID is a Broadcast ID 0xFE and Option is Reset All 0xFF, Factory Reset Instruction(0x06) will NOT be activated.	
		*/
		int factoryReset(int id,bool confirm);

		/*
		The ID is a unique value in the network to identify each DYNAMIXEL with an Instruction Packet. 
		0~252 (0xFC) values can be used as an ID, and 254(0xFE) is occupied as a broadcast ID. 
		The Broadcast ID(254, 0xFE) can send an Instruction Packet to all connected DYNAMIXELs simultaneously.
		*/
		int setID(int id, int newID);

		/*
		Baud Rate determines serial communication speed between a controller and DYNAMIXELs
		*/
		int setBaudRate(int id, int baudRate);

		/*
		This value decides how to return Status Packet when Dynamixel receives an Instruction Packet.
		0- Ping Instruction: Status Packet will not be returned for all Instructions
		1- Ping Instruction, Read Instruction: Status Packet will be returned only for READ Instruction
		2- All Instruction: Status Packet will be returned for all Instructions
		NOTE : If the ID of Instruction Packet is set to Broad Cast ID(0xFE), Status Packet will not be returned for READ and WRITE Instructions 
		regardless of Status Return Level.
		*/
		int setStatusReturnLevel(int id, int level);

		/*
		After the DYNAMIXEL receives an Instruction Packet, it delays transmitting the Status Packet for Return Delay Time(5). 
		For instance, if the Return Delay Time(5) is set to ‘10’, the Status Packet will be returned after 20[μsec] when the Instruction Packet is received.
		*/
		int setReturnDelayTime(int id, int delay);

		/*
		0: EEPROM area can be modified
		1: EEPROM area cannot be modified
		CAUTION : If Lock is set to 1, the power must be turned off and then turned on again to change into 0.
		*/
		int lockRegister(int id);

		/*
		Moving Servo with default speed
		*/
		int move(int id, long position);
		/*
		Moving Servo with specific speed
		*/
		int moveSpeed(int id, long position, long speed);

		/*
		REG_WRITE : Moving Servo with default speed
		*/
		int moveRW(int id, long position);

		/*
		REG_WRITE : Moving Servo with specific speed
		*/
		int moveSpeedRW(int id, long position, long speed);

		/*
		Instruction that executes the Packet that has been registered using Reg Write Instruction
		When controlling multiple devices using Write Instruction, there will be a difference in the time of execution 
		between the first device that receives the Packet and the last device that receives the Packet.
		*/
		void action();

		
		//int setTorqueStatus(int id, bool status);

		/*
		P gain : value of proportional band. Gains values are in between 0 ~ 254.
		Kp : P Gain / 8
		*/
		int setPGain(int id, int p);

		/*
		I gain:  value of integral action. Gains values are in between 0 ~ 254.
		Ki : I Gain * 1,000 / 2,048
		*/
		int setIGain(int id, int i);

		/*
		D gain:  value of derivative action. Gains values are in between 0 ~ 254.
		Kd : D Gain * 4 / 1,000
		*/
		int setDGain(int id, int d);

		/*
		Return the present position value of Dynamixel. The range of the value is 0~4095 (0xFFF), and the unit is 0.088 [°].
		*/
		int readPosition(int id);

		/*
		Return the present speed value. Is the current moving speed. 0~2047 (0x000 ~ 0x7FF) can be used. 
		If a value is in the rage of 0~1023 then the motor rotates to the CCW direction. 
		If a value is in the rage of 1024~2047 then the motor rotates to the CW direction. 
		The 10th bit becomes the direction bit to control the direction; 0 and 1024 are equal. The value unit is about 0.916rpm.
		*/
		int readSpeed(int id);

		/*
		0: Goal position command execution is completed
		1: Goal position command execution is in progress
		*/
		int readMovingStatus(int id);
		
		/*
		0: 	REG_WRITE instruction is not received
		1: 		REG_WRITE instruction is received
		*/
		int readRWStatus(int id);
	
};
#endif



