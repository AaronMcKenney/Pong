#include "SDL/SDL.h"
#include "../include/board.h"

void applySurface(int x, int y, SDL_Surface *src, SDL_Surface *dest)
{
    //Temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Get the offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface;
    SDL_BlitSurface(src, NULL, dest, &offset);
}
