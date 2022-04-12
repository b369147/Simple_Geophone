#ifndef __ADS1115_H__
#define __ADS1115_H__

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
#include "i2c.h"
#include "gpio.h"
#include "command.h"
#include <stdint.h>
	
	/* Private definition*/
#define AdsEnable() HAL_GPIO_WritePin()
#define AdsDisable() HAL_GPIO_WritePin()
	
#define I2C_HANDLE hi2c3
#define ADS1115_Read_Address 0x91
#define ADS1115_Write_Address 0x90
#define Voltage_Reference_4V 4.096
#define Voltage_Reference_2V 2.048
#define ADS1115_DIFF_MODE_Address 0x0583   
	
	typedef enum
	{
		EVENT_OK,
		EVENT_ERR
	}_EVENT;
	
	typedef enum
	{
		CONVERSION_REG_ADDR = 0x00,
		CONFIG_REG_ADDR     = 0X01,
		LOW_THRESOND_REG    = 0X02,
		HIGH_THRESOND_REG   = 0X03
	}ADS1115_REG;
	
	typedef enum 
	{
		RATE_8   = 0,
		RATE_16  = 1,
		RATE_32  = 2,
		RATE_64  = 3,
		RATE_128 = 4,
		RATE_250 = 5,
		RATE_475 = 6,
		RATE_860 = 7
	}DataRate;
	
	typedef enum 
	{
		CC_MODE = 0,
		SS_MODE = 1
	}OPREATION_MODE;
	
	typedef enum 
	{
		FSR_d256 = 5, 
		 // d is dot
		FSR_d512 = 4,
		FSR_1d024 = 3,
		FSR_2d048 = 2,
		FSR_4d096 = 1,
		FSR_6d144 = 0
	}PGA;
	
	typedef enum 
	{
		DIFF01  = 0,
		DIFF03  = 1,
		DIFF13  = 2,
		DIFF23  = 3,
		SINGAL0 = 4,
		SINGAL1 = 5,
		SINGAL2 = 6,
		SINGAL3 = 7
	}MUX;
	
	
	_EVENT ADSReadADC(double *ReData);
	_EVENT ADSRead(double *ReData);
	_EVENT ADSConfight(void);
	_EVENT ADS1115_Read(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size);
	_EVENT ADS1115_Write(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size);
	_EVENT ADSConfig(uint16_t config_set);
	_EVENT ADSConfig_Default();
	_EVENT ADSReadADC(double *ReData);
	
		
	
#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */
