#ifndef PADDLE_H
#define PADDLE_H

//Physical stats about the paddles
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 80
#define PADDLE_DIST_FROM_EDGE 15
#define PADDLE_VEL 1000

//Event keys used by players to control the paddles
#define P1_UP (int)SDLK_w
#define P1_DOWN (int)SDLK_s
#define P2_UP (int)SDLK_UP
#define P2_DOWN (int)SDLK_DOWN

class Paddle
{
    private:
    //Keys that the player presses to move the paddle
    int up;
    int down;

    //The hit box of the paddle. Includes (x,y) cooridnates.
    SDL_Rect box;

    //The velocity of the paddle
    int yVel;

    public:
    //Location of paddle depends on player
    Paddle(int player);

    //Change the velocity based on key presses.
    void handleInput(SDL_Event event);

    void move(float time_step);

    //Move the paddle to a logical location.
    void handleCollisions();

    void show();

    //Getter functions
    SDL_Rect getBox();
    int getVel();
};

#endif
