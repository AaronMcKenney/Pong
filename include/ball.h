#ifndef BALL_H
#define BALL_H

#include "paddle.h"

#define BALL_WIDTH 10
#define BALL_HEIGHT 10
#define BALL_INIT_VEL 100
#define BALL_VEL_INC_PER_HIT 20
#define BALL_VEL_INC_PER_POINT 100

//Used to determine the x and y velocity from the direction
#define PI 3.141159265

//Rounds direction to nearest whole number, since SDL_Rect has
//integer coordinates
//#define ROUND(x) ((x) >= 0 ? (long)((x)+0.5) : (long)((x)-0.5))

//An enum of things that the ball can hit
/*
enum {HIT_NOTHING, HIT_TOP, HIT_BOT,
        HIT_PAD1, HIT_PAD1_T, HIT_PAD1_B,
        HIT_PAD2, HIT_PAD2_T, HIT_PAD2_B,
        HIT_ERROR};
*/

class Ball
{
    private:
    //The hit box of the paddle. Includes (x,y) coordinates as ints
    SDL_Rect box;

    //The previous position of the ball.
    float xPrevPos;
    float yPrevPos;

    //The actual position of the ball.
    float xPos;
    float yPos;

    //The velocity of the ball in (x,y) and as a magnitude.
    float xVel;
    float yVel;

    //How many times the ball has been hit by a paddle.
    //The ball goes faster with each hit
    //num_hits resets when someone scores a point
    int num_hits;

    public:
    Ball(int num_scores);

    void move(float time_step);

    void increaseSpeed();

    //Checks if the ball hit anything in the last time step that it took.
    //Return value is based on the enum above.
    //In addition, if it hit something, (ix, iy) will be set to the 
    //point of intersection between the ball and the object.
    //int detectCollision(SDL_Rect pad1, SDL_Rect pad2, float *ix, float *iy);

    //If the ball has hit something during its last time step 
    //i.e. firstCollision != HIT_NOTHING
    //Then move the ball around so that it doesn't go through objects.
    //void handleCollisions(SDL_Rect pad1, SDL_Rect pad2, int firstCollision, float first_ix, float first_iy);

	void handleCollisions(Paddle pad1, Paddle pad2);

    void show();

    //getters
    SDL_Rect getBox();
}; 

#endif
