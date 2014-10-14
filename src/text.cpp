//C++ headers
#include <string>

//SDL headers
#include "SDL/SDL_ttf.h"

//Pong headers
#include "../include/board.h"
#include "../include/text.h"

void showTextBox(const char *msg, int x, int y, int size)
{
    SDL_Surface *msg_box = NULL;
    SDL_Rect white_outline;
    SDL_Rect black_box;

    //Create the message box, centered at (x,y)
    if(size == SMALL)
        msg_box = TTF_RenderText_Solid(small_font, msg, fontColor);
    else
        msg_box = TTF_RenderText_Solid(big_font, msg, fontColor);
    msg_box->clip_rect.x = x - msg_box->clip_rect.w/2;;
    msg_box->clip_rect.y = y - msg_box->clip_rect.h/2;
    
    //Make a black box with a white outline for the message.
    //Draw a white box first
    white_outline = msg_box->clip_rect;
    white_outline.x = white_outline.x - (OUTLINE_SIZE + OUTLINE_X_SPACE);
    white_outline.y = white_outline.y - (OUTLINE_SIZE + OUTLINE_Y_SPACE);
    white_outline.w = white_outline.w + 2*(OUTLINE_SIZE + OUTLINE_X_SPACE);
    white_outline.h = white_outline.h + 2*(OUTLINE_SIZE + OUTLINE_Y_SPACE);
    SDL_FillRect(screen, &white_outline, 
                 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    //Then draw a smaller black box in the center of the white box.
    black_box = msg_box->clip_rect;
    black_box.x = black_box.x - OUTLINE_X_SPACE;
    black_box.w = black_box.w + 2*OUTLINE_X_SPACE;
    SDL_FillRect(screen, &black_box, SDL_MapRGB(screen->format, 0, 0, 0));

    //Finally, show the message on top.
    applySurface(msg_box->clip_rect.x, msg_box->clip_rect.y, msg_box, screen);
}
