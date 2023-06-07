//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
void setCommand(unsigned char *arrCmd,unsigned char *templateCmd, float setNum, bool type)
{
	uint8_t	i=0;
	unsigned char cmd[32];
	unsigned char* cmd_ptr = (unsigned char*)&cmd[0];
	unsigned char* header_setCode_ptr = (unsigned char*)&header_setCode[0];
	unsigned char hexNum[4];
	unsigned char hexArray[8] = {0,0,0,0,0,0,0,0};
	uint16_t pChecksum=0;

	while (i < 13)
	{
		i++;
		*cmd_ptr++ = *header_setCode_ptr++;
	}

	while (i < 19)
	{
		i++;
		*cmd_ptr++ = *templateCmd++;
	}

	if (type == 0)
	{
		memcpy(&hexNum,&setNum,4);
		for (i = 0; i < 4; i++)
		{
			hex2char(&hexArray[i*2],hexNum[i],2);
		}
		for (i = 0; i < 8; i++)
		{
			cmd[19+i] = hexArray[i];
		}
	} else if (type == 1)
	{
		hex2char(&hexArray[0],(uint16_t)setNum,2);
		for (i = 0; i < 8; i++)
		{
			cmd[19+i] = hexArray[i];
		}
	}

	pChecksum = Checksum16(&cmd[0],32);
	hex2char(&cmd[27],pChecksum,4);

	cmd[31] = 0x0d;
	cmd_ptr = &cmd[0];
	i = 0;
	while (i < 32)
	{
		i++;
		*arrCmd++ = *cmd_ptr++;
	}
}
void getCommand(unsigned char *arrCmd,unsigned char *templateCmd)
{
	uint8_t	i=0;
	unsigned char cmd[24];
	unsigned char* cmd_ptr = (unsigned char*)&cmd[0];
	unsigned char* header_getCode_ptr = (unsigned char*)&header_getCode[0];
//	unsigned char getCode[6];
	uint16_t pChecksum=0;

	while (i < 13)
	{
		i++;
		*cmd_ptr++ = *header_getCode_ptr++;
	}

	while (i < 19)
	{
		i++;
		*cmd_ptr++ = *templateCmd++;
	}

	pChecksum = Checksum16(&cmd[0],24);
	hex2char(&cmd[19],pChecksum,4);

	cmd[23] = 0x0d;
	cmd_ptr = &cmd[0];
	i = 0;
	while (i < 24)
	{
		i++;
		*arrCmd++ = *cmd_ptr++;
	}
}
void RectID_Calculation(char* dst, unsigned char* src)
{
	uint8_t i;
	int8_t rectID1[4];
	char buffer[32];
	char* buffer_ptr = (char*)&buffer[0];
	uint16_t rectID2;

	for (i = 0; i < 4; i++)
	{
		rectID1[i] = char2hex(src[i]);
	}

	for (i = 0; i < 4; i++)
	{
		if (rectID1[i] <= 0x09) rectID1[i] += 0x30;
		else if ((rectID1[i] >= 0x0a) && (rectID1[i] <= 0x0f)) rectID1[i] += 0x37;
	}

	rectID2 = hex2byte2(src,4);

	if (rectID2 < 10)
	{
		snprintf(buffer_ptr,18,"RectID:2%c%c%c%c%d%d%d%d%d",rectID1[0],rectID1[1],rectID1[2],rectID1[3],0,0,0,0,0);
		snprintf(&buffer[17],2,"%d",rectID2);
		buffer[18] = '\0';

	} else if ((rectID2 > 9) && (rectID2 < 100))
	{
		snprintf(buffer_ptr,18,"RectID:2%c%c%c%c%d%d%d%d%d",rectID1[0],rectID1[1],rectID1[2],rectID1[3],0,0,0,0,0);
		snprintf(&buffer[16],3,"%d",rectID2);
		buffer[18] = '\0';
	} else if ((rectID2 > 99) && (rectID2 < 1000))
	{
		snprintf(buffer_ptr,18,"RectID:2%c%c%c%c%d%d%d%d%d",rectID1[0],rectID1[1],rectID1[2],rectID1[3],0,0,0,0,0);
		snprintf(&buffer[15],4,"%d",rectID2);
		buffer[18] = '\0';
	} else if ((rectID2 > 999) && (rectID2 < 10000))
	{
		snprintf(buffer_ptr,18,"RectID:2%c%c%c%c%d%d%d%d%d",rectID1[0],rectID1[1],rectID1[2],rectID1[3],0,0,0,0,0);
		snprintf(&buffer[14],5,"%d",rectID2);
		buffer[18] = '\0';
	} else if ((rectID2 > 9999) /*&& (rectID2 < 100000)*/)
	{
		snprintf(buffer_ptr,18,"RectID:2%c%c%c%c%d%d%d%d%d",rectID1[0],rectID1[1],rectID1[2],rectID1[3],0,0,0,0,0);
		snprintf(&buffer[13],6,"%d",rectID2);
		buffer[18] = '\0';
	}


	buffer_ptr = &buffer[0];
	i = 0;
	while (i < 19)
	{
		i++;
		*dst++ = *buffer_ptr++;
	}
}