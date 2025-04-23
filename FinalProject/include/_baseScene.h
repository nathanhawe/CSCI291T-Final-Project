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

        GLvoid mouseMapping(int x, int y)
        {
            // dimension.x and dimension.y will give screen width and height
            GLint viewPort[4];              // to store view
            GLdouble modelViewMatrix[16];   // to store model view (camera times the model)
            GLdouble projectionMatrix[16];  // to store projection
            GLfloat winX, winY, winZ;       // to get dimensions

            glGetIntegerv(GL_VIEWPORT, viewPort);
            glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
            glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

            winX = (GLfloat)x;
            winY = (GLfloat) (viewPort[3] - y);
            glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

            gluUnProject(winX, winY, winZ, modelViewMatrix, projectionMatrix, viewPort, &mouseX, &mouseY, &mouseZ);

            //cout << "Mouse Mappings (x,y,z): " << mouseX << ", " << mouseY << ", " << mouseZ << endl;
        }

        void debug()
        {
            cout << "Wave Size: " << waveSize << ", Spawned: " << totalEnemiesSpawned << ", DMG: " << playerHitCount << ", Score: " << enemiesDefeatedCount << endl;
        }


        virtual GLint IniGL() = 0;
        virtual GLvoid renderScene() = 0;
        virtual int winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

        enum {SCENE_START, SCENE_RUNNING, SCENE_COLLISION, SCENE_RECOVERY, SCENE_FAILURE, SCENE_VICTORY, SCENE_EXIT, SCENE_PAUSE, SCENE_TRANSITION} SceneState;
        int currentSceneState=0;

        GLdouble mouseX, mouseY, mouseZ;

    protected:
        vec2 dim;
        int playerHitCount = 0;
        int enemiesDefeatedCount = 0;
        int waveSize;
        int totalEnemiesSpawned;


    private:

};

#endif // _BASESCENE_H
