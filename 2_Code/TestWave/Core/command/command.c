#include "command.h"
#include "usart.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

uint8_t rx_char;

// timer=10ms
// by judging the interval of two data, you can determine whether the data is continious 
// if the time of two data exceed the interval, then the data is discontinious, 
// it also means we have received one data
// STA is used to tag the data status
// [15]: 0 means there's no data received, 1 is the opposite
// [14:0]: the length of data received 
uint16_t status = 0;

uint8_t Command_Index = 0;
uint8_t Command_List[10][128];
static uint8_t Command_RxBuffer[128];



void Command_Init()
{
	__HAL_UART_ENABLE_IT(&COMMAND_HANDLE, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&COMMAND_HANDLE, UART_IT_IDLE);
	__HAL_UART_CLEAR_FLAG(&COMMAND_HANDLE, UART_FLAG_RXNE);
	__HAL_UART_CLEAR_IDLEFLAG(&COMMAND_HANDLE);
	printf("Command Init succeed\r\n");
}

void Command_Get()
{
	static uint8_t* pBuff = Command_RxBuffer;
	
	// if command is too long 
	if(status & 0x8000)
	{
		Command_Message(RX_BUSY);
		__HAL_UART_CLEAR_FLAG(&COMMAND_HANDLE, UART_FLAG_RXNE);    
		__HAL_UART_CLEAR_IDLEFLAG(&COMMAND_HANDLE); 
		return;
	}
	else if(status >= COMMAND_MAX_RECV_LEN)
	{
		status = 0;
		Command_Message(OVERFLOW);
		__HAL_UART_CLEAR_FLAG(&COMMAND_HANDLE, UART_FLAG_RXNE);    
		__HAL_UART_CLEAR_IDLEFLAG(&COMMAND_HANDLE);
		return;
	}
	
	// normal read byte
	if(__HAL_UART_GET_FLAG(&COMMAND_HANDLE, UART_FLAG_RXNE) != RESET)
	{		
		uint8_t ch = (uint16_t) READ_REG(COMMAND_HANDLE.Instance->DR);
		*(pBuff++) = ch;
		status++;    
		__HAL_UART_CLEAR_FLAG(&COMMAND_HANDLE, UART_FLAG_RXNE);    
	}	 
	
	// command fully read
	if(__HAL_UART_GET_FLAG(&COMMAND_HANDLE, UART_FLAG_IDLE) != RESET)
	{	      
		status |= 1 << 15;
		Command_Validate();
		pBuff = Command_RxBuffer;
		__HAL_UART_CLEAR_IDLEFLAG(&COMMAND_HANDLE);
	}
}

void Command_Validate()
{
	memcpy(Command_List[0], Command_RxBuffer, sizeof(Command_RxBuffer));
	Command_Index = 1;
	status = 0;
}

void Command_Parser(uint8_t* command, uint8_t* argument_list)
{
	uint8_t string_len = 0;
	uint8_t string_index = 0;
	uint8_t command_count = 0;
	
	for (uint8_t i = 0; i < 128; i++)
	{
		if ((command[i] == '\r') || (command[i] == '\n'))
		{
			memcpy(argument_list + command_count * 10, &command[string_index], sizeof(command[i]) * string_len);
			*(argument_list + command_count * 10 + string_len) = '\0';
			break;
		}
		else if (command[i] == ' ')
		{
			memcpy(argument_list + command_count * 10, &command[string_index], sizeof(command[i]) * string_len);
			*(argument_list + command_count * 10 + string_len) = '\0';
			command_count++;
			string_index = i + 1;
			string_len = 0;
		}
		else
		{
			string_len++;
		}
	}
	*(argument_list + 10 * 10) = command_count;
	asm("nop");
}

void Command_Excute(uint8_t* command)
{
	// the command_list is used to storage the command and argument
	// the first one command_list[0] is the command
	// the follows are arguments
	// the last one command_list[10][0] storage the argument count
	uint8_t command_list[11][10];
	
	Command_Parser(command, command_list);
	if (strcmp(command_list[0], "config") == 0)
	{
		Func_div(command_list);
	}
	else if (strcmp(command_list[0], "read") == 0)
	{
		Func_trig(command_list);		
	}
	else if (strcmp(command_list[0], "test") == 0) 
	{
		Func_test(command_list);
	}
	else
	{
		printf("Command Not Found!\r\n");
	}
	
	// printf("command:  %s\nargument1: %s\nargument2: %s\nargument3: %s\n", command_list[0], command_list[1], command_list[2], command_list[3]);
}


// This function should not be changed
__weak void Func_div(uint8_t* command)
{
	printf("Please complete the function of calib\r\n");
}

__weak void Func_trig(uint8_t* command)
{
	printf("Please complete the function of trig\r\n");
}

__weak void Func_test(uint8_t* command)
{
	printf("Please complete the function of test\r\n");
}


void Command_Message(Return_Message code)
{
	switch (code)
	{
	case OVERFLOW:
		printf("Command Length exceeds 128\r\n");
		break;
	case RX_BUSY:
		printf("MCU is busy processing the last command\r\n");
		break;
	default:
		break;
	} 
	
}

//int fputc(int ch, FILE *f)
//{
//	/* map transmit to printf */
//	HAL_UART_Transmit(&COMMAND_HANDLE, (uint8_t *)&ch, 1, 1000);	
//	
//	return (ch);

#if defined(__GNUC__)
int _write(int fd, char * ptr, int len)
{
	HAL_UART_Transmit(&COMMAND_HANDLE, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}
#elif defined (__ICCARM__)
#include "LowLevelIOInterface.h"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
	HAL_UART_Transmit(&COMMAND_HANDLE, (uint8_t *) buffer, size, HAL_MAX_DELAY);
	return size;
}
#elif defined (__CC_ARM)
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&COMMAND_HANDLE, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
#endif
