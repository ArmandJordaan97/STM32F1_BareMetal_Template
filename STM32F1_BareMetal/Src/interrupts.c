/*
 * interrupts.c
 *
 *  Created on: Jan 26, 2024
 *      Author: Armand
 */
#include <stdio.h>
#include "stm32f1xx.h"
#include "uart.h"
#include "adc.h"

void EXTI0_IRQHandler(void)
{
    EXTI->PR |= (1 << myButtonPin);
    F1_GPIO_Pin_Toggle(GPIO_PORT_C, GPIO_PIN_13);
}


void USART3_IRQHandler(void)
{
    if(USART3->SR &= USART_SR_RXNE)
    {
	char rec_data = USART3->DR;
	F1_UART_send_char(UART_3, rec_data);
    }
}

void ADC1_2_IRQHandler(void)
{
    if((ADC1->CR1 & ADC_CR1_EOCIE) && (ADC1->SR & ADC_SR_EOC))
    {
	uint16_t adcData = F1_ADC_Get_ADC_Data(ADC_1);
	char b[20] = {'\0'};
	sprintf(b, "ADC1 Int ==== %d\n\r", adcData);
	F1_UART_send_string(UART_3, b);
    }

    if((ADC2->CR1 & ADC_CR1_EOCIE) && (ADC2->SR & ADC_SR_EOC))
    {
	uint16_t adcData = F1_ADC_Get_ADC_Data(ADC_2);
	char b[20] = {'\0'};
	sprintf(b, "ADC2 Int == %d\n\r", adcData);
	F1_UART_send_string(UART_3, b);
    }
}

void DMA1_Channel1_IRQHandler(void)
{
    if(DMA1->ISR & DMA_ISR_TCIF1)
    {
	DMA1_Channel1->CCR &= ~(DMA_CCR_EN);
	DMA1->IFCR |= DMA_IFCR_CGIF1;		// Clear the flag

	uint32_t tot = 0;
	for(uint16_t i=0; i<MAX_DMA_BUFFER; i++)
	{
	    tot += dmaBuf[i];
	}
	uint32_t avg = tot / MAX_DMA_BUFFER;
	char buff[30] = {'\0'};
	sprintf(buff, "DMA =  %lu \r\n", avg);
	F1_UART_send_string(UART_3, buff);
    }
}



