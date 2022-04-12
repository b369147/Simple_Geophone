#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

	// NOTES
	// To use this module, first include this header in stm32fxxx_it.c
	// Then place Command_Get(); to USART_IRQHandler
	// Then check if the USART is enabled and COMMAND_USART/COMMAND_HANDLE is correct 
	// 	Finally you should replace the default Func_ function to what ever function you want 
	
	
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
	
	/* Private definition*/
#define COMMAND_HANDLE huart1
	
#define COMMAND_MAX_RECV_LEN 128
	
	typedef enum {
		OVERFLOW = 0, 
		RX_BUSY = 1
	} Return_Message;
	
	
	
	void Command_Init();
	void Command_Get();
	void Command_Message(Return_Message code);
	void Command_Validate();
	void Command_Parser(uint8_t* command, uint8_t* argument_list);
	void Command_Excute(uint8_t* command);
	
	void Func_print(uint8_t* command);
	void Func_div(uint8_t* command);
	void Func_test(uint8_t* command);
	void Func_trig(uint8_t* command);
	
#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */
