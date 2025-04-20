#include "_parallax.h"

_parallax::_parallax()
{
    //ctor
    timerIntervalMs = 10;

    xMin = 0.0;
    xMax = 1.0;
    yMin = 0.0;
    yMax = 1.0;

    vertXMin = -2;
    vertXMax = 2;
    vertYMin = -2;
    vertYMax = 2;
    vertZMin = -10;
    vertZMax = -10;

    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
}

_parallax::~_parallax()
{
    //dtor
}

void _parallax::drawBackground(float width, float height)
{
    glColor3f(1.0, 1.0, 1.0);
    backgroundTexture->textureBinder();


    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glRotatef(rotation.z, 0, 0, 1);

    glBegin(GL_POLYGON);

        // Bottom left
        glTexCoord2f(xMin, yMax);
        glVertex3f(vertXMin, vertYMin, vertZMax);

        // Bottom right
        glTexCoord2f(xMax, yMax);
        glVertex3f(vertXMax, vertYMin, vertZMax);

        // Top Right
        glTexCoord2f(xMax, yMin);
        glVertex3f(vertXMax, vertYMax, vertZMin);

        // Top Left
        glTexCoord2f(xMin, yMin);
        glVertex3f(vertXMin, vertYMax, vertZMin);

    glEnd();

}

void _parallax::initParallax(char* filename)
{
    backgroundTexture->loadImage(filename);
}

void _parallax::scrollParallax(int direction, float speed)
{
    if(timer->getTicks() < timerIntervalMs) return;

    if(direction == LEFT)
    {
        xMin -= speed;
        xMax -= speed;
    }

    if(direction == RIGHT)
    {
        xMin += speed;
        xMax += speed;
    }

    if(direction == UP)
    {
        yMin -= speed;
        yMax -= speed;
    }

    if(direction == DOWN)
    {
        yMin += speed;
        yMax += speed;
    }

    timer->reset();
}
