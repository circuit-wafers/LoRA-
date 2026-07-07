/*
 * SX1262.h
 *
 *  Created on: Jul 7, 2026
 *      Author: moswa
 */

#ifndef INC_SX1262_H_
#define INC_SX1262_H_

#define				AT				"AT\r\n"
#define				AT_VERSION		"AT+VER\r\n"


// Possible states of the DTDS-6022 Chip
typedef enum
{
	SX1262_OK = 0x01,
	SX1262_BUSY = 0x02,
	SX1262_ERROR = 0x03,
	SX1262_IDLE = 0x04,
	SX1262_TX = 0x05,
	SX1262_RX = 0x06,
	SX1262_CX = 0x07
}SX1262_STATUS;

SX1262_STATUS SX1262_Test(void);
SX1262_STATUS SX1262_Status(void);
SX1262_STATUS SX1262_Reset(void);

SX1262_STATUS SX1262_Get_Modem(void);
SX1262_STATUS SX1262_Set_Modem(void);

SX1262_STATUS SX1262_Get_Freq(void);
SX1262_STATUS SX1262_Set_Freq(void);

SX1262_STATUS SX1262_Get_LM_Config(void);
SX1262_STATUS SX1262_Set_LM_Config(void);

SX1262_STATUS SX1262_Get_TX_Power(void);
SX1262_STATUS SX1262_Set_TX_Power(void);

SX1262_STATUS SX1262_Get_LPCFG(void);
SX1262_STATUS SX1262_Set_LPCFG(void);


SX1262_STATUS SX1262_PKT(void);
SX1262_STATUS SX1262_Send(void);

SX1262_STATUS SX1262_Receive(void);

SX1262_STATUS SX1262_Stop(void);

void SX1262_Init(void);

#endif /* INC_SX1262_H_ */
