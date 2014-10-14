//Standard C++ headers
#include <string>

//SDL headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

//Pong headers
#include "../include/board.h"
#include "../include/timer.h"
#include "../include/paddle.h"
#include "../include/ball.h"
#include "../include/text.h"

//namespaces
using namespace std;

//The paddles
Paddle pad1 = Paddle(P1);
Paddle pad2 = Paddle(P2);

//The ball
Ball ball = Ball(0);

//Used for regulating the frame rate
Timer fps = Timer();

//The event structure that stores event data
SDL_Event event;

//If true, exit the game
bool quit = false;
//If true, play another game
bool replay = false;

//The players score, and the winner of the game.
int p1_score = 0;
int p2_score = 0;
int winner = -1;

//The surface (an extern variable in board.h)
SDL_Surface *screen = NULL;

//The font and font color for the messages
TTF_Font *small_font = NULL;
TTF_Font *big_font = NULL;
SDL_Color fontColor = {0xFF, 0xFF, 0xFF};

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

    //Initialize SDL_TTF, and load the fonts
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_INIT failed\n");
        return false;
    }
    if((small_font = TTF_OpenFont(FONT_FILE, SMALL_FONT_SIZE)) == NULL)
    {
        fprintf(stderr, "TTF_OpenFont failed for small font size\n");
        return false;
    }
    if((big_font = TTF_OpenFont(FONT_FILE, BIG_FONT_SIZE)) == NULL)
    {
        fprintf(stderr, "TTF_OpenFont failed for big font size\n");
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
    TTF_CloseFont(small_font);
    TTF_CloseFont(big_font);

    //Always cleans up the screen's surface for you
    SDL_Quit();
}

