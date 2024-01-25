/*
 * gpio.c
 *
 *  Created on: 24 Jan 2024
 *      Author: Armand
 */

#include "gpio.h"

static GPIO_TypeDef *_F1_GPIO_Port_Lists[] = {GPIOA,
						GPIOB,
						GPIOC,
						GPIOD,
						GPIOE};


static IRQn_Type _F1_Exti_lines[] = {EXTI0_IRQn,		// Pin 0
					EXTI1_IRQn,		// Pin 1
					EXTI2_IRQn,		// Pin 2
					EXTI3_IRQn,		// Pin 3
					EXTI4_IRQn,		// Pin 4
					EXTI9_5_IRQn,		// Pin 5
					EXTI9_5_IRQn,		// Pin 6
					EXTI9_5_IRQn,		// Pin 7
					EXTI9_5_IRQn,		// Pin 8
					EXTI9_5_IRQn,		// Pin 9
					EXTI15_10_IRQn,		// Pin 10
					EXTI15_10_IRQn,		// Pin 11
					EXTI15_10_IRQn,		// Pin 12
					EXTI15_10_IRQn,		// Pin 13
					EXTI15_10_IRQn,		// Pin 14
					EXTI15_10_IRQn,		// Pin 15
};

/*
 * Configure the pin with the selected mode
 */
F1_gpio_status_t F1_GPIO_Pin_Setup_OUT(F1_gpio_port_t port, F1_gpio_pin_t pin, F1_gpio_output_mode_t mode, F1_gpio_output_config_t conf)
{
    /* Enable the GPIO port clock */
    if(F1_GPIO_Clock_Enable(port) != GPIO_OK)
    {
	return GPIO_FAIL;
    }
    if(pin < 8)
    {
	uint8_t location_mode = (pin*4);
	uint8_t location_cnf = (pin*4)+2;
	_F1_GPIO_Port_Lists[port]->CRL &= ~(0x03 << location_mode);
	_F1_GPIO_Port_Lists[port]->CRL |= (mode << location_mode);

	_F1_GPIO_Port_Lists[port]->CRL &= ~(0x03 << location_cnf);
	_F1_GPIO_Port_Lists[port]->CRL |= (conf << location_cnf);
    }
    else
    {
	uint8_t location_mode = ((pin-8)*4);
	uint8_t location_cnf = ((pin-8)*4)+2;
	_F1_GPIO_Port_Lists[port]->CRH &= ~(0x3 << location_mode);
	_F1_GPIO_Port_Lists[port]->CRH |= (mode << location_mode);

	_F1_GPIO_Port_Lists[port]->CRH &= ~(0x3 << location_cnf);
	_F1_GPIO_Port_Lists[port]->CRH |= (conf << location_cnf);
    }
    return GPIO_OK;
}

/*
 * Configure the pin as input with the selected configuration
 */
F1_gpio_status_t F1_GPIO_Pin_Setup_INPUT(F1_gpio_port_t port, F1_gpio_pin_t pin, F1_gpio_input_config_t conf)
{
    /* Enable the GPIO port clock */
    if(F1_GPIO_Clock_Enable(port) != GPIO_OK)
    {
	return GPIO_FAIL;
    }
    if(pin < 8)
    {
	uint8_t location_mode = (pin*4);
	uint8_t location_cnf = (pin*4)+2;
	_F1_GPIO_Port_Lists[port]->CRL &= ~(0x3 << location_mode);	// Clear the mode for Input

	_F1_GPIO_Port_Lists[port]->CRL &= ~(0x3 << location_cnf);	// Clear the CNF register for a clean start
	_F1_GPIO_Port_Lists[port]->CRL |= (conf << location_cnf);	// Set the pin as the desired mode
	if(conf == GPIO_INPUT_PU)
	{
	    _F1_GPIO_Port_Lists[port]->ODR |= (1 << pin);		// Set the pin in ODR to enable pull-up
	}
	else if(conf == GPIO_INPUT_PD)
	{
	    _F1_GPIO_Port_Lists[port]->ODR &= ~(1 << pin);		// Set the pin in ODR to enable pull-up
	}
    }
    else
    {
	uint8_t location_mode = ((pin-8)*4);
	uint8_t location_cnf = ((pin-8)*4)+2;
	_F1_GPIO_Port_Lists[port]->CRH &= ~(0x3 << location_mode);	// Clear the mode for Input

	_F1_GPIO_Port_Lists[port]->CRH &= ~(0x3 << location_cnf);
	_F1_GPIO_Port_Lists[port]->CRH |= (conf << location_cnf);
	if(conf == GPIO_INPUT_PU)
	{
	    _F1_GPIO_Port_Lists[port]->ODR |= (1 << pin);		// Set the pin in ODR to enable pull-up
	}
	else if(conf == GPIO_INPUT_PD)
	{
	    _F1_GPIO_Port_Lists[port]->ODR &= ~(1 << pin);		// Set the pin in ODR to enable pull-up
	}
    }
    return GPIO_OK;
}


