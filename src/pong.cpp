//Standard C++ headers
#include <string>
#include <sstream>

//Operating Systems headers

//SDL headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

//Pong headers
#include "../include/board.h"
#include "../include/timer.h"
#include "../include/paddle.h"
#include "../include/ball.h"

//The surface (an extern variable in board.h)
SDL_Surface *screen = NULL;

//The surfaces for keeping score
SDL_Surface *p1_score_msg = NULL;
SDL_Surface *p2_score_msg = NULL;

//The surface for displaying the win message
SDL_Surface *win_msg = NULL;

//The event structure that stores event data
SDL_Event event;

//The font and font color for the score messages
TTF_Font *font = NULL;
SDL_Color fontColor = {0xFF, 0xFF, 0xFF};

SDL_Surface *loadImage(std::string filename);

using namespace std;

//initializes SDL functionality
bool SDLInit()
{
    //Initialize all SDL subsystems
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        fprintf(stderr, "SDL_INIT failed\n");
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 
                              SCREEN_BPP, SDL_SWSURFACE);
    if(screen == NULL)
    {
        fprintf(stderr, "SDL_SetVideoMode failed\n");
        return false;
    }

    //Initialize SDL_TTF, and load the score font
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_INIT failed\n");
        return false;
    }
    if((font = TTF_OpenFont(FONT_FILE, FONT_SIZE)) == NULL)
    {
        fprintf(stderr, "TTF_OpenFont failed\n");
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption("Pong", NULL);

    return true;
}

//Cleans up SDL functionality
void SDLQuit()
{
    //Close the font
    TTF_CloseFont(font);

    //Always cleans up the screen's surface for you
    SDL_Quit();
}

