//#include "ADS1115_w.h"
//
////_EVENT ADSReadADC(double *ReData)
////{
////	uint8_t rx_data[2] = { 0 };
////	uint16_t REGADS = 0xc383;
////	int16_t data;
////	// QURESTION: Is I2C_MEMADD_SIZE_8BIT right?
////	// I think it's right to change 8bit to 16bit
////	if (ADS1115_Write(ADS1115_Write_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&REGADS, 2) != EVENT_OK)
////		return EVENT_ERR;
////	HAL_Delay(10);
////	if (ADS1115_Read(ADS1115_Read_Address, CONVERSION_REG_ADDR, I2C_MEMADD_SIZE_8BIT, rx_data, 2) != EVENT_OK)
////		return EVENT_ERR;
////	data = rx_data[0] * 256 + rx_data[1];
////	if ((data == 0x7FFF) | (data == 0x8000))
////	{
////		data = 0;
////		printf("over PGA \r\n");
////	}
////	*ReData = (double) data * 125 / 1000000.0;
////	return EVENT_OK;
////}
//	
//	
////_EVENT ADSRead(double *ReData)
////{
////	uint16_t ADCData;
////	uint16_t HL_CMD = 0x0082;
////	if (ADS1115_Write(ADS1115_Write_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&HL_CMD, 2) != EVENT_OK)
////		return EVENT_ERR;
////	HAL_Delay(50);
////	if (ADS1115_Write(ADS1115_Write_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_8BIT, 0x00, 2) != EVENT_OK) 
////		return EVENT_ERR;
////	if (ADS1115_Read(ADS1115_Read_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&ADCData, 2) != EVENT_OK)
////	{
////		return EVENT_ERR;
////	}
////	*ReData = (double)(ADCData * 4.096 * 2 * 5);
////	return EVENT_OK;
////}
//	
//	
//_EVENT ADSConfig_Default()
//{
//	uint16_t config_set = 0x0583; // default
//	if (ADS1115_Write(ADS1115_Write_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_16BIT, (uint16_t*)&config_set, 2) != EVENT_OK)
//		return EVENT_ERR;
//	return EVENT_OK;
//}
//
//_EVENT ADSConfig(uint16_t config_set)
//{
//	if(ADS1115_Write(ADS1115_Write_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_16BIT, (uint16_t*)&config_set, 2) != EVENT_OK)
//		return EVENT_ERR;
//	return EVENT_OK;
//}
//
//_EVENT ADSReadData(double *ReData)
//{
//	uint16_t ADCData;
//	if (ADS1115_Write(ADS1115_Write_Address, CONVERSION_REG_ADDR, I2C_MEMADD_SIZE_8BIT, 0x00, 1) != EVENT_OK)
//		return EVENT_ERR;
//	if (ADS1115_Read(ADS1115_Read_Address, CONVERSION_REG_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&ADCData, 2) != EVENT_OK)
//	{
//		return EVENT_ERR;
//	}
//	*ReData = (double)ADCData;
//	return EVENT_OK;
//}
//
//_EVENT ADSConfight(void)
//{
//	uint16_t regADS = ADS1115_DIFF_MODE_Address;
//	if (ADS1115_Write(ADS1115_Write_Address, CONFIG_REG_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&regADS, 2) != EVENT_OK)
//		return EVENT_ERR;
//	return EVENT_OK;
//}
//	
//	
//	
//_EVENT ADS1115_Read(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
//{
//	if (HAL_I2C_Mem_Read(&I2C_HANDLE, dev_address, mem_address, mem_addr_size, p_data, size, 10) != HAL_OK)
//		return EVENT_ERR;
//	return EVENT_OK;
//}
//
//_EVENT ADS1115_Write(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
//{
//	if (HAL_I2C_Mem_Write(&I2C_HANDLE, dev_address, mem_address, mem_addr_size, p_data, size, 10) != HAL_OK)
//		return EVENT_ERR;
//	return EVENT_OK;
//}
//	