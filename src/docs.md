## Mx12() ##

- Setup baudrate = 1000000 - UART

- wiringPiSetupGpio();	// *Broadcom GPIO*

- Setup GPIO


## readData(int id) ##

Read the value from Status Packet of the Dynamixel

	id : id of the Dynamixel

## ping(int id) ##

Instruction to check the existence of a Device and basic information
Regardless of the Status Return Level of the Device, the Status Packet is always sent to Ping Instruction.
When the Packet ID field is 0xFE(Broadcast ID) : All devices send their Status Packet according to their arranged order.

## factoryReset (int id, bool confirm) ##

Instruction that resets the Control Table to its initial factory default settings.
In case of when Packet ID is a Broadcast ID 0xFE and Option is Reset All 0xFF, Factory Reset Instruction(0x06) will NOT be activated.	
	
	confirm : confirm reset the Control Table

## setID (int id, int newID) ##

Set new ID for the Dynamixel

	newID: new id of the Dynamixel

## setBaudRate(int id, int baudRate) ##

Set the baudrate for Serial communication

	baudrate: This speed is calculated by using the below formula

	Speed(BPS)  = 2000000/(Data+1)

| Data | Set BPS | Target BPS | Tolerance |
|------|---------|------------|-----------|
|1|1000000.0|1000000.0|0.000%|
|3|500000.0|500000.0|0.000%|
|4|400000.0|400000.0|0.000%|
|7|250000.0|250000.0|0.000%|
|9|200000.0|200000.0|0.000%|
|16|117647.1|115200.0|-2.124%|
|34|57142.9|57600.0|0.794%|
|103|19230.8|19200.0|-0.160%|
|207|9615.4|9600.0|-0.160%|

## setStatusReturnLevel (int id, int Level) ##

This value decides how to return Status Packet when Dynamixel receives an Instruction Packet.

|Value|Responding Instructions|Description|
|-----|-----------------------|-----------|
|0|PING Instruction|Status Packet will not be returned for all Instructions|
|1|PING Instruction, READ Instruction|Status Packet will be returned only for READ Instruction|
|2|All Instructions|Status Packet will be returned for all Instructions|
	
		
NOTE : If the ID of Instruction Packet is set to Broad Cast ID(0xFE), Status Packet will not be returned for READ and WRITE Instructions regardless of Status Return Level.

	Level : level of Status Packet 

## setReturnDelayTime(int id, int delay) ##

After the DYNAMIXEL receives an Instruction Packet, it delays transmitting the Status Packet for Return Delay Time(5). 
For instance, if the Return Delay Time(5) is set to ‘10’, the Status Packet will be returned after 20[μsec] when the Instruction Packet is received.

	delay: delay time

## lockRegister(int id) ##

EEPROM area cannot be modified

	CAUTION : If Lock is set to 1, the power must be turned off and then turned on again to change into 0.

## move(int id, long position) ##

Moving Servo with default speed to new Position

	position : value of new position

## moveSpeed(int id, long position, long speed) ##

Moving Servo with specific speed

	speed : value of the speed

## moveRW(int id, long position) ##

REG_WRITE : Moving Servo with default speed

## moveSpeedRW(int id, long position, long speed) ##

REG_WRITE : Moving Servo with specific speed

## action() ##

Instruction that executes the Packet that has been registered using Reg Write Instruction
When controlling multiple devices using Write Instruction, there will be a difference in the time of execution 
between the first device that receives the Packet and the last device that receives the Packet.

## setPGain(int id, int p) ##

 Set P gain.
 Kp : P Gain / 8

	p: value of proportional band. Gains values are in between 0 ~ 254.

## setIGain(int id, int i) ##

Set I gain.
Ki : I Gain * 1,000 / 2,048

	i :  value of integral action. Gains values are in between 0 ~ 254.

## setDGain(int id, int d) ##

Set D gain.
Kd : D Gain * 4 / 1,000

	d :  value of derivative action. Gains values are in between 0 ~ 254.


## readPosition(int id) ##

Return the present position value of Dynamixel. The range of the value is 0~4095 (0xFFF), and the unit is 0.088 [°].

## readSpeed(int id) ##

Return the present speed value. Is the current moving speed. 0~2047 (0x000 ~ 0x7FF) can be used. 
If a value is in the rage of 0~1023 then the motor rotates to the CCW direction. 
If a value is in the rage of 1024~2047 then the motor rotates to the CW direction. 
The 10th bit becomes the direction bit to control the direction; 0 and 1024 are equal. The value unit is about 0.916rpm.


## readMovingStatus(int id) ##

Return value :

	0: Goal position command execution is completed
	
	1: Goal position command execution is in progress

## readRWStatus(int id) ##

Return value :

	0: 	REG_WRITE instruction is not received

	1: 		REG_WRITE instruction is received