//Show the top panel (has the players' names and scores)
void ShowTopPanel(int p1_score, int p2_score)
{
    SDL_Rect top_line;
    stringstream p1_score_str;
    stringstream p2_score_str;

    //Begin by showing the top line
    top_line.x = 0;
    top_line.y = TOP_PANEL_HEIGHT - TOP_PANEL_LINE_HEIGHT;
    top_line.w = SCREEN_WIDTH;
    top_line.h = TOP_PANEL_LINE_HEIGHT;
    SDL_FillRect(screen, &top_line, 
                 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

    //Show P1's score
    p1_score_str << p1_score;
    p1_score_msg = TTF_RenderText_Solid(font, p1_score_str.str().c_str(), 
                                        fontColor);
    applySurface(SCORE_MSG_X_OFFSET, SCORE_MSG_Y_OFFSET, p1_score_msg, screen);

    //Show P2's score
    p2_score_str << p2_score;
    p2_score_msg = TTF_RenderText_Solid(font, p2_score_str.str().c_str(), 
                                        fontColor);
    applySurface(SCREEN_WIDTH - (p2_score_msg->clip_rect.w + SCORE_MSG_X_OFFSET), 
                 SCORE_MSG_Y_OFFSET, p2_score_msg, screen);
}

//Show the middle line
void ShowMidLine()
{
    SDL_Rect mid_line;
    mid_line.x = SCREEN_WIDTH/2;
    mid_line.y = 0;
    mid_line.w = 1;
    mid_line.h = SCREEN_HEIGHT;

    SDL_FillRect(screen, &mid_line, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

//Show the bottom panel
void ShowBotPanel()
{
    SDL_Rect bot_line;
    bot_line.x = 0;
    bot_line.y = SCREEN_HEIGHT - BOT_PANEL_HEIGHT;
    bot_line.w = SCREEN_WIDTH;
    bot_line.h = BOT_PANEL_LINE_HEIGHT;

    SDL_FillRect(screen, &bot_line, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

//Sees if anyone has scored a point, sets scores accordingly.
//returns true if someone scored a point, and false otherwise.
bool updateScores(SDL_Rect box, int *p1_score, int *p2_score)
{
    //See if someone scored a point first, and act accordingly
    if(box.x + box.w > SCREEN_WIDTH)
    {
        ++(*p1_score);
        return true;
    }
    else if(box.x < 0)
    {
        ++(*p2_score);
        return true;
    }

    return false;   
}

//Show who won!
void showWinMsg(int winner)
{
    stringstream win_msg_str;
    SDL_Rect white_outline;
    SDL_Rect black_box;

    win_msg_str << "Player ";
    win_msg_str << winner;
    win_msg_str << " Wins!";

    //Create the win message
    win_msg = TTF_RenderText_Solid(font, win_msg_str.str().c_str(), fontColor);
    //Set the win message's position to the middle of the creen.
    win_msg->clip_rect.x = SCREEN_WIDTH/2 - win_msg->clip_rect.w/2;
    win_msg->clip_rect.y = SCREEN_HEIGHT/2 - win_msg->clip_rect.h/2;
    //Make a black box with a white outline for the win message.
    white_outline = win_msg->clip_rect;
    white_outline.x = white_outline.x - (OUTLINE_SIZE + OUTLINE_X_SPACE);
    white_outline.y = white_outline.y - (OUTLINE_SIZE + OUTLINE_Y_SPACE);
    white_outline.w = white_outline.w + 2*(OUTLINE_SIZE + OUTLINE_X_SPACE);
    white_outline.h = white_outline.h + 2*(OUTLINE_SIZE + OUTLINE_Y_SPACE);
    SDL_FillRect(screen, &white_outline, 
                 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    black_box = win_msg->clip_rect;
    black_box.x = black_box.x - OUTLINE_X_SPACE;
    black_box.w = black_box.w + 2*OUTLINE_X_SPACE;
    SDL_FillRect(screen, &black_box, SDL_MapRGB(screen->format, 0, 0, 0));
    //Finally, show the win message
    applySurface(win_msg->clip_rect.x, win_msg->clip_rect.y, win_msg, screen);
}

int main()
{
    bool quit = false;

    int p1_score = 0;
    int p2_score = 0;
    int winner = -1;

    //Seed the random number generator
    srand(time(NULL));

    //The paddles
    Paddle pad1 = Paddle(P1);
    Paddle pad2 = Paddle(P2);

    //The ball
    Ball ball = Ball(0);
    //(ix, iy) is the point of impact that the ball has had with an object
    //float ix = 0, iy = 0;
    //What the ball has collided with during the time step
    //int ball_coll;

    //Used for regulating the frame rate
    Timer fps = Timer();

    if(!SDLInit())
        return 1;

    //GAME LOOP
    while(!quit)
    {
        //Start the frame rate regulator
        fps.start();

        //Handle Events
        while(SDL_PollEvent(&event))
        {
            //Adjust the paddles' velocity here
            pad1.handleInput(event);
            pad2.handleInput(event);

            //Quit if the user closes the window
            if(event.type == SDL_QUIT)
                quit = true;
        }

        //Move the paddles and ball
        pad1.move(DT);
        pad2.move(DT);
        ball.move(DT);

        //Handle any collisions that the paddles could have
        pad1.handleCollisions();
        pad2.handleCollisions();

        //Detect and handle any collisions that the ball has.
        /*
        ball_coll = ball.detectCollision(pad1.getBox(), pad2.getBox(), &ix, &iy);
        if(ball_coll != HIT_NOTHING && ball_coll != HIT_ERROR)
            ball.handleCollisions(pad1.getBox(), pad2.getBox(), ball_coll, ix, iy);
        */
        ball.handleCollisions(pad1, pad2);
        
        if(updateScores(ball.getBox(), &p1_score, &p2_score))
            ball = Ball(p1_score + p2_score);

        //First blackout the screen to remove previous information
        SDL_FillRect(screen, &screen->clip_rect, 
                     SDL_MapRGB(screen->format, 0, 0, 0));
        //Display the panels that exist outside the gaming area.
        ShowTopPanel(p1_score, p2_score);
        ShowMidLine();
        ShowBotPanel();
        //Display our objects
        pad1.show();
        pad2.show();
        ball.show();

        //Update the screen
        if(SDL_Flip(screen) == -1)
        {
            fprintf(stderr, "Failed to update the screen\n");
            return 1;
        }

        //Cap the frame rate at 1000/MAX_FRAMES_PER_SEC milliseconds
        if(fps.getTicks() < 1000*DT)
            SDL_Delay((1000*DT) - fps.getTicks());

        if(p1_score >= MAX_SCORE)
            winner = P1;
        else if(p2_score >= MAX_SCORE)
            winner = P2;
        if(winner == P1 || winner == P2)
            break;
    }

    if(winner == P1 || winner == P2)
    {
        //GAMEOVER LOOP
        while(!quit)
        {
            //Handle Events
            while(SDL_PollEvent(&event))
            {
                //Adjust the paddles' velocity here
                pad1.handleInput(event);
                pad2.handleInput(event);
    
                //Quit if the user closes the window
                if(event.type == SDL_QUIT)
                    quit = true;
            }

            //Show who won and whatnot
            //First blackout the screen to remove previous information
            SDL_FillRect(screen, &screen->clip_rect, 
                         SDL_MapRGB(screen->format, 0, 0, 0));
            //Display the panels that exist outside the gaming area.
            ShowTopPanel(p1_score, p2_score);
            ShowMidLine();
            ShowBotPanel();
            pad1.show();
            pad2.show();

            showWinMsg(winner);

            //Update the screen
            if(SDL_Flip(screen) == -1)
            {
                fprintf(stderr, "Failed to update the screen\n");
                return 1;
            }
        }
    }

    SDLQuit();
    return 0;
}
