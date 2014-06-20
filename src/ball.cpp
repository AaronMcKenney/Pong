#include <math.h>

#include <time.h>

#include "SDL/SDL.h"
#include "../include/board.h"
#include "../include/ball.h"

Ball::Ball(int num_scores)
{
    float vel, angle;

    //Put the ball in the middle of the screen
    box.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
    box.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
    box.w = BALL_WIDTH;
    box.h = BALL_HEIGHT;

    //give the ball its actual position as floats

    xPos = SCREEN_WIDTH/2.0 - BALL_WIDTH/2.0;
    yPos = SCREEN_HEIGHT/2.0 - BALL_HEIGHT/2.0;
    xPrevPos = xPos;
    yPrevPos = yPos;

    //The ball has yet to hit anything, 'cause it just got born
    num_hits = 0;

    //Init the velocity, and compute the x and y velocities
    //ball goes faster for every point scored
    vel = BALL_INIT_VEL + num_scores*BALL_VEL_INC_PER_POINT;
    angle = rand()%360;
    if(angle > 80.0 && angle <= 90.0)
        angle = 80.0;
    else if(angle > 90.0 && angle < 110.0)
        angle = 110.0;
    else if(angle > 260.0 && angle <= 270.0)
        angle = 260.0;
    else if(angle > 270.0 && angle <= 280.0)
        angle = 280.0;

    //Assume velocity [vel, 0], and rotate vector by angle
    xVel = (float)vel*cos(angle * PI/180.0);
    yVel = (float)vel*sin(angle * -(PI/180.0));
}

void Ball::move(float time_step)
{
    xPrevPos = xPos;
    yPrevPos = yPos;

    //Movie it using the projectile motion equation: d = d0 + vt + 1/2at^2
    xPos = xPos + xVel*time_step;
    yPos = yPos + yVel*time_step;

    //update the box positions in case checkCollision is called
    box.x = (int)xPos;
    box.y = (int)yPos;
}

void Ball::increaseSpeed()
{
    //Find the unit vector, and scale it to a macro
    float magn = sqrt((xVel*xVel) + (yVel*yVel));
    float xUnitScaled = (xVel/magn) * BALL_VEL_INC_PER_HIT;
    float yUnitScaled = (yVel/magn) * BALL_VEL_INC_PER_HIT;

    xVel += xUnitScaled;
    yVel += yUnitScaled;
}

bool checkCollision(SDL_Rect A, SDL_Rect B)
{
    //The first four conditions depict cases where its impossible
    //for the two rectangles to be inside one another.
    //Conversely, there must be a collision if all four cases are false
    if(A.y >= B.y + B.h)
        return false;
    else if(A.y + A.h <= B.y)
        return false;
    else if(A.x >= B.x + B.w)
        return false;
    else if(A.x + A.w <= B.x)
        return false;
    else
        return true;
}

void Ball::handleCollisions(Paddle pad1, Paddle pad2)
{
    SDL_Rect pad1_box = pad1.getBox();
    SDL_Rect pad2_box = pad2.getBox();

    //See if the ball went past the top.
    if(box.y < TOP_PANEL_HEIGHT)
    {
        box.y = TOP_PANEL_HEIGHT;
        yPos = (float)box.y;
        yVel = -yVel;
    }
    //See if the ball went below the bot.
    else if(box.y + box.h > SCREEN_HEIGHT - BOT_PANEL_HEIGHT)
    {
        box.y = SCREEN_HEIGHT - (BOT_PANEL_HEIGHT + box.h);
        yPos = (float)box.y;
        yVel = -yVel;
    }
    
    //See if the ball hit pad1
    else if(checkCollision(box, pad1_box))
    {
        box.x = pad1_box.x + pad1_box.w;
        xPos = (float)box.x;
        xVel = -xVel;
        yVel += ((float)pad1.getVel())/20.0;
        increaseSpeed();
    }
    //See if the ball hit pad2
    else if(checkCollision(box, pad2_box))
    {
        box.x = pad2_box.x - box.w;
        xPos = (float)box.x;
        xVel = -xVel;
        yVel += ((float)pad2.getVel())/20.0;
        increaseSpeed();
    }
}

void Ball::show()
{
    //update the box positions before showing them.
    box.x = (int)xPos;
    box.y = (int)yPos;

    SDL_FillRect(screen, &box, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

SDL_Rect Ball::getBox()
{
    return box;
}
