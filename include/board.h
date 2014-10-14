/*
 * Here lies macros and functions used in multiple files.
 * The Board describes the basic layout and rules that any
 * and every object provided with a tangible existence
 * must follow.
 */
#ifndef BOARD_H
#define BOARD_H


/*~~MACROS~~*/

//Screen dimensions and attributes
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

//FPS and time per frame (in milliseconds)
#define MAX_FRAMES_PER_SEC 50
#define DT 0.02
#define EPSILON 0.01 

//The players
#define P1 1
#define P2 2

//The number of points you need to reach to win
#define MAX_SCORE 3

//The height of the top and bottom sections (line height included)
#define TOP_PANEL_HEIGHT 80
#define BOT_PANEL_HEIGHT 80
#define TOP_PANEL_LINE_HEIGHT 5
#define BOT_PANEL_LINE_HEIGHT TOP_PANEL_LINE_HEIGHT

//Used to determine the location of the scores
#define SCORE_MSG_X_OFFSET SCREEN_WIDTH/2 - 80
#define SCORE_MSG_Y_OFFSET 20

//Font File and Size
#define FONT_FILE "src/lgs.ttf"
#define BIG_FONT_SIZE 60
#define SMALL_FONT_SIZE 30


/*~~EXTERNS~~*/
extern SDL_Surface *screen;


/*~~FUNCTIONS~~*/
//Applies source to dest with an offset of (x,y)
//Function exists here because all graphical components use it.
void applySurface(int x, int y, SDL_Surface *src, SDL_Surface *dest);


#endif
