#ifndef _SOUNDS_H
#define _SOUNDS_H
#include<_common.h>
#include<SNDS/irrKlang.h>
#include<_timer.h>

using namespace irrklang;

class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();

        ISoundEngine *eng = createIrrKlangDevice();
        ISound* playMusic(char* filename);
        void pauseMusic(ISound *snd);
        void resumeMusic(ISound *snd);
        void playSound(char* filename);
        int initSound();

    protected:

    private:
        _timer *myTimer = new _timer();
};

#endif // _SOUNDS_H