/*
 * Enable of disable the EXTI
 */
F1_gpio_status_t F1_GPIO_Pin_Conf_EXTI(F1_gpio_port_t port, F1_gpio_pin_t pin, F1_gpio_input_exti_t exti, F1_gpio_input_exti_trig_t edge)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;		// Enable the AF clock
    AFIO->EXTICR[pin >> 2] |= (port << (pin*4));	// Enable the pin on EXTI0
    if(exti == GPIO_INPUT_EXTI_EN)
    {
	EXTI->IMR |= (1 << pin);	// Enable the interrupt on this pin
	if(edge == GPIO_INPUT_EXTI_TRIG_RISNG)
	{
	    EXTI->RTSR |= (1 << pin);
	}
	else if(edge == GPIO_INPUT_EXTI_TRIG_FALL)
	{
	    EXTI->FTSR |= (1 << pin);
	}
    }

    NVIC_EnableIRQ(_F1_Exti_lines[pin]);
    NVIC_SetPriority(_F1_Exti_lines[pin], 2);
    return GPIO_OK;
}


/*
 * Enable the clock of the GPIO peripheral
 */
F1_gpio_status_t F1_GPIO_Clock_Enable(F1_gpio_port_t port)
{
    switch(port)
    {
	case GPIO_PORT_A:
	    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		// Enable the peripheral clock
	    break;
	case GPIO_PORT_B:
	    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;		// Enable the peripheral clock
	    break;
	case GPIO_PORT_C:
	    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;		// Enable the peripheral clock
	    break;
	case GPIO_PORT_D:
	    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;		// Enable the peripheral clock
	    break;
	case GPIO_PORT_E:
	    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;		// Enable the peripheral clock
	    break;
	default:
	    return GPIO_FAIL;
	    break;
    }
    return GPIO_OK;
}

/*
 * Toggle the GPIO pin
 */
void F1_GPIO_Pin_Toggle(F1_gpio_port_t port, F1_gpio_pin_t pin)
{
    /* Check the status of the pin (high or low) */
    if(_F1_GPIO_Port_Lists[port]->ODR & (1 << pin))
    {
	/* Pin is high, reset it */
	F1_GPIO_Pin_Reset(port, pin);
    }
    else
    {
	/* Pin is low, set it */
	F1_GPIO_Pin_Set(port, pin);
    }
}

/*
 * Sets the pin
 */
void F1_GPIO_Pin_Set(F1_gpio_port_t port, F1_gpio_pin_t pin)
{
    /* Set the pin to high */
    _F1_GPIO_Port_Lists[port]->BSRR |= (1 << pin);
}

/*
 * Resets the pin
 */
void F1_GPIO_Pin_Reset(F1_gpio_port_t port, F1_gpio_pin_t pin)
{
    /* Reset the pin to low */
    _F1_GPIO_Port_Lists[port]->BSRR |= (1 << (16+pin));
}











