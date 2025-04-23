#include "_sounds.h"

_sounds::_sounds()
{
    //ctor
}

_sounds::~_sounds()
{
    //dtor
    eng->drop();
}

ISound* _sounds::playMusic(char* filename)
{
    return eng->play2D(filename, true, false, true);
}

void _sounds::pauseMusic(ISound* snd)
{
    snd->setIsPaused(true);
}

void _sounds::resumeMusic(ISound* snd)
{
    snd->setIsPaused(false);
}

ISoundSource* _sounds::loadSoundSource(char* filename)
{
    return eng->addSoundSourceFromFile(filename);
}

void _sounds::playSound(char* filename)
{
    eng->play2D(filename, false, false);
}

void _sounds::playSoundSource(ISoundSource* soundSource)
{
    eng->play2D(soundSource);
}


int _sounds::initSound()
{
    if (!eng)
    {
        cout << "ERROR: *** The sound engine could not load ***" << endl;
        return 0;
    }
    return 1;
}


