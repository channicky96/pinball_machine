/**
  * @file main.c
  * @brief Main runner of the project.
  * @author Nicholas Chan
  * @author Niklas Henderson
  * @date 10/5/2010.
  */ 



#include "setup.h"
#include "main.h"
#include "draw_functions.h"
#include "screens.h"

/**
  * @brief A variable of type GLCD_FONT denoting the font size of 6x8.
  */
extern GLCD_FONT 		GLCD_Font_6x8;
/**
  * @brief A variable of type GLCD_FONT denoting the font size of 16x24.
  */
extern GLCD_FONT 		GLCD_Font_16x24;


/**
  * @brief An enum containing different type of GUI screens.
  */
enum screen currentScreen = Home;

/**
  * @brief Thread function for player 1 paddles handling.
  */
void player1Task (void const* argument);
/**
  * @brief Thread function for player 2 paddles handling.
  */
void player2Task (void const* argument);
/**
  * @brief Thread function for handling analog to digital conversion, and handling infrared and light sensors.
  */
void analogTask (void const* argument);

/**
  * @brief typedef used for TIM3 configuration.
  */
TIM_HandleTypeDef htim3;
/**
  * @brief typedef used for TIM12 configuration.
  */
TIM_HandleTypeDef htim12;
/**
  * @brief typedef used for Analog to Digital Conversion configuration.
  */
ADC_HandleTypeDef AdcHandle1;
/**
  * @brief typedef used for Analog to Digital Conversion, Channel 1.
  */
ADC_ChannelConfTypeDef adcChannel1;

static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM12_Init(void);
static void MX_TIM3_Init(void);

void Error_Handler(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void ConfigureADC(void);

/**
  * @brief Stores average values for ADC. For Infrared Sensor 1.
  */
int averageValue1;
/**
  * @brief Stores average values for ADC. For Infrared Sensor 2.
  */
int averageValue2;
/**
  * @brief Stores average values for ADC. For Light Sensor.
  */
int averageValue3;


/**
  * @brief A variable indicating the current score of player 1.
  */
int player1Score = 0;
/**
  * @brief A variable indicating the current score of player 2.
  */
int player2Score = 0;


/*
char buffer[5];
char buffer2[5];
char buffer3[5];
*/

/**
  * @brief Thread ID struct for player 1 thread.
  */
osThreadId player1Paddles;
/**
  * @brief Thread ID struct for player 2 thread.
  */
osThreadId player2Paddles;	
/**
  * @brief Thread ID struct for ADC.
  */
osThreadId analogThread;


/**
  * @brief Defining thread configuration struct for player 1.
  */
osThreadDef(player1Task, osPriorityHigh, 1, 0);
/**
  * @brief Defining thread configuration struct for player 2.
  */
osThreadDef(player2Task, osPriorityHigh, 1, 0);
/**
  * @brief Defining thread configuration struct for ADC.
  */
osThreadDef(analogTask, osPriorityNormal, 1, 0);


/**
  * @brief typedef used for ADC configuration.
  */
ADC_HandleTypeDef AdcHandle1;
/**
  * @brief typedef used for ADC configuration with channel 1.
  */
ADC_ChannelConfTypeDef adcChannel1;


/**
  * @brief Calls initialise functions for simple GPIO input and output pins.
  * @param None.
  * @returns Void.
  */
void GPIOSetup(void){
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	initAsInput(0);
	initAsInput(1);
	initAsInput(2);
	initAsOutput(4);
}

/**
  * @brief hard-coded LOW value for servo PWM pulse.
  */
int PWM_LOW = 51;
/**
  * @brief hard-coded HIGH value for servo PWM pulse.
  */
int PWM_HIGH = 58;


/**
  * @brief player 1 thread function. Controls Servos 1 and 2.
  * @param Thread function default argument paramater.
  * @returns Void.
  */
void player1Task (void const* argument) {
	GPIO_PinState bitstatus;
	for(;;){
		bitstatus = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
		if(bitstatus != 0){
			TIM3->CCR1 = PWM_LOW;
			osDelay(300);
			TIM3->CCR1 = PWM_HIGH;
		}else{
		}
		osDelay(50);
	}
}


/**
  * @brief player 2 thread function. Controls Servos 3 and 4.
  * @param Thread function default argument paramater.
  * @returns Void.
  */
void player2Task(void const* argument) {
	GPIO_PinState bitstatus;
	for(;;){
		bitstatus = HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6);
		if(bitstatus != 0){
			TIM12->CCR1 = PWM_HIGH;
			osDelay(300);
			TIM12->CCR1 = PWM_LOW;
		}else{
		}
		osDelay(50);
	}
}



/**
  * @brief ADC thread function. Controls all analog peripherals.
  * @param Thread function default argument paramater.
  * @returns Void.
  */
