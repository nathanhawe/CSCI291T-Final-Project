#include "_camera.h"

_camera::_camera()
{
    //ctor
}

_camera::~_camera()
{
    //dtor
}

void _camera::init()
{
    rotAngle.x = 0.0;
    rotAngle.y = 0.0;

    eye.x = 0.0;
    eye.y = 0.0;
    eye.z = 0.0;

    des.x = 0.0;
    des.y = 0.0;
    des.z = 0.0;

    up.x = 0;
    up.y = 1;
    up.z = 0;

    step = 0.5;

    distance = sqrt(
                    pow(des.x - eye.x, 2)
                    + pow(des.y - eye.y, 2)
                    + pow(des.z - eye.z, 2)
                );
}

void _camera::setup()
{
    gluLookAt(
        eye.x, eye.y, eye.z,
        des.x, des.y, des.z,
        up.x, up.y, up.z);
}

void _camera::reset()
{
    this->init();
}

void _camera::rotationXY()
{
    // fine rotation

    des.x = eye.x + distance * cos(rotAngle.y * PI / 180.0) * sin(rotAngle.x * PI/180.0);
    des.y = eye.y + distance * sin(rotAngle.y * PI / 180.0);
    des.z = eye.z + distance * cos(rotAngle.y * PI / 180.0) * cos(rotAngle.x * PI/180.0);
}

// rotations around the z- axis (change y and z?)
void _camera::rotationUD()
{

}

void _camera::moveForward()
{
    float dX, dZ, dY;

    dX = cos(rotAngle.y * PI / 180.0) * sin(rotAngle.x * PI/180.0) * step;
    dY = sin(rotAngle.y * PI / 180.0) * step;
    dZ = cos(rotAngle.y * PI / 180.0) * cos(rotAngle.x * PI/180.0) * step;

    eye.x += dX;
    des.x += dX;
    eye.y += dY;
    des.y += dY;
    eye.z += dZ;
    des.z += dZ;
}

void _camera::moveBack()
{
    float dX, dZ, dY;

    dX = cos(rotAngle.y * PI / 180.0) * sin(rotAngle.x * PI/180.0) * step;
    dY = sin(rotAngle.y * PI / 180.0) * step;
    dZ = cos(rotAngle.y * PI / 180.0) * cos(rotAngle.x * PI/180.0) * step;

    eye.x -= dX;
    des.x -= dX;
    eye.y -= dY;
    des.y -= dY;
    eye.z -= dZ;
    des.z -= dZ;
}

void _camera::moveLeft()
{
    float dX, dZ;
    dZ = cos((rotAngle.x * PI / 180.0) + (PI / 2.0) ) * step;
    dX = sin((rotAngle.x * PI / 180.0) + (PI / 2.0) ) * step;
    eye.x += dX;
    des.x += dX;
    eye.z += dZ;
    des.z += dZ;

}

void _camera::moveRight()
{
    float dX, dZ;
    dZ = cos((rotAngle.x * PI / 180.0) - (PI / 2.0) ) * step;
    dX = sin((rotAngle.x * PI / 180.0) - (PI / 2.0) ) * step;
    eye.x += dX;
    des.x += dX;
    eye.z += dZ;
    des.z += dZ;
}

