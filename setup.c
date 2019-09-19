/**
  * @file setup.c
  * @brief Define functions that will be used accross the project.
  * @author Nicholas Chan
  * @date 10/5/2010.
  */ 
  
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_tim.h"
#include "Board_LED.h"
#include "cmsis_os.h"
#include "setup.h"

/**
  * @brief A variable containing GPIO pin numbers according to the discovery board pin number.
  *        e.g. pin number 1 corresponds to GPIOX pin 7.
  */
uint16_t globalPins[8] = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_6, GPIO_PIN_4, GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_6, GPIO_PIN_3};

/**
  * @brief A variable containing GPIO ports according to the discovery board pin number.
  *        e.g. pin number 1 corresponds to GPIOC pin X.
  */
GPIO_TypeDef *globalPorts[8] = {GPIOC, GPIOC, GPIOG, GPIOB, GPIOG, GPIOI, GPIOH, GPIOI};

#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
} 
#endif

/**
  * @brief GPIO clocks and timers configuration.
  * @param None.
  * @returns Void.
  */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM12_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |
	RCC_CLOCKTYPE_PCLK1 |
	RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource =
	RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * @brief This method will enable a GPIO pin given a pin number.
  * @param pinNumber The pin number to be enabled.
  * @returns Void.
  */
void enablePin(int pinNumber){
	HAL_GPIO_WritePin(globalPorts[pinNumber], globalPins[pinNumber] , GPIO_PIN_SET);
}

/**
  * @brief This method will reset a GPIO pin given a pin number.
  * @param pinNumber The pin number to be reset.
  * @returns Void
  */
void resetPin(int pinNumber){
	HAL_GPIO_WritePin(globalPorts[pinNumber], globalPins[pinNumber] , GPIO_PIN_RESET);
}

/**
  * @brief This method will initialise a GPIO pin as an input pin given a pin number.
  * @param inputPin The pin number to be initialise.
  * @returns Void.
  */
void initAsInput(int inputPin){
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_INPUT; 
	gpio.Pull = GPIO_PULLDOWN; 
	gpio.Speed = GPIO_SPEED_HIGH; 
	gpio.Pin = globalPins[inputPin];
	HAL_GPIO_Init(globalPorts[inputPin], &gpio);
}

/**
  * @brief This method will initialise a GPIO pin as an output pin given a pin number.
  * @param inputPin The pin number to be initialise.
  * @returns Void.
  */
void initAsOutput(int inputPin){
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 
	gpio.Pull = GPIO_NOPULL; 
	gpio.Speed = GPIO_SPEED_HIGH; 
	gpio.Pin = globalPins[inputPin];
	HAL_GPIO_Init(globalPorts[inputPin], &gpio);
	HAL_GPIO_WritePin(globalPorts[inputPin], globalPins[inputPin] , GPIO_PIN_RESET);
}
