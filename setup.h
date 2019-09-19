/**
  * @file setup.h
  * @brief Header file of the setup.c source file.
  * @author Nicholas Chan
  * @author Niklas Henderson
  * @date 10/5/2010.
  */ 

#include "cmsis_os.h"


void SystemClock_Config(void);
extern uint32_t os_time;
uint32_t HAL_GetTick(void);

void enablePin(int pinNumber);
void resetPin(int pinNumber);
void initAsInput(int inputPin);
void initAsOutput(int inputPin);