#include "_timer.h"

_timer::_timer()
{
    //ctor
    isPaused = false;
    startTime = clock();
}

_timer::~_timer()
{
    //dtor
}
clock_t _timer::getTicks()
{
    if(isPaused)
        return totalTimeAtPause;

    return clock()-startTime;
}

void _timer::reset()
{
    isPaused = false;
    startTime = clock();
}

void _timer::pauseTime()
{
    if(!isPaused)
    {
        totalTimeAtPause = this->getTicks();
        isPaused = true;
        cout << "Timer paused with ticks: " << totalTimeAtPause << endl;
    }
}

void _timer::resumeTime()
{
    if(isPaused)
    {
        startTime = clock() - totalTimeAtPause;
        isPaused = false;
        cout << "Timer resumed with ticks: " << getTicks() << endl;
    }
}
