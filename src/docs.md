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

	id : id of the Dynamixel	

## factoryReset (int id, bool confirm) ##

Instruction that resets the Control Table to its initial factory default settings.
In case of when Packet ID is a Broadcast ID 0xFE and Option is Reset All 0xFF, Factory Reset Instruction(0x06) will NOT be activated.	

	id : id of the Dynamixel
	
	confirm : confirm reset the Control Table

## setID (int id, int newID) ##

Set new ID for the Dynamixel
	
	id: current id of the Dynamixel

	newID: new id of the Dynamixel

## setBaudRate(int id, int baudRate) ##

Set the baudrate for Serial communication

	id: current id of the Dynamixel

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
	|1|PING Instruction|Status Packet will be returned only for READ Instruction|
	||READ Instruction||
	|2|All Instructions|Status Packet will be returned for all Instructions|
	
		
NOTE : If the ID of Instruction Packet is set to Broad Cast ID(0xFE), Status Packet will not be returned for READ and WRITE Instructions regardless of Status Return Level.
	
	id : current id of the Dynamixel

	Level : level of 