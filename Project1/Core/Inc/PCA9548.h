/*
 * PCA9548.h
 *
 *  Created on: Jul 5, 2026
 *      Author: Navier
 */

#ifndef INC_PCA9548_H_
#define INC_PCA9548_H_



/* Device Addressing for the PCA9548 */
/* Bit 7 : 1
 * Bit 6 : 1
 * Bit 5 : 1
 * Bit 4 : 1
 * Bit 3 : 0 (A2)
 * Bit 2 : 0 (A1)
 * Bit 1 : 0 (A0)
 * Bit 0 : RW
 * if A2		A1		A0
 * 	   0         0		 1
 * 	   Device Address : (0x71 << 1)
 * */
#define				PCA9548A_ADDR						(0x70 << 1)

// Private macros for PCA9548
#define				PCA9548_CHANNEL0_ENABLED			0x01
#define				PCA9548_CHANNEL0_DISABLED			0x00

#define				PCA9548_CHANNEL1_ENABLED			0x02
#define				PCA9548_CHANNEL1_DISABLED			0x00

#define				PCA9548_CHANNEL2_ENABLED			0x04
#define				PCA9548_CHANNEL2_DISABLED			0x00

#define				PCA9548_CHANNEL3_ENABLED			0x08
#define				PCA9548_CHANNEL3_DISABLED			0x00

#define				PCA9548_CHANNEL4_ENABLED			0x10
#define				PCA9548_CHANNEL4_DISABLED			0x00

#define				PCA9548_CHANNEL5_ENABLED			0x20
#define				PCA9548_CHANNEL5_DISABLED			0x00

#define				PCA9548_CHANNEL6_ENABLED			0x40
#define				PCA9548_CHANNEL6_DISABLED			0x00

#define				PCA9548_CHANNEL7_ENABLED			0x80
#define				PCA9548_CHANNEL7_DISABLED			0x00

// Select channel on the PCA9548 I2C IC
HAL_StatusTypeDef PCA9548_SelectChannel(uint8_t devAddress,uint8_t channel);

#endif /* INC_PCA9548_H_ */
