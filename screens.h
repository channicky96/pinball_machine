/**
  * @file screens.h
  * @brief Header file of the screens.c source file.
  * @author Nicholas Chan
  * @author Niklas Henderson
  * @date 10/5/2010.
  */ 

#include "Board_Touch.h"
#include <stdbool.h>

/**
  * @brief An enum containing different type of GUI screens
  */
enum screen{
	Home, 
	Game,
	Error
};

/**
  * @brief A struct containing connection status and settings
  */
typedef struct{
	bool connection;
	int connectedCount;
	char* ip;
	}settings;

void home(enum screen* currentScreen, TOUCH_STATE* tsc_state, settings* curSettings);
void game(enum screen* currentScreen, TOUCH_STATE* tsc_state, settings* curSettings);
void error(enum screen* currentScreen, TOUCH_STATE* tsc_state);
