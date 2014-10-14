#ifndef TEXT_H
#define TEXT_H

//Used to determine the outline of a message box
#define OUTLINE_SIZE 3
#define OUTLINE_X_SPACE 12
#define OUTLINE_Y_SPACE 0

//Used to determine the size of the font
#define SMALL 0
#define BIG 1

extern TTF_Font *small_font;
extern TTF_Font *big_font;
extern SDL_Color fontColor;

//Show a text box on the board with message msg, where its center is 
//located on the coordinates (x,y)
void showTextBox(const char *msg, int x, int y, int size);

#endif