//Show the top panel (has the players' scores)
void ShowTopPanel(int p1_score, int p2_score)
{
    SDL_Surface *p1_score_msg = NULL;
    SDL_Surface *p2_score_msg = NULL;
    SDL_Rect top_line;
    
    //Convert the scores to C strings
    char p1_score_str[MAX_SCORE / 10 + 2];
    char p2_score_str[MAX_SCORE / 10 + 2];
    sprintf(p1_score_str, "%d", p1_score);
    sprintf(p2_score_str, "%d", p2_score);

    //Begin by showing the top line
    top_line.x = 0;
    top_line.y = TOP_PANEL_HEIGHT - TOP_PANEL_LINE_HEIGHT;
    top_line.w = SCREEN_WIDTH;
    top_line.h = TOP_PANEL_LINE_HEIGHT;
    SDL_FillRect(screen, &top_line, 
                 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

    //Show P1's score
    p1_score_msg = TTF_RenderText_Solid(big_font, p1_score_str, fontColor);
    applySurface(SCORE_MSG_X_OFFSET, SCORE_MSG_Y_OFFSET, p1_score_msg, screen);

    //Show P2's score
    p2_score_msg = TTF_RenderText_Solid(big_font, p2_score_str, fontColor);
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

    SDL_FillRect(screen, &mid_line, 
                 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

//Show the bottom panel
void ShowBotPanel()
{
    SDL_Rect bot_line;
    bot_line.x = 0;
    bot_line.y = SCREEN_HEIGHT - BOT_PANEL_HEIGHT;
    bot_line.w = SCREEN_WIDTH;
    bot_line.h = BOT_PANEL_LINE_HEIGHT;

    SDL_FillRect(screen, &bot_line, 
                 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

//Sees if anyone has scored a point and sets scores accordingly.
//returns true if someone scored a point, and false otherwise.
bool updateScores(SDL_Rect box, int *p1_score, int *p2_score)
{
    //See if someone scored a point first, and act accordingly
    if(box.x > SCREEN_WIDTH)
    {
        ++(*p1_score);
        return true;
    }
    else if(box.x + box.w < 0)
    {
        ++(*p2_score);
        return true;
    }

    return false;   
}

//shows how to play messages, credits, and the title
void showWelcomeMsgs(bool p1_hit_up, bool p1_hit_down, 
                     bool p2_hit_up, bool p2_hit_down)
{
    char pong_msg[] = "PONG";
    int pong_x = SCREEN_WIDTH/2;
    int pong_y = SCREEN_HEIGHT/2;
    
    char credit_msg[] = "Recreated by Aaron McKenney";
    int credit_x = SCREEN_WIDTH/2;
    int credit_y = SCREEN_HEIGHT - BOT_PANEL_HEIGHT/2;
    
    char p1_up_msg[] = "Press 'w' to go up";
    int p1_up_x = SCREEN_WIDTH/4;
    int p1_up_y = TOP_PANEL_HEIGHT + 20;
    
    char p1_down_msg[] = "Press 's' to go down";
    int p1_down_x = SCREEN_WIDTH/4;
    int p1_down_y = (SCREEN_HEIGHT - BOT_PANEL_HEIGHT) - 20;
    
    char p2_up_msg[] = "Press 'up' to go up";
    int p2_up_x = (3*SCREEN_WIDTH)/4;
    int p2_up_y = TOP_PANEL_HEIGHT + 20;
    
    char p2_down_msg[] = "Press 'down' to go down";
    int p2_down_x = (3*SCREEN_WIDTH)/4;
    int p2_down_y = (SCREEN_HEIGHT - BOT_PANEL_HEIGHT) - 20;
    
    showTextBox(pong_msg, pong_x, pong_y, BIG);
    showTextBox(credit_msg, credit_x, credit_y, SMALL);
    if(!p1_hit_up)
        showTextBox(p1_up_msg, p1_up_x, p1_up_y, SMALL);
    if(!p1_hit_down)
        showTextBox(p1_down_msg, p1_down_x, p1_down_y, SMALL);
    if(!p2_hit_up)
        showTextBox(p2_up_msg, p2_up_x, p2_up_y, SMALL);
    if(!p2_hit_down)
        showTextBox(p2_down_msg, p2_down_x, p2_down_y, SMALL);
}

//Shows messages indicating if the players are ready to play a game
void showReadyMsgs(bool p1_ready, bool p2_ready)
{
    char ready_msg[] = "Ready!";
    int ready_1x = SCREEN_WIDTH/6;
    int ready_2x = (5*SCREEN_WIDTH)/6;
    int ready_y = TOP_PANEL_HEIGHT/2;

    if(p1_ready)
        showTextBox(ready_msg, ready_1x, ready_y, SMALL);
    if(p2_ready)
        showTextBox(ready_msg, ready_2x, ready_y, SMALL);
}

//Show who won and how to replay.
void showGameOverMsgs(int winner)
{
    char win_msg[] = "Player x Wins!";
    win_msg[7] = '0' + winner;
    int win_x = SCREEN_WIDTH/2;
    int win_y = SCREEN_HEIGHT/2;
    
    char replay_msg[] = "Press both move keys to replay";
    int replay_x = SCREEN_WIDTH/2;
    int replay_y = SCREEN_HEIGHT - BOT_PANEL_HEIGHT/2;
    
    showTextBox(win_msg, win_x, win_y, BIG);
    showTextBox(replay_msg, replay_x, replay_y, SMALL);
}

//Show the board. Board consists of all graphical components
//needed to play the game. Message boxes are excluded.
void showBoard()
{
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
}

//Welcome message and how to play is printed here
void welcomeLoop()
{
    bool p1_hit_up = false, p1_hit_down = false, p1_ready = false;
    bool p2_hit_up = false, p2_hit_down = false, p2_ready = false;
    bool play = false;
    Uint8 *keystates;
    
    while(!quit && !play)
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
        
        //Get the keystates. If a player presses both of their movement keys,
        //they are ready for the next game.
        keystates = SDL_GetKeyState(NULL);
        if(keystates[P1_UP])
            p1_hit_up = true;
        if(keystates[P1_DOWN])
            p1_hit_down = true;
        if(keystates[P2_UP])
            p2_hit_up = true;
        if(keystates[P2_DOWN])
            p2_hit_down = true;
        
        if(p1_hit_up && p1_hit_down)
            p1_ready = true;
        if(p2_hit_up && p2_hit_down)
            p2_ready = true;
        
        //replay the game if both players are ready and if we aren't quitting
        if(p1_ready && p2_ready && !quit)
            play = true;
            
        //Move the paddles
        pad1.move(DT);
        pad2.move(DT);

        //Handle any collisions that the paddles could have
        pad1.handleCollisions();
        pad2.handleCollisions();

        //Show the board
        showBoard();
        //Show the welcome message, how to play messages, and ready messages
        showWelcomeMsgs(p1_hit_up, p1_hit_down, p2_hit_up, p2_hit_down);
        showReadyMsgs(p1_ready, p2_ready);

        //Update the screen. SDL docs don't say how SDL_Flip can fail, only 
        //that it can. I don't think that it's worthy of shutting down the 
        //game if it does fail.
        if(SDL_Flip(screen) == -1)
            fprintf(stderr, "Failed to update the screen\n");
            
        //Cap the frame rate at 1000/MAX_FRAMES_PER_SEC milliseconds
        if(fps.getTicks() < 1000*DT)
            SDL_Delay((1000*DT) - fps.getTicks());
    }
}

//Countdown timer is here. Occurs right before the game begins
void countdownLoop()
{
    char countdown[] = "3";
    int countdown_x = SCREEN_WIDTH/2;
    int countdown_y = SCREEN_HEIGHT/2;
    Timer timeInLoop = Timer();
    
    timeInLoop.start();
    while(!quit && countdown[0] > '0')
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

        //Move the paddles
        pad1.move(DT);
        pad2.move(DT);

        //Handle any collisions that the paddles could have
        pad1.handleCollisions();
        pad2.handleCollisions();

        //Show the board
        showBoard();
        //show the timer and ready messages
        showTextBox(countdown, countdown_x, countdown_y, BIG);
        showReadyMsgs(true, true);
        
        //Update the counter
        countdown[0] = '3' - timeInLoop.getTicks()/1000;

        //Update the screen. SDL docs don't say how SDL_Flip can fail, only 
        //that it can. I don't think that it's worthy of shutting down the 
        //game if it does fail.
        if(SDL_Flip(screen) == -1)
            fprintf(stderr, "Failed to update the screen\n");

        //Cap the frame rate at 1000/MAX_FRAMES_PER_SEC milliseconds
        if(fps.getTicks() < 1000*DT)
            SDL_Delay((1000*DT) - fps.getTicks());
    }
}

//Pong is played here
void gameLoop()
{
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
        ball.handleCollisions(pad1, pad2);
        
        //Update the player's scores.
        //If they had to be updated, make a new ball.
        if(updateScores(ball.getBox(), &p1_score, &p2_score))
            ball = Ball(p1_score + p2_score);

        //Show the board
        showBoard();

        //Update the screen. SDL docs don't say how SDL_Flip can fail, only 
        //that it can. I don't think that it's worthy of shutting down the 
        //game if it does fail.
        if(SDL_Flip(screen) == -1)
            fprintf(stderr, "Failed to update the screen\n");

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
}

//If someone wins the game, loop here to display the win message.
void gameOverLoop()
{
    bool p1_ready = false;
    bool p2_ready = false;
    Uint8 *keystates;
    
    while(!quit && !replay)
    {
        //Handle Events
        while(SDL_PollEvent(&event))
        {
            //Quit if the user closes the window
            if(event.type == SDL_QUIT)
                quit = true;
        }
        
        //Get the keystates. If a player presses both of their movement keys,
        //they are ready for the next game.
        keystates = SDL_GetKeyState(NULL);
        if(keystates[P1_UP] && keystates[P1_DOWN])
            p1_ready = true;
        if(keystates[P2_UP] && keystates[P2_DOWN])
            p2_ready = true;
        
        //replay the game if both players are ready and if we aren't quitting
        if(p1_ready && p2_ready && !quit)
            replay = true;

        //Show the board
        showBoard();
        //Show the win message, replay message, and ready messages
        showGameOverMsgs(winner);
        showReadyMsgs(p1_ready, p2_ready);

        //Update the screen. SDL docs don't say how SDL_Flip can fail, only 
        //that it can. I don't think that it's worthy of shutting down the 
        //game if it does fail.
        if(SDL_Flip(screen) == -1)
            fprintf(stderr, "Failed to update the screen\n");
    }
}

void resetGame()
{
    //reset the scores
    p1_score = 0;
    p2_score = 0;
    winner = -1;
    
    //Reinitialize the ball.
    ball = Ball(0);
            
    replay = false;
}

int main()
{
    if(!SDLInit())
        return 1;

    //welcome the players to pong
    welcomeLoop();

    do {
        //If the players already played a game and want to play another,
        //reset the game to its initial state
        if(replay)
            resetGame();

        //Start a countdown so that both players will be ready
        countdownLoop();
        
        gameLoop();

        //If the game ended naturally, say who won.
        if(winner == P1 || winner == P2)
            gameOverLoop();
    } while(!quit && replay);

    SDLQuit();
    return 0;
}
