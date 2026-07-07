/*
 * PCA9548.c
 *
 *  Created on: Jul 5, 2026
 *      Author: Navier
 */

/* PCA9548        --->     STM32
 *     VIN		  --->		3.3V
 *     GND		  --->		 GND
 *     SDA		  --->		 PB7 (PULL UP TO 3.3V USING 4.7K Resistor)
 *     SCL		  --->		 PB6 (PULL UP TO 3.3V USING 4.7K Resistor)
 *     RST		  --->  	3.3V (OR PULL UP VIA 10K Ohm resistor)
 *     	A2		  --->		   0
 *     	A1		  --->		   0
 *     	A0		  --->		   0
 *
 *     	Using NUCLEO-L031K6
 *     	I2C Mode : Standard
 *     	I2C Frequency : 100KHz
 *     	Pin Used (B6,PB7 with no internal pull up)
 * */

#include "main.h"
#include "PCA9548.h"

// I2C Handle Instance for PCA9548 I2C Multiplexing Switch
extern I2C_HandleTypeDef hi2c1;
#define				PCA9548_I2C					hi2c1



/* Check if the device is ready for communication or not */
HAL_StatusTypeDef PCA9548_SelectChannel(uint8_t devAddress,uint8_t channel)
{
	  //channel = PCA9548A_CHANNEL;
	  if(HAL_I2C_IsDeviceReady(&PCA9548_I2C, PCA9548A_ADDR, 2, 100) == HAL_OK)
	  {
		  HAL_I2C_Master_Transmit(&PCA9548_I2C, PCA9548A_ADDR, &channel, 1, HAL_MAX_DELAY);
		  while(!(HAL_I2C_STATE_READY == HAL_I2C_GetState(&hi2c1)));
		  return HAL_OK;
	  }
	  else
		  return HAL_ERROR;
}
