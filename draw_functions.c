/**
  * @file draw_functions.c
  * @brief Define functions that will be used to .
  * @author Niklas Henderson
  * @date 10/5/2010.
  */ 

#include "main.h"
#include "draw_functions.h"

/**
  * @brief A function to generate stars in the background,
  *        So background is different everytime,
  *        to be used in drawBackground function.
  * @param x,y the co-ordinates of the stars
  * @returns Void.
  */
void drawStar(int x, int y){
	GLCD_SetForegroundColor (GLCD_COLOR_WHITE);

	GLCD_DrawPixel(x, y);
	GLCD_DrawPixel(x-1, y);
	GLCD_DrawPixel(x+1, y);
	GLCD_DrawPixel(x, y-1);
	GLCD_DrawPixel(x, y+1);
	return;
}

/**
  * @brief A function to generate the background for the GUI
  * @param None.
  * @returns Void.
  */
void drawBackground(void){
	unsigned int n;
	GLCD_SetBackgroundColor (GLCD_COLOR_BLACK);
	GLCD_ClearScreen ();
	GLCD_SetForegroundColor (GLCD_COLOR_DARK_GREY);
	GLCD_DrawBargraph(0, 0, 480, 15, 100);
	GLCD_DrawBargraph(0, 257, 480, 15, 100);
	GLCD_DrawBargraph(0, 0, 15, 272, 100);
	GLCD_DrawBargraph(465, 0, 15, 272, 100);
	
	GLCD_SetForegroundColor (GLCD_COLOR_LIGHT_GREY);
	GLCD_DrawBargraph(15, 257, 455, 5, 100);
	GLCD_DrawBargraph(465, 15, 5, 242, 100);
	
	GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
	for(n=0; n<100; n++){
		drawStar((rand() % 445)+15, (rand() % 237)+15);
	}
	drawStar(100, 100);
	return;
}