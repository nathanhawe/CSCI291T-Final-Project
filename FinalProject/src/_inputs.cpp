#include "_inputs.h"

_inputs::_inputs()
{
    //ctor

    mouse_translate=false;
    mouse_rotate = false;
    isFirstMouseMove = true;
}

_inputs::~_inputs()
{
    //dtor
}
void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
        case VK_LEFT:
              mdl->rot.y +=1.0;
             break;
        case VK_RIGHT:
              mdl->rot.y -=1.0;
             break;
        case VK_UP:
              mdl->rot.x +=1.0;
             break;
        case VK_DOWN:
              mdl->rot.x -=1.0;
             break;
    }

}

void _inputs::keyPressed(_parallax* prlx)
{
    float speed = 0.01;
    switch(wParam)
    {
        case VK_LEFT:
            prlx->xMin -= speed;
            prlx->xMax -= speed;
            break;

        case VK_RIGHT:
            prlx->xMin += speed;
            prlx->xMax += speed;
            break;


        case VK_UP:
            prlx->yMin -= speed;
            prlx->yMax -= speed;
            break;

        case VK_DOWN:
            prlx->yMin += speed;
            prlx->yMax += speed;
            break;
    }

}
void _inputs::keyPressed(_skyBox* sky)
{
     switch(wParam)
    {
        case VK_LEFT:
              sky->rot.y -=1.0;

             break;
        case VK_RIGHT:
               sky->rot.y +=1.0;
             break;
        case VK_UP:

               sky->rot.x +=1.0;
             break;
        case VK_DOWN:
               sky->rot.x -=1.0;
             break;

        case VK_F2:
            sky->pos.z +=1.0;
            break;

        case VK_F3:
            sky->pos.z -=1.0;
            break;
    }

}
void _inputs::keyPressed(_2DPlyer* ply)
{
     switch(wParam)
    {
        case VK_LEFT:
              ply->actiontrigger=ply->WALKLEFT;

             break;
        case VK_RIGHT:
              ply->actiontrigger=ply->WALKRIGHT;
             break;
        case VK_UP:
              ply->actiontrigger=ply->WALKFRONT;

             break;
        case VK_DOWN:
              ply->actiontrigger=ply->WALKBACK;
             break;
    }
}

void _inputs::keyPressed(_3dmodelloader* ply, _3dmodelloader* W )
{
     switch(wParam)
    {
        case VK_LEFT:
            break;

        case VK_RIGHT:
            break;

        case VK_UP:

            break;

        case VK_DOWN:

            break;

        default:

            break;
    }
    cout << "Player Position: (" << ply->pos.x << ", " << ply->pos.y << ", " << ply->pos.z << ")" << endl;
}





void _inputs::keyPressed(_camera* camera)
{
    //cout << "Camera Key: " << wParam << endl;
    switch(wParam)
    {
        case 0x57:  // Forward (W)
            camera->moveForward();
            break;

        case 0x53:  // Back (S)
            camera->moveBack();
            break;

        case 0x41:  // Left (A)
            camera->moveLeft();
            break;

        case 0x44:  // Right (D)
            camera->moveRight();
            break;

        case VK_SPACE:
            camera->reset();
    }
}



void _inputs::keyUp()
{
   switch(wParam)
   {
       default:
           cout<< "came here";
            break;
   }
}

void _inputs::mouseEventDown(_model* mdl, double x, double y)
{
       switch(wParam)
       {
       case MK_LBUTTON:
              mouse_rotate = true;
             break;
       case MK_RBUTTON:
               mouse_translate =true;
             break;
       case MK_MBUTTON:
             break;
       }
       prev_mouse.x = x;
       prev_mouse.y = y;
}

void _inputs::mouseEventUp()
{
     mouse_rotate = false;
     mouse_translate = false;
}

void _inputs::mouseWheel(_model* mdl, double delta)
{
     mdl->pos.z +=delta/100.0;
}

void _inputs::mouseMove(_skyBox* mdl, double x, double y)
{
    if(mouse_rotate)
    {
        mdl->rot.y += (x-prev_mouse.x)/3.0;
        mdl->rot.x += (y-prev_mouse.y)/3.0;
    }
    if(mouse_translate)
    {
       mdl->pos.x += (x-prev_mouse.x)/100.0;
       mdl->pos.y -= (y-prev_mouse.y)/100.0;
    }
    prev_mouse.x =x;
    prev_mouse.y =y;
}

void _inputs::mouseMove(_camera *cam, double x, double y)
{
    if(isFirstMouseMove)
    {
        isFirstMouseMove = false;
        prev_mouse.x = x;
        prev_mouse.y = y;
    }

    // Measure change in x and y
    double deltaX = x - prev_mouse.x;
    double deltaY = y - prev_mouse.y;
    double speedFactor = 0.5;

    cam->rotAngle.x -= deltaX * speedFactor;
    cam->rotAngle.y -= deltaY * speedFactor;

    // constrain the allowed angle of rotations on the y-axis to try and avoid
    // gimble lock or flipping the y values when looking too far up.
    if (cam->rotAngle.y < -80)
        cam->rotAngle.y = -80;
    else if (cam->rotAngle.y > 80)
        cam->rotAngle.y = 80;


    cam->rotationXY();

    prev_mouse.x = x;
    prev_mouse.y = y;
}

