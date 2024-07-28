#ifndef __ISR_H
#define __ISR_H

#include <stdio.h>

extern char Serial_RxPacket[100];
extern uint8_t Serial_RxFlag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
//USART_TypeDef *uart_index[3] = { USART1 , USART2 , USART3 };

#endif
