//SDL headers
#include "SDL/SDL.h"

//Pong headers
#include "../include/board.h"

//This piece of code was originally from Lazy Foo' Productions
//[http://lazyfoo.net/]
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
