#ifndef _SCENELANDING_H
#define _SCENELANDING_H

#include <_baseScene.h>
#include <_common.h>
#include <_camera.h>
#include <_textureloader.h>
#include <_sounds.h>

#define MUSIC_FILE "sounds/guards_of_war.mp3"

class _sceneLanding : public _baseScene
{
    public:
        _sceneLanding();
        virtual ~_sceneLanding();

        GLint IniGL();                                    //initialization
        GLvoid renderScene();                              // Draw Scene

        int winMsg(
            HWND	hWnd,			// Handle For This Window
            UINT	uMsg,			// Message For This Window
            WPARAM  wParam,			// Additional Message Information
            LPARAM	lParam
        );

    protected:

    private:
        _camera *camera;
        _textureLoader *tex;
        _sounds *snds = new _sounds();
        ISound *backgroundMusic;

        GLuint landingPage, menuPage, helpPage, menuPageNew, menuPageHelp, menuPageExit;

        enum {HOVER_NONE = 0, HOVER_NEW, HOVER_HELP, HOVER_EXIT};
        int mouseHoverState = 0;

};

#endif // _SCENELANDING_H
