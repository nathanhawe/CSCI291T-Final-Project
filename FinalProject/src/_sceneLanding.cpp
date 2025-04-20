#include "_sceneLanding.h"

_sceneLanding::_sceneLanding()
{
    //ctor
    camera = new _camera();
    tex = new _textureLoader();
}

_sceneLanding::~_sceneLanding()
{
    //dtor
    delete camera;
    delete tex;
    delete snds;
}

GLint _sceneLanding::IniGL()
{
    glClearColor(0.0,0.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Setup camera
    camera->init();
    camera->eye.x = 0;
    camera->eye.y = 0;
    camera->eye.z = 1;


    // Initialize background images
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    landingPage = tex->loadImages("images/landing_page.png");
    helpPage = tex->loadImages("images/help_page.png");
    menuPage = tex->loadImages("images/menu_page.png");
    menuPageNew = tex->loadImages("images/menu_page_new.png");
    menuPageHelp = tex->loadImages("images/menu_page_help.png");
    menuPageExit = tex->loadImages("images/menu_page_exit.png");

    // load background music
    snds->initSound();
    backgroundMusic = snds->playMusic(MUSIC_FILE);
    backgroundMusic->setVolume(0.15f);

    return true;
}


GLvoid _sceneLanding::renderScene()
{

    // reset the color buffer bit (all colors on screen) and depth bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update camera
    camera->setup();

    // Setup background
    float ratX = dim.x / dim.y;
    glColor3f(1.0, 1.0, 1.0);

    switch(currentSceneState)
    {
        case SCENE_START:
            glBindTexture(GL_TEXTURE_2D, landingPage);
            break;

        case SCENE_RUNNING:
        case SCENE_VICTORY:
        case SCENE_EXIT:
            switch(mouseHoverState)
            {
                case HOVER_NEW: glBindTexture(GL_TEXTURE_2D, menuPageNew); break;
                case HOVER_HELP: glBindTexture(GL_TEXTURE_2D, menuPageHelp); break;
                case HOVER_EXIT: glBindTexture(GL_TEXTURE_2D, menuPageExit); break;
                default: glBindTexture(GL_TEXTURE_2D, menuPage); break;
            }

            break;

        case SCENE_RECOVERY:
            glBindTexture(GL_TEXTURE_2D, helpPage);
            break;
        }
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 1); glVertex3f(-1.0 * ratX, -1, -1.25);
        glTexCoord2f(1, 1); glVertex3f(ratX, -1, -1.25);
        glTexCoord2f(1, 0); glVertex3f(ratX, 1, -1.25);
        glTexCoord2f(0, 0); glVertex3f(-1.0 * ratX, 1, -1.25);
    glEnd();
    glEnable(GL_LIGHTING);

}

int _sceneLanding::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int prev = currentSceneState;

    if (prev == SCENE_START)
        snds->resumeMusic(backgroundMusic);

    switch(uMsg)
    {
        case WM_KEYDOWN:
            //cout << "Key Value: " << wParam;
            switch(wParam)
            {
                case 13: // Enter
                    if(currentSceneState == SCENE_START)
                        currentSceneState = SCENE_RUNNING;
                    break;

                case 78: // n
                    if(currentSceneState == SCENE_RUNNING)
                    {
                        currentSceneState = SCENE_TRANSITION;
                        snds->pauseMusic(backgroundMusic);
                    }
                    break;

                case 72: // h
                    if(currentSceneState == SCENE_RUNNING)
                        currentSceneState = SCENE_RECOVERY;
                    break;

                case VK_ESCAPE:
                    if(currentSceneState == SCENE_RUNNING)
                    {
                        currentSceneState = SCENE_EXIT;
                    }
                    else if(currentSceneState == SCENE_RECOVERY)
                    {
                        currentSceneState = SCENE_RUNNING;
                    }
            }
            break;

        case WM_KEYUP: break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            if (currentSceneState == SCENE_START)
            {
                currentSceneState = SCENE_RUNNING;
            }
            else if(currentSceneState == SCENE_RUNNING)
            {
                if(mouseHoverState == HOVER_NEW)
                {
                    currentSceneState = SCENE_TRANSITION;
                    snds->pauseMusic(backgroundMusic);
                }
                else if(mouseHoverState == HOVER_HELP)
                {
                    currentSceneState = SCENE_RECOVERY;
                }
                else if(mouseHoverState == HOVER_EXIT)
                {
                    currentSceneState = SCENE_EXIT;
                }

            }

            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            break;

        case WM_MOUSEMOVE:
            if(currentSceneState == SCENE_RUNNING)
            {


                float posX = LOWORD(lParam) / dim.x;
                float posY = HIWORD(lParam) / dim.y;

                if(
                    posX >= 0.355 && posX <= 0.654 &&
                    posY >= 0.221 && posY <= 0.324)
                {
                    mouseHoverState = HOVER_NEW;
                }
                else if(
                    posX >= 0.392 && posX <= 0.608 &&
                    posY >= 0.435 && posY <= 0.552)
                {
                    mouseHoverState = HOVER_HELP;
                }
                else if(
                    posX >= 0.378 && posX <= 0.624 &&
                    posY >= 0.644 && posY <= 0.766)
                {
                    mouseHoverState = HOVER_EXIT;
                }
                else mouseHoverState = HOVER_NONE;
            }
            else mouseHoverState = HOVER_NONE;
            break;
        case WM_MOUSEWHEEL: break;
    }

    if (prev != currentSceneState)
        cout << "Landing: Switched state " << prev << "->" << currentSceneState << endl;
}
