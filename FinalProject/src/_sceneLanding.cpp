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
    creditsPage = tex->loadImages("images/credits.jpg");
    menuPage = tex->loadImages("images/menu.png");
    //menuPageNew = tex->loadImages("images/menu_page_new.png");
    //menuPageHelp = tex->loadImages("images/menu_page_help.png");
    //menuPageExit = tex->loadImages("images/menu_page_exit.png");

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
            /*
            switch(mouseHoverState)
            {
                case HOVER_NEW: glBindTexture(GL_TEXTURE_2D, menuPageNew); break;
                case HOVER_HELP: glBindTexture(GL_TEXTURE_2D, menuPageHelp); break;
                case HOVER_EXIT: glBindTexture(GL_TEXTURE_2D, menuPageExit); break;
                default: glBindTexture(GL_TEXTURE_2D, menuPage); break;
            }*/
            glBindTexture(GL_TEXTURE_2D, menuPage);  // always bind the same base menu image

            break;

        case SCENE_RECOVERY:
            glBindTexture(GL_TEXTURE_2D, helpPage);
            break;

        case SCENE_CREDITS:
            glBindTexture(GL_TEXTURE_2D, creditsPage);
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


if (currentSceneState == SCENE_RUNNING) {

    glEnable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 0.0f);


    float ratX = dim.x / dim.y;

    glBegin(GL_LINE_LOOP);
    switch(mouseHoverState)
{
    case HOVER_NEW: {
        float left = 0.61f, right = 0.84f;
        float top = 0.44f, bottom = 0.51f;
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        break;
    }

    case HOVER_HELP: {
        float left = 0.66f, right = 0.79f;
        float top = 0.53f, bottom = 0.585f;
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        break;
    }

    case HOVER_CREDITS: {
        float left = 0.65f, right = 0.8f;
        float top = 0.605f, bottom = 0.665f;
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        break;
    }

    case HOVER_EXIT: {
        float left = 0.65f, right = 0.79f;
        float top = 0.68f, bottom = 0.735f;
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(top    * 2.0f - 1.0f), -1.0f);
        glVertex3f((right * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        glVertex3f((left  * 2.0f - 1.0f) * ratX, -(bottom * 2.0f - 1.0f), -1.0f);
        break;
    }


    default: break;
}

    glEnd();

}

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

                case 67: // C
                    if (currentSceneState == SCENE_RUNNING)
                        currentSceneState = SCENE_CREDITS;
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
                    else if(currentSceneState == SCENE_CREDITS)
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
                float posX = LOWORD(lParam) / (float)dim.x;
                float posY = HIWORD(lParam) / (float)dim.y;

                if (
                    posX >= 0.6328f && posX <= 0.9056f &&
                    posY >= 0.4282f && posY <= 0.5139f
                ) {
                    currentSceneState = SCENE_TRANSITION;
                    snds->pauseMusic(backgroundMusic);
                }
                else if (
                    posX >= 0.6914f && posX <= 0.8431f &&
                    posY >= 0.5312f && posY <= 0.5972f
                ) {
                    currentSceneState = SCENE_RECOVERY;
                }
                else if (
                    posX >= 0.6809f && posX <= 0.8581f &&
                    posY >= 0.6226f && posY <= 0.6875f
                ) {
                    currentSceneState = SCENE_CREDITS;
                }
                else if (
                    posX >= 0.6861f && posX <= 0.8477f &&
                    posY >= 0.7106f && posY <= 0.7801f
                ) {
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

                if (
                    posX >= 0.6328f && posX <= 0.9056f &&
                    posY >= 0.4282f && posY <= 0.5139f
                    )
                {
                    mouseHoverState = HOVER_NEW;
                }

                else if (
                    posX >= 0.6914f && posX <= 0.8431f &&
                    posY >= 0.5312f && posY <= 0.5972f
                        )
                {
                    mouseHoverState = HOVER_HELP;
                }
                else if (
                    posX >= 0.6809f && posX <= 0.8581f &&
                    posY >= 0.6226f && posY <= 0.6875f
                    )
                {
                    mouseHoverState = HOVER_CREDITS;
                }
                else if(
                    posX >= 0.6861f && posX <= 0.8477f &&
                    posY >= 0.7106f && posY <= 0.7801f
                    )
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
