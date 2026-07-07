/*
 * BM280.c
 *
 *  Created on: Jul 7, 2026
 *      Author: Navier
 */


#include "main.h"
#include "stdio.h"
#include "PCA9548.h"
#include "BMP280.h"


extern I2C_HandleTypeDef hi2c1;

uint16_t dig_T1;
int32_t dig_T2,dig_T3;

int32_t dig_P1,dig_P2,dig_P3,dig_P4,dig_P5,dig_P6,dig_P7,dig_P8,dig_P9;

uint8_t dataBuffer[24],press[3],temp[3],press_msb,press_lsb_,press_xlsb;
uint32_t temperature,pressure;

/* Variables for Pressure and Temperature */
float tem1;


//short int dig_T2,dig_T3,dig_H2,dig_H4,dig_H5;


//uint8_t dig_H1,dig_H3;



HAL_StatusTypeDef BMP280_Reset(void)
{
	// Reset the device
	uint8_t val = 0xB6;
	 if(HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDRESS, BMP280_RESET, 1, &val, 1, HAL_MAX_DELAY) == HAL_OK)
	 {
		 return HAL_OK;
	 }
	 else
		 return HAL_ERROR;
	 // Check whether the STM32 is busy or not
	 while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));
}

HAL_StatusTypeDef BMP280_Cont_Meas(void)
{
	uint8_t val = (NORMAL_MODE << 0) | (PRESSURE_OVRSMPL_16 << 2) | (TEMPERATURE_OVRSMPL_2 << 5);
	 if(HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDRESS, BMP280_CTRL_MEAS, 1, &val, 1, HAL_MAX_DELAY) == HAL_OK)
	 {
		 return HAL_OK;
	 }
	 else
		 return HAL_ERROR;
	 // Check whether the STM32 is busy or not
	 while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));
}

HAL_StatusTypeDef BMP280_Config(void)
{
	uint8_t val = (BMP280_I2C << 0) | (IIR_FILTER_16 << 2) | (STDBY_TIME_0p5 << 5);
	 if(HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDRESS, BMP280_CONFIG, 1, &val, 1, HAL_MAX_DELAY) == HAL_OK)
	 {
		 return HAL_OK;
	 }
	 else
		 return HAL_ERROR;
	 // Check whether the STM32 is busy or not
	 while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));
}

int32_t t_fine;

/*double bmp280_compensate_T_double(int32_t adc_T)
{
	// Declaring data variables..
	double var1, var2, T;

	var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);

	t_fine = (int32_t)(var1 + var2);
	T = (var1 + var2)/5120.0;
	return T;

}*/

int32_t tem;
int32_t compensate_temp(int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3))
            >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t press1;
uint32_t compensate_pressure(int32_t adc_P) {
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) dig_P6);
    var2 = var2 + ((var1 * ((int32_t) dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t) dig_P4) << 16);
    var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t) dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t) dig_P1)) >> 15);
    if (var1 == 0)
        return 0;

    p = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (p < 0x80000000)
        p = (p << 1) / ((uint32_t) var1);
    else
        p = (p / (uint32_t) var1) * 2;

    var1 = (((int32_t) dig_P9) * ((int32_t) (((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t) (p >> 2)) * ((int32_t) dig_P8)) >> 13;
    p = (uint32_t) ((int32_t) p + ((var1 + var2 + dig_P7) >> 4));

    return p;
}

uint8_t BMP280_DeviceID(void)
{
	uint8_t id;
	PCA9548_SelectChannel(PCA9548A_ADDR,PCA9548_CHANNEL0_ENABLED);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDRESS, BMP280_DEVICE_ID, 1, &id, 1, HAL_MAX_DELAY);
	while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));
	return id;
}

void BMP280_ReadCalibrationRegisters(void)
{
	PCA9548_SelectChannel(PCA9548A_ADDR,PCA9548_CHANNEL0_ENABLED);
	BMP280_Reset();
	BMP280_Cont_Meas();
	BMP280_Config();

	  uint8_t reg_addr = 0x88;
	  HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDRESS, reg_addr, 1, dataBuffer, 24, HAL_MAX_DELAY);
	  while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));

	  dig_T1 = dataBuffer[0] | dataBuffer[1] << 8;
	  dig_T2 = dataBuffer[2] | dataBuffer[3] << 8;
	  dig_T3 = dataBuffer[4] | dataBuffer[5] << 8;

	  dig_P1 = dataBuffer[6] | dataBuffer[7] << 8;
	  dig_P2 = dataBuffer[8] | dataBuffer[9] << 8;
	  dig_P3 = dataBuffer[10] | dataBuffer[11] << 8;
	  dig_P4 = dataBuffer[12] | dataBuffer[13] << 8;
	  dig_P5 = dataBuffer[14] | dataBuffer[15] << 8;
	  dig_P6 = dataBuffer[16] | dataBuffer[17] << 8;
	  dig_P7 = dataBuffer[18] | dataBuffer[19] << 8;
	  dig_P8 = dataBuffer[20] | dataBuffer[21] << 8;
	  dig_P9 = dataBuffer[22] | dataBuffer[23] << 8;

	  // Read raw pressure values
	  reg_addr = 0xF7;
	  HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDRESS, reg_addr, 1, press, 3, HAL_MAX_DELAY);
	  while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));

	  // Read raw temperature values
	  reg_addr = 0xFA;
	  HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDRESS, reg_addr, 1, temp, 3, HAL_MAX_DELAY);

	  while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));

	  temperature = temp[0] << 12 | temp[1] << 4 | temp[2] >> 3;
	  pressure = press[0] << 12 | press[1] << 4 | press[2] >> 3;

	  tem =  compensate_temp(temperature)/100;

	  press1 =  compensate_pressure(pressure)/1000;
}
