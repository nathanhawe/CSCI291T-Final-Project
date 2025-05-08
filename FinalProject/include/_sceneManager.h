#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include<_common.h>
#include<_sceneLanding.h>
#include<_sceneA.h>
#include<_sceneB.h>
#include<_sceneC.h>

class _sceneManager
{
    public:
        _sceneManager();
        virtual ~_sceneManager();

        GLint init();

        GLvoid renderActiveScene();

        GLvoid resizeWindow(
             GLsizei width,
             GLsizei height);

        int winMsg(
            HWND	hWnd,
            UINT	uMsg,
			WPARAM wParam,
			LPARAM lParam
        );

        bool exit = false;

    protected:

    private:

        void switchActiveScene();

        bool hasLoadedGameScene[3] = { false, false, false };

        _baseScene *activeScene;
        vec2 dimension;

        // Available scenes
        _sceneLanding *sceneLanding;
        _sceneA *sceneA;
        _sceneB *sceneB;
        _sceneC *sceneC;





};

#endif // _SCENEMANAGER_H
