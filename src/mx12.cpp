#include "mx12.h"

Mx12::Mx12()
{
	if (fd == NULL) 
	{
		if((fd = serialOpen ("/dev/ttyS0", 1000000)) < 0 )
		{
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
		}
	}
	wiringPiSetupGpio();
	pinMode(RPI_DIRECTION_PIN, OUTPUT);
	direction(RPI_DIRECTION_RX);
}

void Mx12::direction(bool direction)
{
    delay(RPI_DIRECTION_SWITCH_DELAY);
    digitalWrite(RPI_DIRECTION_PIN,direction);
}



int Mx12::readData(int id)
{
	direction(RPI_DIRECTION_RX);
	char response[12]={0,0,0,0,0,0,0,0,0,0,0,0,0};
	char c;
	int i,count=0;
	int x =0;
	do
	{
		c = serialGetchar(fd);
		response[x] = c;
	} while(serialDataAvail(fd));
	
	for(i=0;i<12;i++)
	{
		if(response[i] != 0) count++;
	}
	if (count > 4)
	{
		char hd1_byte = response[0];
		char hd2_byte = response[1];
		if(((int)hd1_byte == 0xff)&&((int)hd2_byte == 0xff))
		{
			char len_byte = response[3];
			char err_byte = response[4];
			int length = (int)len_byte - 2;
			int error = (int)err_byte;
			if (error !=0) {
				printf("\n [Error] Servo %d - Code %d",id,error);
				return -error;
			} 
			else if (length == 0) return 0; // no error
			else if (length == 1 && response[5] !=0)
			{
				int returnVal = int(response[5]);
				return returnVal;
			} 
			else if ( length == 2 && response[5] != 0 && response[6] ! = 0)
			{
				long returnVal = ((int)response[6]<<8)+((int)response[5]<<0)  
				return returnVal;
			}
			else {
				printf("\n [Error] Return package incorrect ID#%d",id);
				return -128;
			}
		}
	}
	else 
	{
		printf("\n  [Error] Header incorrect ID#%d",id);
		return -128;
	}
}

int Mx12::ping(int id)
{
    direction(RPI_DIRECION_TX);
    serialFlush(fd);
    int checksum = (~(id + MX_READ_DATA + MX_PING))&0xff;    
    char outData[6];
    outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_PING_LENGTH);
    outData[4] = char(MX_PING);
    outData[5] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
   	return readData(id); 
}

int Mx12::factoryReset(int id,bool confirm)
{
	if(confirm)
	{
		direction(RPI_DIRECION_TX);
		serialFlush(fd);
		int checksum = (~(id + MX_RESET_LENGTH + MX_FACTORY_RESET))&0xff;
		char outData[6];
		outData[0] = char(MX_START);
    	outData[1] = char(MX_START);
    	outData[2] = char(id);
    	outData[3] = char(MX_RESET_LENGTH);
    	outData[4] = char(MX_FACTORY_RESET);
    	outData[5] = char(checksum);
    	serialPuts(fd,outData);
    	serialFlush(fd);
    	return readData(id);
	}
	else 
	{
		printf("please sen confirm = TRUE as this function reset to the factory default value, i.e reset the motor ID");
		return 0;
	}
}

int Mx12::setID(int id, int newID)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_ID_LENGTH + MX_WRITE_DATA + MX_ID + newID))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_ID_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_ID);
    outData[6] = char(newID);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::setBaudRate(int id, int baudRate)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int br = ((2000000/long(baudRate))-1)
	int checksum = (~(id + MX_BD_LENGTH + MX_WRITE_DATA + MX_BAUD_RATE + br))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_BD_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_BAUD_RATE);
    outData[6] = char(br);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}


int Mx12::setStatusReturnLevel(int id, int level)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	char outData[8];
	int checksum = (~(id + MX_SRL_LENGTH + MX_WRITE_DATA + MX_STATUS_RETURN_LEVEL level))&0xff;
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_SRL_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_STATUS_RETURN_LEVEL);
    outData[6] = char(level);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}


int Mx12::setReturnDelayTime(int id, int delay)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_RDT_LENGTH + MX_WRITE_DATA + MX_RETURN_DELAY_TIME +(delay/2)&0xff))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_RDT_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
	outData[5] = char(MX_RETURN_DELAY_TIME);
	outData[6] = char((delay/2)&0xff);
	outData[7] = char (checksum);
	serialPuts(fd,outData);
	serialFlush(fd);
	return readData(id);
}

int Mx12::lockRegister(int id)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_LR_LENGTH + MX_WRITE_DATA + MX_LOCK + MX_LOCK_VALUE))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_LR_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
	outData[5] = char(MX_LOCK);
	outData[6] = char(MX_LOCK_VALUE);
	outData[7] = char(checksum);
	serialPuts(fd,outData);
	serialFlush(fd);
	return readData(id);
}

int Mx12::move(int id, long position)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int p[2] = {position & 0xff,position >> 8}
	int checksum = (~(id + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_GOAL_POSITION_L + p[0] + p[1]))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_GOAL_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
	outData[5] = char(MX_GOAL_POSITION_L);
	outData[6] = char(p[0]);
	outData[7] = char(p[1]);
	outData[8] = char (checksum);
	serialPuts(fd,outData);
	serialFlush(fd);
	return readData(id);
}

