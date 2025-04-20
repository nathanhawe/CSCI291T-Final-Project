#ifndef _CAMERA_H
#define _CAMERA_H
#include<_common.h>

class _camera
{
    public:
        _camera();
        virtual ~_camera();

        vec3 eye;                       // Camera (eye) position
        vec3 des;                       // Center aka viewed point
        vec3 up;                        // Head direction: which direction is "up" for the camera

        float step;                     // movement
        float distance;                 // distance between eye and des
        vec2 rotAngle;                  // X and Y rotation angles.  Z rotation is uncommon in most games

        void init();                    // Initialize camera
        void setup();                   //
        void reset();                   // Reset camera

        void rotationXY(); // Rotation on X/Y
        void rotationUD(); // Rotation on Up/Down
        void moveForward();
        void moveBack();
        void moveLeft();
        void moveRight();

    protected:

    private:
};

#endif // _CAMERA_H
