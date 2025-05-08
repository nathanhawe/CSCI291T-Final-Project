#include "_sceneManager.h"

_sceneManager::_sceneManager()
{
    //ctor
    sceneLanding = new _sceneLanding();
    sceneA = new _sceneA();
    sceneB = new _sceneB();
    sceneC = new _sceneC();
    activeScene = sceneLanding;

}

_sceneManager::~_sceneManager()
{
    //dtor
    delete sceneLanding;
    delete sceneA;
    delete sceneB;
    delete sceneC;
}

GLint _sceneManager::init()
{
    sceneLanding->IniGL();
}

GLvoid _sceneManager::renderActiveScene()
{
    if(activeScene->currentSceneState == activeScene->SCENE_EXIT)
    {
        if (activeScene == sceneLanding) {
            exit = true;
            return;
        }
        activeScene->snds->pauseMusic(activeScene->backgroundMusic);
        activeScene = sceneLanding;
        activeScene->currentSceneState = activeScene->SCENE_START;
        cout << "Manager: Switched to Landing" << endl;
    }

    switchActiveScene();

    activeScene->renderScene();
}

GLvoid _sceneManager::resizeWindow(GLsizei width, GLsizei height)
{
    activeScene->resizeWindow(width, height);

    dimension.x = width;
    dimension.y = height;
}

int _sceneManager::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    activeScene->winMsg(hWnd, uMsg, wParam, lParam);
}

void _sceneManager::switchActiveScene()
{

    if (activeScene->currentSceneState == activeScene->SCENE_TRANSITION)
    {
        if (activeScene == sceneLanding)
        {
            if(!hasLoadedGameScene[0])
            {
                sceneA->IniGL();
                hasLoadedGameScene[0] = true;
            }
            else sceneA->reset();

            activeScene=sceneA;
            activeScene->resizeWindow(dimension.x, dimension.y);
            cout << "Manager: Switched to Scene A" << endl;

        }
        else if (activeScene == sceneA)
        {
            if(!hasLoadedGameScene[1])
            {
                sceneB->IniGL();
                hasLoadedGameScene[1] = true;
            }
            else sceneB->reset();

            activeScene = sceneB;
            activeScene->resizeWindow(dimension.x, dimension.y);
            cout << "Manager: Switched to Scene B" << endl;
        }
        else if (activeScene == sceneB)
        {
            if(!hasLoadedGameScene[2])
            {
                sceneC->IniGL();
                hasLoadedGameScene[2] = true;
            }
            else sceneC->reset();

            activeScene = sceneC;
            activeScene->resizeWindow(dimension.x, dimension.y);
            cout << "Manager: Switched to Scene C" << endl;

        }
        else
        {
            // If we don't have logic to exit the current scene, return to the menu
            activeScene = sceneLanding;
            activeScene->currentSceneState = activeScene->SCENE_START;
            cout << "Manager: Switched to Landing" << endl;
        }
    }
}
