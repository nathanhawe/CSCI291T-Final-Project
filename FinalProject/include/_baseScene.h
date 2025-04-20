#ifndef _BASESCENE_H
#define _BASESCENE_H

#include <_common.h>
#define MAX_HITS 5

class _baseScene
{
    public:
        GLvoid resizeWindow(GLsizei width, GLsizei height)
        {
            GLfloat Ratio = (GLfloat) width / (GLfloat) height;
            glViewport(0, 0, width, height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, Ratio, 0.1, 1000);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Capture new window dimensions after resize
            dim.x = width;
            dim.y = height;
        }


        virtual GLint IniGL() = 0;
        virtual GLvoid renderScene() = 0;
        virtual int winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

        enum {SCENE_START, SCENE_RUNNING, SCENE_COLLISION, SCENE_RECOVERY, SCENE_FAILURE, SCENE_VICTORY, SCENE_EXIT, SCENE_PAUSE, SCENE_TRANSITION} SceneState;
        int currentSceneState=0;


    protected:
        vec2 dim;
        int playerHitCount = 0;

    private:

};

#endif // _BASESCENE_H
