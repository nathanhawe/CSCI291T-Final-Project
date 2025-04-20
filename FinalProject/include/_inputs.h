#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_2dplyer.h>
#include <_camera.h>
#include<_3dmodelloader.h>


class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *);
        void keyUp(); // ToDo: address accordingly

        void keyPressed(_parallax *);
        void keyPressed(_skyBox *);
        void keyPressed(_2DPlyer *);
        void keyPressed(_3dmodelloader *,_3dmodelloader * );
        void keyPressed(_camera *);


        void mouseEventDown(_model*, double,double);
        void mouseEventUp();// ToDo: address accordingly

        void mouseWheel(_model*,double);
        void mouseMove(_skyBox*,double,double);
        void mouseMove(_camera *cam, double x, double y);

        vec2 prev_mouse;

        bool mouse_translate; // flag to indicate mouse move
        bool mouse_rotate; //flag to indicate mouse rotation
        bool isFirstMouseMove;

        WPARAM wParam;

    protected:

    private:
};

#endif // _INPUTS_H
