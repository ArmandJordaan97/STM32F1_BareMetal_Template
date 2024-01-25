/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f1xx.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"

#define myLEDport	GPIO_PORT_C
#define myLEDpin	GPIO_PIN_13

#define myButtonPort	GPIO_PORT_B
#define myButtonPin	GPIO_PIN_0

void EXTI0_IRQHandler(void)
{
    EXTI->PR |= (1 << myButtonPin);
    F1_GPIO_Pin_Toggle(GPIO_PORT_C, GPIO_PIN_13);
}

void USART1_IRQHandler(void)
{
    if(USART1->SR &= USART_SR_RXNE)
    {
	char rec_data = USART1->DR;
	F1_UART_send_char(UART_1, rec_data);
    }
}



int main(void)
{
    F1_System_Init(HSE_72MHZ, SYSTICK_1KHZ);
    F1_GPIO_Pin_Setup_OUT(myLEDport, myLEDpin, GPIO_OUTPUT_10MHZ, GPIO_OUTPUT_PP);
    F1_GPIO_Pin_Setup_INPUT(myButtonPort, myButtonPin, GPIO_INPUT_PU);
    F1_GPIO_Pin_Conf_EXTI(myButtonPort, myButtonPin, GPIO_INPUT_EXTI_EN, GPIO_INPUT_EXTI_TRIG_FALL);
    F1_UART_Init(UART_3, UART_BAUD_115200, UART3_TX_PB10_RX_PB11);

    char buff[30] = {'\0'};
    int t = 0;
    while(1)
    {
	sprintf(buff, "%d Val\n\r", t);
	t+=1;
	F1_UART_send_string(UART_3, buff);
	F1_GPIO_Pin_Toggle(GPIO_PORT_C, GPIO_PIN_13);
	F1_delay_ms(1000);

    }
}




