void analogTask(void const* argument) {
	int runningAverage1;
	int runningAverage2;
	int runningAverage3;
	int counter1 = 0;
	int counter2 = 0;
	int counter3 = 0;
	
	
	for(;;){
		HAL_ADC_Stop(&AdcHandle1);
		adcChannel1.Channel = ADC_CHANNEL_0;
		HAL_ADC_ConfigChannel(&AdcHandle1, &adcChannel1);
		HAL_ADC_Start(&AdcHandle1);
		if (HAL_ADC_PollForConversion(&AdcHandle1, 1000000) == HAL_OK){
			runningAverage1 = 0;
			for (counter1 = 0; counter1 < 100000; counter1++){
				runningAverage1 += HAL_ADC_GetValue(&AdcHandle1);
			}
			averageValue1 = runningAverage1/100000;
		}
		HAL_ADC_Stop(&AdcHandle1);
		adcChannel1.Channel = ADC_CHANNEL_8;
		HAL_ADC_ConfigChannel(&AdcHandle1, &adcChannel1);
		HAL_ADC_Start(&AdcHandle1);
		if (HAL_ADC_PollForConversion(&AdcHandle1, 1000000) == HAL_OK){
			runningAverage2 = 0;
			for (counter2 = 0; counter2 < 100000; counter2++){
				runningAverage2 += HAL_ADC_GetValue(&AdcHandle1);
			}
			averageValue2 = runningAverage2/100000;
		}

		
		HAL_ADC_Stop(&AdcHandle1);
		adcChannel1.Channel = ADC_CHANNEL_6;
		HAL_ADC_ConfigChannel(&AdcHandle1, &adcChannel1);
		HAL_ADC_Start(&AdcHandle1);
		
		if (HAL_ADC_PollForConversion(&AdcHandle1, 1000000) == HAL_OK){
			runningAverage3 = 0;
			for (counter3 = 0; counter3 < 100; counter3++){
				runningAverage3 += HAL_ADC_GetValue(&AdcHandle1);
			}
			averageValue3 = runningAverage3/100;
		}
		sprintf(buffer, "%d", averageValue1);
		sprintf(buffer2, "%d", averageValue2);
		sprintf(buffer3, "%d", averageValue3);
		osDelay(50);
		
	}
}


/**
  * @brief Main runner of the program.
  * @param None.
  * @returns Program exit code.
  */
int main (void) {
	// Variable Setup	

	TOUCH_STATE tsc_state;
	settings curSettings = {false, 0, "foobar"};
	
	// Initialising and Preparing Screen and Touch
	HAL_Init();
	SystemClock_Config();
	
	MX_GPIO_Init();
	MX_TIM12_Init();
	MX_TIM3_Init();
	
	GPIOSetup();
	
	ConfigureADC();
	HAL_ADC_Start(&AdcHandle1);
	
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	
	osKernelInitialize();
	GLCD_Initialize();
	Touch_Initialize();
	player1Paddles = osThreadCreate(osThread(player1Task), NULL);
  player2Paddles = osThreadCreate(osThread(player2Task), NULL);
	analogThread = osThreadCreate(osThread(analogTask), NULL);
	
  osKernelStart();
	//Default screen settings 
  GLCD_SetFont            (&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
	//ADC test screen
	GLCD_SetBackgroundColor (GLCD_COLOR_WHITE);
	GLCD_ClearScreen ();
	GLCD_SetFont (&GLCD_Font_16x24);
	GLCD_SetForegroundColor (GLCD_COLOR_BLACK);
	GLCD_DrawString (150, 100, "Hello Bijan");
	
	initAsOutput(7);
	initAsOutput(5);
	

	while(1){
		switch(currentScreen) {
			
			case Home:
				home(&currentScreen, &tsc_state, &curSettings);
				break;
			
			case Game:
				game(&currentScreen, &tsc_state, &curSettings);
				break;
			case Error:
				error(&currentScreen, &tsc_state);
				break;
			default:
				home(&currentScreen, &tsc_state, &curSettings);
				break;
		}
	}
}


/**
  * @brief Configuration for TIM3.
  * @param None.
  * @returns Void.
  */
static void MX_TIM3_Init(void){
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 2550;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 580;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim3);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);
  HAL_TIM_PWM_Init(&htim3);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = PWM_HIGH;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_MspPostInit(&htim3);
}

/**
  * @brief Configuration for TIM12.
  * @param None.
  * @returns Void.
  */
static void MX_TIM12_Init(void){
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 2550;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 580;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim12);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig);
  HAL_TIM_PWM_Init(&htim12);
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = PWM_LOW;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_MspPostInit(&htim12);
}


/**
  * @brief Enables clocks required for Timers.
  * @param None.
  * @returns Void.
  */
static void MX_GPIO_Init(void){
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
}


/**
  * @brief Configuration function for ADC.
  * @param None.
  * @returns Void.
  */
void ConfigureADC(void){	
	GPIO_InitTypeDef gpioInit;	
	__GPIOA_CLK_ENABLE(); 
	__GPIOF_CLK_ENABLE(); 
	__ADC3_CLK_ENABLE();

	gpioInit.Pin = GPIO_PIN_0;
	gpioInit.Mode = GPIO_MODE_ANALOG;
	gpioInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpioInit);

	gpioInit.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOF, &gpioInit);
	
	gpioInit.Pin = GPIO_PIN_8 ;
	HAL_GPIO_Init(GPIOF, &gpioInit);
	
	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	AdcHandle1.Instance = ADC3;
	AdcHandle1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle1.Init.Resolution = ADC_RESOLUTION_12B;
	AdcHandle1.Init.ScanConvMode = DISABLE;
	AdcHandle1.Init.ContinuousConvMode = ENABLE;
	AdcHandle1.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle1.Init.NbrOfDiscConversion = 0;
	AdcHandle1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle1.Init.NbrOfConversion = 1;
	AdcHandle1.Init.DMAContinuousRequests = ENABLE;
	AdcHandle1.Init.EOCSelection = DISABLE;
	HAL_ADC_Init(&AdcHandle1);
	adcChannel1.Channel = ADC_CHANNEL_0;
	adcChannel1.Rank = ADC_REGULAR_RANK_1;
	adcChannel1.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	adcChannel1.Offset = 0;
	HAL_ADC_ConfigChannel(&AdcHandle1, &adcChannel1);
}


/**
  * @brief Low-level configuration for TIM3 and TIM12.
  * @param None.
  * @returns Void.
  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM3){
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
  else if(htim->Instance==TIM12){
    __HAL_RCC_GPIOH_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  }
}