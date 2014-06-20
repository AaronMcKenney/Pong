//This piece of code was originally from Lazy Foo' Productions
//[http://lazyfoo.net/]

#ifndef TIMER_H
#define TIMER_H

class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;
    
    //The ticks stored when the timer was paused
    int pausedTicks;
    
    //The timer status
    bool paused;
    bool started;
    
    public:
    //Initializes variables
    Timer();
    
    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    //Gets the timer's time
    int getTicks();
    
    //Checks the status of the timer
    bool isStarted();
    bool isPaused();    
};

#endif
