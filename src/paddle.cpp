//SDL headers
#include "SDL/SDL.h"

//Pong headers
#include "../include/board.h"
#include "../include/paddle.h"

Paddle::Paddle(int player)
{
    //set the paddle's info based on which player controls it
    if(player == P1)
    {
        up = P1_UP;
        down = P1_DOWN;

        box.x = PADDLE_DIST_FROM_EDGE;
    }
    else
    {
        up = P2_UP;
        down = P2_DOWN;

        box.x = SCREEN_WIDTH - (PADDLE_WIDTH + PADDLE_DIST_FROM_EDGE);
    }
    box.y = (SCREEN_HEIGHT - (TOP_PANEL_HEIGHT + BOT_PANEL_HEIGHT))/2 
            - PADDLE_HEIGHT/2 + TOP_PANEL_HEIGHT;
    box.w = PADDLE_WIDTH;
    box.h = PADDLE_HEIGHT;

    yVel = 0;
}

void Paddle::handleInput(SDL_Event event)
{
    int key = (int)event.key.keysym.sym;

    //If a key was pressed, adjust the velocity to move the paddle
    if(event.type == SDL_KEYDOWN)
    {
        if(key == up)
            yVel = -PADDLE_VEL;
        else if(key == down)
            yVel = PADDLE_VEL;
    }
    //If a key was released, adjust velocity to make paddle stop moving
    else if(event.type == SDL_KEYUP && (key == up || key == down))
        yVel = 0;
}

void Paddle::move(float time_step)
{
    box.y = box.y + yVel*time_step;
}

void Paddle::handleCollisions()
{
    if(box.y < TOP_PANEL_HEIGHT)
        box.y = TOP_PANEL_HEIGHT;
    else if(box.y + box.h > SCREEN_HEIGHT - BOT_PANEL_HEIGHT)
        box.y = SCREEN_HEIGHT - (BOT_PANEL_HEIGHT + box.h);
}

void Paddle::show()
{
    SDL_FillRect(screen, &box, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

SDL_Rect Paddle::getBox()
{
    return box;
}

int Paddle::getVel()
{
    return yVel;
}
