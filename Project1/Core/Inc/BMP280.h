/*
 * BMP280.h
 *
 *  Created on: Jul 7, 2026
 *      Author: moswa
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

/* Macros Definition for BMP280 */
#define				BMP280_ADDRESS						0xEC
#define				BMP280_DEVICE_ID					0xD0
#define				BMP280_RESET						0xE0
#define				BMP280_CTRL_MEAS					0xF4
#define				BMP280_CONFIG						0xF5
#define				BMP280_SPI							0x1
#define				BMP280_I2C							0x0

/* Pressure Oversampling */
#define				PRESSURE_OVRSMPL_0				    0x0
#define				PRESSURE_OVRSMPL_1				    0x1
#define				PRESSURE_OVRSMPL_2				    0x2
#define				PRESSURE_OVRSMPL_4				    0x3
#define				PRESSURE_OVRSMPL_8				    0x4
#define				PRESSURE_OVRSMPL_16				    0x5

/* Temperature Oversampling */
#define				TEMPERATURE_OVRSMPL_0			    0x0
#define				TEMPERATURE_OVRSMPL_1			    0x1
#define				TEMPERATURE_OVRSMPL_2			    0x2
#define				TEMPERATURE_OVRSMPL_4			    0x3
#define				TEMPERATURE_OVRSMPL_8			    0x4
#define				TEMPERATURE_OVRSMPL_16			    0x5

/* Different Modes of BMP280 */
#define				SLLEEP_MODE			                0x0
#define				FORCED_MODE			                0x2
#define				NORMAL_MODE			                0x3

/* Standby Time Settings */
#define				STDBY_TIME_0p5			            0x0
#define				STDBY_TIME_62p5			            0x1
#define				STDBY_TIME_125			            0x2
#define				STDBY_TIME_250			            0x3
#define				STDBY_TIME_500			            0x4
#define				STDBY_TIME_1000			            0x5
#define				STDBY_TIME_10			            0x6
#define				STDBY_TIME_20			            0x7

/* IIR Filter Settings */
#define				IIR_FILTER_OFF			            0x0
#define				IIR_FILTER_2			            0x1
#define				IIR_FILTER_4			            0x2
#define				IIR_FILTER_8			            0x3
#define				IIR_FILTER_16			            0x4

HAL_StatusTypeDef BMP280_Reset(void);
HAL_StatusTypeDef BMP280_Cont_Meas(void);
HAL_StatusTypeDef BMP280_Config(void);
uint8_t BMP280_DeviceID(void);

void BMP280_ReadCalibrationRegisters(void);

#endif /* INC_BMP280_H_ */
