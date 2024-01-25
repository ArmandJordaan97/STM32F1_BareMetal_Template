/*
 * gpio.h
 *
 *  Created on: 24 Jan 2024
 *      Author: Armand
 *
 *      How to use:
 *      1. Ensure the system is Initialized with F1_System_Init() from rcc.h
 *      2. For an output pin:
 *      	- Call F1_GPIO_Pin_Setup_OUT() with the settings as described in the functions below
 *      	- Use the Set/Reset/Toggle functions to control the pin
 *      3. For an input pin:
 *      	- Call F1_GPIO_Pin_Setup_INPUT() with the settings as described in the functions below
 *      	- Call F1_GPIO_Pin_Conf_EXTI() to setup the external interrupt on the pin
 *      	- Use the EXTIx_IRQHandler handler where "x" is the number being used
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
    GPIO_INPUT_PU,
    GPIO_INPUT_PD
}F1_gpio_input_config_t;

/*
 * Typedef to select if the external interrupt is enabled on the input
 */
typedef enum{
    GPIO_INPUT_EXTI_DIS = 0,
    GPIO_INPUT_EXTI_EN
}F1_gpio_input_exti_t;

/*
 * Typedef to select if the external interrupt should trigger on rising or falling edge
 */
typedef enum{
    GPIO_INPUT_EXTI_TRIG_FALL = 0,
    GPIO_INPUT_EXTI_TRIG_RISNG
}F1_gpio_input_exti_trig_t;

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
 * 	Configure the selected Port and Pin to the selected mode
 * @arg:
 * 	For example PA6
 * 	F1_gpio_port_t 		GPIO port (GPIO_PORT_A)
 * 	F1_gpio_pin_t		Pin (GPIO_PIN_6)
 * @return:
 * 	F1_gpio_status_t	Status
 */
F1_gpio_status_t F1_GPIO_Pin_Setup_INPUT(F1_gpio_port_t port, F1_gpio_pin_t pin, F1_gpio_input_config_t conf);

/*
 * @brief:
 * 	Enable the External Interrupt for this pin
 * @arg:
 * 	For example PA6
 * 	F1_gpio_port_t 		GPIO port (GPIO_PORT_A)
 * 	F1_gpio_pin_t		Pin (GPIO_PIN_6)
 * 	F1_gpio_input_exti_t	Enable or disable
 * @return:
 * 	F1_gpio_status_t	Status
 */
F1_gpio_status_t F1_GPIO_Pin_Conf_EXTI(F1_gpio_port_t port, F1_gpio_pin_t pin, F1_gpio_input_exti_t exti, F1_gpio_input_exti_trig_t edge);

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
