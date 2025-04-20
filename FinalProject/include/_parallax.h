#ifndef _PARALLAX_H
#define _PARALLAX_H

#include<_common.h>
#include<_textureLoader.h>
#include<_timer.h>

class _parallax
{
    public:
        _parallax();
        virtual ~_parallax();

        _textureLoader *backgroundTexture = new _textureLoader();

        void drawBackground(
            float width,
            float height);

        void initParallax(char *filename);

        void scrollParallax(
            int direction,
            float speed);

        enum scrollDirection {STOP, UP, DOWN, LEFT, RIGHT};
        int timerIntervalMs;
        float xMin, xMax, yMin, yMax;
        float vertXMin, vertXMax, vertYMin, vertYMax, vertZMin, vertZMax;
        vec3 rotation;
    protected:

    private:
        _timer *timer = new _timer();
};

#endif // _PARALLAX_H
