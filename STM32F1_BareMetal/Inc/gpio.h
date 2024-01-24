/*
 * gpio.h
 *
 *  Created on: 24 Jan 2024
 *      Author: Armand
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f1xx.h"

/*
 * Typedef to select the GPIO Port
 */
typedef enum{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E
}F1_gpio_port_t;

/*
 * Typedef to select the GPIO pin
 */
 typedef enum{
     GPIO_PIN_0 = 0,
     GPIO_PIN_1,
     GPIO_PIN_2,
     GPIO_PIN_3,
     GPIO_PIN_4,
     GPIO_PIN_5,
     GPIO_PIN_6,
     GPIO_PIN_7,
     GPIO_PIN_8,
     GPIO_PIN_9,
     GPIO_PIN_10,
     GPIO_PIN_11,
     GPIO_PIN_12,
     GPIO_PIN_13,
     GPIO_PIN_14,
     GPIO_PIN_15
}F1_gpio_pin_t;

/*
 * Typedef to select the config of the input pin
 */
typedef enum{
    GPIO_INPUT_ANALOG = 0,
    GPIO_INPUT_FLOAT,
    GPIO_INPUT_PU_PD,
}F1_gpio_input_config_t;

/*
 * Typedef to select the mode of the output pin
 */
typedef enum{
    GPIO_OUTPUT_10MHZ = 1,
    GPIO_OUTPUT_2MHZ,
    GPIO_OUTPUT_50MHZ
}F1_gpio_output_mode_t;

/*
 * Typedef to select the config of the output pin
 */
typedef enum{
    GPIO_OUTPUT_PP = 0,		// Output Push-Pull
    GPIO_OUTPUT_OD,		// Output Open Drain
    GPIO_OUTPUT_AF_PP,		// Output Alternate Function Push-Pull
    GPIO_OUTPUT_AF_OD		// Output Alternate Function Open Drain
}F1_gpio_output_config_t;

/*
 * Typedef for the status of the GPIO
 */
 typedef enum{
     GPIO_OK = 0,
     GPIO_FAIL
 }F1_gpio_status_t;


 /*
  * ********************************************************************************
  * Functions
  * ********************************************************************************
  */

 /*
 * @brief:
 * 	Configure the selected Port and Pin to the selected mode
 * @arg:
 * 	For example PA6
 * 	F1_gpio_port_t 		GPIO port (GPIO_PORT_A)
 * 	F1_gpio_pin_t		Pin (GPIO_PIN_6)
 * 	F1_gpio_mode_t		Mode for pin (GPIO_INPUT_PD)
 * @return:
 * 	F1_gpio_status_t	Status
 */
F1_gpio_status_t F1_GPIO_Pin_Setup_OUT(F1_gpio_port_t port, F1_gpio_pin_t pin, F1_gpio_output_mode_t mode, F1_gpio_output_config_t conf);


 /*
 * @brief:
 * 	Enable the selected Port's clock
 * @arg:
 * 	F1_gpio_port_t 		GPIO port
 * @return:
 * 	F1_gpio_status_t	Status
 */
F1_gpio_status_t F1_GPIO_Clock_Enable(F1_gpio_port_t port);

 /*
 * @brief:
 * 	Toggle the selected pin
 * @arg:
 * 	F1_gpio_port_t 		GPIO port
 * 	F1_gpio_pin_t		GPIO Pin
 * @return:
 * 	void
 */
void F1_GPIO_Pin_Toggle(F1_gpio_port_t port, F1_gpio_pin_t pin);

 /*
 * @brief:
 * 	Sets the selected pin
 * @arg:
 * 	F1_gpio_port_t 		GPIO port
 * 	F1_gpio_pin_t		GPIO Pin
 * @return:
 * 	None
 */
void F1_GPIO_Pin_Set(F1_gpio_port_t port, F1_gpio_pin_t pin);

 /*
 * @brief:
 * 	Resets the selected pin
 * @arg:
 * 	F1_gpio_port_t 		GPIO port
 * 	F1_gpio_pin_t		GPIO Pin
 * @return:
 * 	None
 */
void F1_GPIO_Pin_Reset(F1_gpio_port_t port, F1_gpio_pin_t pin);












#endif /* GPIO_H_ */
