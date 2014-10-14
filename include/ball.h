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
    //initialize the ball. The higher the number of scores, the faster it gets.
    Ball(int num_scores);

    void move(float time_step);

    void increaseSpeed();

    //move the ball so that it isn't inside any objects.
	void handleCollisions(Paddle pad1, Paddle pad2);

    void show();

    //getters
    SDL_Rect getBox();
}; 

#endif