int Mx12::moveSpeed(int id, long position, long speed)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int p[2] = {position & 0xff,position >> 8}
	int s[2] = {speed & 0xff,speed >> 8}
	int checksum = (~(id + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_GOAL_POSITION_L + p[0] + p[1] + s[0] + s[1]))&0xff;
	char outData[11];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_GOAL_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
	outData[5] = char(MX_GOAL_POSITION_L);
	outData[6] = char(p[0]);
	outData[7] = char(p[1]);
	outData[8] = char(s[0]);
	outData[9] = char(s[1]);
	outData[10] = char (checksum);
	serialPuts(fd,outData);
	serialFlush(fd);
	return readData(id);
}

int Mx12::moveRW(int id, long position)
{
	irection(RPI_DIRECION_TX);
	serialFlush(fd);
	int p[2] = {position & 0xff,position >> 8}
	int checksum = (~(id + MX_GOAL_LENGTH + MX_REG_WRITE + MX_GOAL_POSITION_L + p[0] + p[1]))&0xff;
	char outData[9];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_GOAL_LENGTH);
    outData[4] = char(MX_REG_WRITE);
	outData[5] = char(MX_GOAL_POSITION_L);
	outData[6] = char(p[0]);
	outData[7] = char(p[1]);
	outData[8] = char (checksum);
	serialPuts(fd,outData);
	serialFlush(fd);
	return readData(id);
}

int Mx12::moveSpeedRW(int id, long position, long speed)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int p[2] = {position & 0xff,position >> 8}
	int s[2] = {speed & 0xff,speed >> 8}
	int checksum = (~(id + MX_GOAL_LENGTH + MX_REG_WRITE + MX_GOAL_POSITION_L + p[0] + p[1] + s[0] + s[1]))&0xff;
	char outData[11];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_GOAL_LENGTH);
    outData[4] = char(MX_REG_WRITE);
	outData[5] = char(MX_GOAL_POSITION_L);
	outData[6] = char(p[0]);
	outData[7] = char(p[1]);
	outData[8] = char(s[0]);
	outData[9] = char(s[1]);
	outData[10] = char (checksum);
	serialPuts(fd,outData);
	serialFlush(fd);
	return readData(id);
}

void Mx12::action()
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	char outData[6];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(MX_BROADCAST_ID);
    outData[3] = char(MX_ACTION_LENGTH);
    outData[4] = char(MX_ACTION);
    outData[5] = char(MX_ACTION_CHECKSUM);
    serialPuts(fd,outData);
    serialFlush(fd);
}

/*int Mx12::setTorqueStatus(int id, bool status)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int ts;
	if((status == TRUE) or (status == 1))
	{
		ts = 1;
	}
	else ts = 0;
	int checksum = (~(id + MX_TORQUE_LENGTH + MX_WRITE_DATA + MX_TORQUE_ENABLE + ts))&0xff;
	char outData[20];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_TORQUE_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_TORQUE_ENABLE);
    outData[6] = char(ts);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
	}*/

int Mx12::setPGain(int id, int p)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_P_GAIN_LENGTH + MX_WRITE_DATA + MX_P_GAIN + p))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_P_GAIN_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_P_GAIN);
    outData[6] = char(p);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::setIGain(int id, int i)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_I_GAIN_LENGTH + MX_WRITE_DATA + MX_I_GAIN + i))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_I_GAIN_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_I_GAIN);
    outData[6] = char(i);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::setPGain(int id, int d)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_D_GAIN_LENGTH + MX_WRITE_DATA + MX_D_GAIN + d))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_D_GAIN_LENGTH);
    outData[4] = char(MX_WRITE_DATA);
    outData[5] = char(MX_D_GAIN);
    outData[6] = char(d);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::readPosition(int id)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_POS_LENGTH + MX_READ_DATA + MX_PRESENT_POSTITION_L + MX_INT_READ))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_POS_LENGTH);
    outData[4] = char(MX_READ_DATA);
    outData[5] = char(MX_PRESENT_POSTITION_L);
    outData[6] = char(MX_INT_READ);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::readSpeed(int id)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_SPEED_LENGTH + MX_READ_DATA + MX_PRESENT_SPEED_L + MX_INT_READ))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_SPEED_LENGTH);
    outData[4] = char(MX_READ_DATA);
    outData[5] = char(MX_PRESENT_SPEED_L);
    outData[6] = char(MX_INT_READ);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::readMovingStatus(int id)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_MOVING_LENGTH + MX_READ_DATA + MX_MOVING + MX_BYTE_READ))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_MOVING_LENGTH);
    outData[4] = char(MX_READ_DATA);
    outData[5] = char(MX_MOVING);
    outData[6] = char(MX_BYTE_READ);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}

int Mx12::readRWStatus(int id)
{
	direction(RPI_DIRECION_TX);
	serialFlush(fd);
	int checksum = (~(id + MX_RWS_LENGTH + MX_READ_DATA + MX_REGISTED_INSTRUCTION + MX_BYTE_READ))&0xff;
	char outData[8];
	outData[0] = char(MX_START);
    outData[1] = char(MX_START);
    outData[2] = char(id);
    outData[3] = char(MX_RWS_LENGTH);
    outData[4] = char(MX_READ_DATA);
    outData[5] = char(MX_REGISTED_INSTRUCTION);
    outData[6] = char(MX_BYTE_READ);
    outData[7] = char(checksum);
    serialPuts(fd,outData);
    serialFlush(fd);
    return readData(id);
}