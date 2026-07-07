/*
 * SX1262.c
 *
 *  Created on: Jul 7, 2026
 *      Author: Navier
 */

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "SX1262.h"

extern UART_HandleTypeDef huart2;

// Buffers for Lora receivers
char rxBuffer[32],statusBuffer[32];


SX1262_STATUS SX1262_Test(void)
{
	memset(statusBuffer,0x00,32);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	char tx_param[] = "AT\r\n";

	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	// Check whether the command has been received successfully using idle line..
	while(!(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)));

	// Read buffer for the error
	if(strstr(rxBuffer,"OK") != NULL)
	{
		return SX1262_OK;
	}
	else
		return SX1262_ERROR;

	// First clear the Transmission Complete and IDLE flag..
	//__HAL_UART_CLEAR_IT(&huart2,UART_CLEAR_TCF);
	//__HAL_UART_CLEAR_IDLEFLAG(&huart2);
}

/* Check status of the SX1262 Chip */
SX1262_STATUS SX1262_Status(void)
{
	memset(statusBuffer,0x00,32);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)statusBuffer, 32);

	char tx_param[] = "AT+STAT\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	// Check whether the command has been received successfully using idle line..
	//while(!(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)));
	while(!(HAL_UART_STATE_READY == HAL_UART_GetState(&huart2)));

	if(strstr(statusBuffer,"IDLE") != NULL)
	{
		return SX1262_IDLE;
	}
	else if(strstr(statusBuffer,"TX") != NULL)
	{
		return SX1262_TX;
	}
	else if(strstr(statusBuffer,"RX") != NULL)
	{
		return SX1262_RX;
	}
	else
	{
		return SX1262_CX;
	}
}

/* Reset the chip */
SX1262_STATUS SX1262_Reset(void)
{
	memset(rxBuffer,0x00,32);

	char tx_param[] = "AT+RESET\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	// Check whether the SX1262 is busy..
	if(HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY) == HAL_OK)
	{
		return SX1262_OK;
	}
	else
		return SX1262_ERROR;
}

/* Set the LoRA modem frequency */
SX1262_STATUS SX1262_Set_Freq(void)
{
	// Check whether the SX1262 is busy or not..
	while(!(SX1262_IDLE == SX1262_Status()));

	memset(rxBuffer,0x00,32);
	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+FREQ=923000000\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Get frequency of LoRA */
SX1262_STATUS SX1262_Get_Freq(void)
{
	memset(rxBuffer,0x00,32);

	char tx_param[] = "AT+FREQ?\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	HAL_Delay(100);

	if(strstr(rxBuffer,"OK") != NULL)
	{
		return SX1262_OK;
	}
	else
		return SX1262_ERROR;
}

/* Set the modem type */
SX1262_STATUS SX1262_Set_Modem(void)
{
	// Check for the SX1262 moduleis busy or not..
	while(!(SX1262_IDLE == SX1262_Status()));
	memset(rxBuffer,0x00,32);

	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+MODEM=1\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

SX1262_STATUS SX1262_Get_Modem(void)
{
	memset(rxBuffer,0x00,32);

	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+MODEM?\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	HAL_Delay(100);

	if(strstr(rxBuffer,"OK") != NULL)
	{
		return SX1262_OK;
	}
	else if(strstr(rxBuffer,"BUSY") != NULL)
	{
		return SX1262_BUSY;
	}
	else
		return SX1262_ERROR;
}

/* Configure the LoRA Module */
SX1262_STATUS SX1262_Set_LM_Config(void)
{
	while(SX1262_BUSY == SX1262_Status());
	// Clear the buffer for erasing previous data
	memset(rxBuffer,0x00,32);

	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+LMCFG=7,0,1\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Get LoRA module configuration*/
SX1262_STATUS SX1262_Get_LM_Config(void)
{
	// Clear the buffer before receiving data.
	memset(rxBuffer,0x00,32);

	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+LMCFG?\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(strstr(rxBuffer,"OK") != NULL)
	{
		return SX1262_OK;
	}
	else if(strstr(rxBuffer,"BUSY") != NULL)
	{
		return SX1262_BUSY;
	}
	else
		return SX1262_ERROR;
}

/* Set transmitter power */
SX1262_STATUS SX1262_Set_TX_Power(void)
{
	memset(rxBuffer,0x00,32);

	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+TXPWR=22\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Get transmitter power */
SX1262_STATUS SX1262_Get_TX_Power(void)
{
	memset(rxBuffer,0x00,32);

	/* MODEM VAL = 1(LoRA) */
	char tx_param[] = "AT+TXPWR?\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;


}

/* Send Packet */
SX1262_STATUS SX1262_PKT(void)
{
	while(!(SX1262_IDLE == SX1262_Status()));
	memset(rxBuffer,0x00,32);

	uint8_t data = 0xF1;
	char tx_param[32];
	/* MODEM VAL = 1(LoRA) */
	sprintf(tx_param,"AT+PKT=3,%02x\r\n",data);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);


	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Send Data over LoRAWAN */
SX1262_STATUS SX1262_Send(void)
{
	while(SX1262_BUSY == SX1262_Status());
	memset(rxBuffer,0x00,32);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	char tx_param[32];
	/* MODEM VAL = 1(LoRA) */
	sprintf(tx_param,"AT+SEND=1,2000\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Set LoRA Packet Configuration */
SX1262_STATUS SX1262_Set_LPCFG(void)
{
	while(SX1262_BUSY == SX1262_Status());
	memset(rxBuffer,0x00,32);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	char tx_param[32];
	/* MODEM VAL = 1(LoRA) */
	sprintf(tx_param,"AT+LPCFG=8,0,1,0,0\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else if(strstr(rxBuffer,"BUSY") != NULL)
		{
			return SX1262_BUSY;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;

	while(SX1262_BUSY == SX1262_Status());
}

/* Check LoRA Packet Configuration */
SX1262_STATUS SX1262_Get_LPCFG(void)
{

	memset(rxBuffer,0x00,32);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	char tx_param[32];
	/* MODEM VAL = 1(LoRA) */
	sprintf(tx_param,"AT+LPCFG?\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	HAL_Delay(100);
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Stops the LoRA module */
SX1262_STATUS SX1262_Stop(void)
{
	memset(rxBuffer,0x00,32);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	char tx_param[32];
	/* MODEM VAL = 1(LoRA) */
	sprintf(tx_param,"AT+STOP\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
			memset(rxBuffer,0x00,32);
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Stops the LoRA module */
SX1262_STATUS SX1262_Receive(void)
{

	memset(rxBuffer,0x00,32);

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, 32);

	char tx_param[32];
	/* MODEM VAL = 1(LoRA) */
	sprintf(tx_param,"AT+RECV=0,1\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)tx_param, sizeof(tx_param), HAL_MAX_DELAY);

	HAL_Delay(100);
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE))
	{
		if(strstr(rxBuffer,"OK") != NULL)
		{
			return SX1262_OK;
		}
		else
			return SX1262_ERROR;
	}
	else
		return SX1262_ERROR;
}

/* Initialize the LoRA Module for TX/Rx packets */
void SX1262_Init(void)
{
	SX1262_Reset();
	HAL_Delay(100);

	SX1262_Set_Modem();
	HAL_Delay(10);

	SX1262_PKT();
	HAL_Delay(10);

	SX1262_Set_LM_Config();
	HAL_Delay(10);

	SX1262_Set_TX_Power();
	HAL_Delay(10);
}
