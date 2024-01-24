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
	_F1_GPIO_Port_Lists[port]->CRH &= ~(0x03 << location_mode);
	_F1_GPIO_Port_Lists[port]->CRH |= (mode << location_mode);

	_F1_GPIO_Port_Lists[port]->CRH &= ~(0x03 << location_cnf);
	_F1_GPIO_Port_Lists[port]->CRH |= (conf << location_cnf);
    }
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













