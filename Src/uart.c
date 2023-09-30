/*
 * uart.c
 *
 *  Created on: Sep 22, 2023
 *      Author: malef
 */

#include <stdio.h>
#include "uart.h"
#include "gpio_driver.h"
#include "stm32f411xe.h"

static void uart_write(int ch);

/* use uart for printf*/
int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}


void uart_tx_init(void){
	/*Initialize USRAT2 pins in alt fn mode*/
	GPIO_Handle_t uart_pin;

	/*Enable clock for GPIOD*/
	GPIO_ClockConfig(GPIOD, ENABLE);

	/*pin configurations*/
	uart_pin.GPIOx = GPIOD;

	uart_pin.GPIOx_Cofig.GPIO_OPType = PushPull;
	uart_pin.GPIOx_Cofig.GPIO_PinAltFnMode = AF7;
	uart_pin.GPIOx_Cofig.GPIO_PinMode = GPIO_MODE_Alt;

	uart_pin.GPIOx_Cofig.GPIO_PinNumber = GPIO_PIN_5;
	uart_pin.GPIOx_Cofig.GPIO_PinSpeed = LowSpeed;
	uart_pin.GPIOx_Cofig.GPIO_PullUpPullDown_Ctrl = NoPullUpOrPullDown;

	GPIO_Init(&uart_pin); /*initialize tx pin*/

	/*Enable clock for USART2*/
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/*configure baudrate*/
	USART2->BRR = 0x683; /*9600 bits/s*/

	/*uart tx_mode*/
	USART2->CR1 |= USART_CR1_TE; /*Transmit enable*/

	/*enable uart*/
	USART2->CR1 |= USART_CR1_UE;
}

static void uart_write(int ch){
	/*wait until transmit buffer is enpty*/
	while(!(USART2->SR & USART_SR_TXE));
	/*write data to transmit data register*/
	USART2->DR = (0xFF&ch);
}



