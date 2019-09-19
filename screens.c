/**
  * @file screens.c
  * @brief Define functions to display different screens.
  * @author Niklas Henderson
  * @author Nicholas Chan
  * @date 10/5/2010.
  */   
  
#include "main.h"
#include "setup.h"
#include "screens.h"
#include "draw_functions.h"

/**
  * @brief A variable of type GLCD_FONT denoting the font size of 6x8.
  */
extern GLCD_FONT 		GLCD_Font_6x8;

/**
  * @brief A variable of type GLCD_FONT denoting the font size of 16x24.
  */
extern GLCD_FONT 		GLCD_Font_16x24;

// For debugging purposes
//extern char buffer[5];
//extern char buffer2[5];
//extern char buffer3[5];

/**
  * @brief A variable indicating the current score of player 1.
  */
extern int player1Score;

/**
  * @brief A variable indicating the current score of player 2.
  */
extern int player2Score;

/**
  * @brief A variable used to check IR sensor value for player 1.
  */
int lastPlayer1 = 500;

/**
  * @brief A variable used to check IR sensor value for player 2.
  */
int lastPlayer2 = 500;

/**
  * @brief A variable inidicating current IR sensor value for player 1,
  *        used to compare with lastPlayer1 to determine if player 2 has scored.
  */
int currentPlayer1;

/**
  * @brief A variable inidicating current IR sensor value for player 2,
  *        used to compare with lastPlayer2 to determine if player 1 has scored.
  */
int currentPlayer2;

/**
  * @brief A variable used store player1Score, to be used for displaying on GLCD screen.
  */
char player1Buffer[2];

/**
  * @brief A variable used store player2Score, to be used for displaying on GLCD screen.
  */
char player2Buffer[2];

/**
  * @brief A function used to display the "error" screen on the GLCD.
  * @param currentScreen variable to determine which screen the GLCD should display.
  * @param tsc_state a variable to check if a "button" is pressed on the GLCD screen.
  * @returns Void.
  */
void error(enum screen* currentScreen, TOUCH_STATE* tsc_state){
	int counter;
	int lightValue = 0;
	int averageLight = 0;
	player2Score = 0;
	player1Score = 0;
	drawBackground();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
	GLCD_DrawString (170, 50, "Error. Lid open");
    // When lid is opened, enable the amber LED and disable the green LED
	enablePin(5);
	resetPin(7);
	 
	for(;;){
        // For checking if lid is close by utilising the light sensor value
		// GLCD_DrawString (330, 250, buffer3);
		averageLight = 0;
		for(counter=0; counter<20; counter++){
				sscanf(buffer3, "%d", &lightValue);
				averageLight += lightValue;
		}
		averageLight = averageLight/20;
		
		if(averageLight > 4050){
			*currentScreen = Game;
			break;
		}
	}
	
}

/**
  * @brief A function used to display the "home" screen on the GLCD.
  * @param currentScreen Variable to determine which screen the GLCD should display.
  * @param tsc_state A variable to check if a "button" is pressed on the GLCD screen.
  * @param curSettings For checking if both player are connected
  * @returns Void.
  */
void home(enum screen* currentScreen, TOUCH_STATE* tsc_state, settings* curSettings) {
	char* devicesString = (char*)malloc(13 * sizeof(char));
	bool ready = true;
	player2Score = 0;
	player1Score = 0;
	drawBackground();

	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
	GLCD_DrawString (200, 50, "Home");
	
	GLCD_SetFont(&GLCD_Font_6x8);
	
	GLCD_SetForegroundColor (GLCD_COLOR_LIGHT_GREY);
	GLCD_DrawBargraph(170, 150, 130, 50, 100);
	GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
	GLCD_SetBackgroundColor (GLCD_COLOR_LIGHT_GREY);
	
	if(curSettings->connectedCount == 2 & curSettings->connection == true){
		ready = true;
		GLCD_DrawString (195, 170, "START GAME");
	}else{
		GLCD_DrawString (190, 170, "START GAME");
	}

	while(1){
		Touch_GetState(tsc_state); 
		if(tsc_state->pressed){
			if(tsc_state->x > 170 & tsc_state->x <300
						& tsc_state->y > 150 & tsc_state->y < 200
						& ready == true){
						*currentScreen = Game;
						return;
			}}
	}
	return;
}

/**
  * @brief A function used to display the "game" screen on the GLCD.
  * @param currentScreen Variable to determine which screen the GLCD should display.
  * @param tsc_state A variable to check if a "button" is pressed on the GLCD screen.
  * @param curSettings For checking if both player are connected
  * @returns Void.
  */
void game(enum screen* currentScreen, TOUCH_STATE* tsc_state, settings* curSettings){
	int lightValue = 0;
	int averageLight = 0;
	int counter;
	drawBackground();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
	GLCD_DrawString (70, 50, "Player 1");
	GLCD_DrawString (270, 50, "Player 2");
	
	GLCD_DrawVLine(240, 25, 222);	
	
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_SetForegroundColor (GLCD_COLOR_LIGHT_GREY);
	GLCD_DrawBargraph(20, 20, 50, 30, 100);
	GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
	GLCD_SetBackgroundColor (GLCD_COLOR_LIGHT_GREY);
	GLCD_DrawString (30, 30, "back");

	GLCD_SetBackgroundColor (GLCD_COLOR_BLACK);
	GLCD_SetFont(&GLCD_Font_16x24);

	enablePin(7);
	resetPin(5);
	
	while(1){
		Touch_GetState(tsc_state); 
		if(tsc_state->pressed){
			if(tsc_state->x > 20 & tsc_state->x <70
					& tsc_state->y > 20 & tsc_state->y < 50){
					*currentScreen = Home;
					return;
			}
		}
		sprintf(player1Buffer, "%d", player1Score);
		sprintf(player2Buffer, "%d", player2Score);
		
		GLCD_DrawString (110, 120, player1Buffer);
	    GLCD_DrawString (330, 120, player2Buffer);

		//sscanf(buffer, "%d", &currentPlayer1);
		//sscanf(buffer2, "%d", &currentPlayer2);
		
		if (lastPlayer1 - currentPlayer1 > 50){
			player1Score++;
		}
		lastPlayer1 = currentPlayer1;
		
		if(currentPlayer2 < 125){
			player2Score++;
		}else if (lastPlayer2 - currentPlayer2  > 40){
			player2Score++;
		}
		lastPlayer2 = currentPlayer2;
		
		averageLight = 0;
		for(counter=0; counter<5; counter++){
			sscanf(buffer3, "%d", &lightValue);
			averageLight += lightValue;
		}
		averageLight = averageLight/5;
		
		if(averageLight < 3700){
			*currentScreen = Error;
			break;
		}
		
		osDelay(10);
	}
	
	return;
}
	
	
