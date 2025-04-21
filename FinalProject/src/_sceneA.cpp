#include "_sceneA.h"


_sceneA::_sceneA()
{
    //ctor
    isAutoScroll = true;

}

_sceneA::~_sceneA()
{

    //dtor
    delete snds;
    delete myInputs;
    delete victoryTimer;
    delete immunityTimer;
    delete spawnTimer;
    delete transitionDelayTimer;
    delete textureLoader;
    delete camera;
    delete enemyFactory;

    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        delete obstacles[i].model;
        delete obstacles[i].weapon;
    }
}


GLint _sceneA::IniGL()
{
    glClearColor(0.0,0.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup camera
    camera->init();
    camera->eye.x = 0;
    camera->eye.y = 2.0;
    camera->eye.z = 3.0;


    // Initialize parallax
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    /* Load obstacles into array */
    for (int i = 0; i < TOTAL_OBSTACLES; i++)
        enemyFactory->GenerateVegeta(&obstacles[i].model, &obstacles[i].weapon, i);

    // Load popup images
    img_popup = textureLoader->loadImages("images/popup.png");
    img_defeat = textureLoader->loadImages("images/defeat.png");
    img_victory = textureLoader->loadImages("images/victory.png");


    // Start background music
    snds->initSound();
    backgroundMusic = snds->playMusic(MUSIC_FILE);
    backgroundMusic->setVolume(0.15f);
    return true;
}


GLvoid _sceneA::renderScene()
{
    int prevSceneState = currentSceneState;

    if(prevSceneState == SCENE_START){
        victoryTimer->reset();
        snds->resumeMusic(backgroundMusic);
    }

    transitionSceneState();

    if(prevSceneState != currentSceneState)
        cout << "State Change: " << prevSceneState << " -> " << currentSceneState << "  hits: " << playerHitCount << endl;


    // SCENE_START, SCENE_RUNNING, SCENE_COLLISION, SCENE_RECOVERY, SCENE_FAILURE, SCENE_VICTORY
    switch(currentSceneState)
    {
        case SCENE_START:
            //player->actionTrigger = playerW->actionTrigger = player->STAND;
            break;

        case SCENE_COLLISION:
            spawnTimer->pauseTime();
            //player->actionTrigger = playerW->actionTrigger = player->PAIN;
            if(collidedObstacle != nullptr)
                collidedObstacle->actionTrigger = collidedObstacleW->actionTrigger = collidedObstacle->DEATH;
            break;

        case SCENE_RUNNING:
            if(collidedObstacle != nullptr)
                collidedObstacle->actionTrigger = collidedObstacleW->actionTrigger = collidedObstacle->RUN;
        case SCENE_RECOVERY:
            spawnTimer->resumeTime();
            //player->actionTrigger = playerW->actionTrigger = player->RUN;
            break;

        case SCENE_FAILURE:
            victoryTimer->pauseTime();
            //player->actionTrigger = playerW->actionTrigger = player->DEATH;
            if(collidedObstacle != nullptr)
                collidedObstacle->actionTrigger = collidedObstacleW->actionTrigger = collidedObstacle->TAUNT;
            break;

        case SCENE_TRANSITION:
            snds->pauseMusic(backgroundMusic);
        case SCENE_VICTORY:
            //player->actionTrigger = playerW->actionTrigger = player->SALUTE;
            break;

        case SCENE_PAUSE:
            //player->actionTrigger = playerW->actionTrigger = player->STAND;
            break;

        default:
            break;
    }


    spawnObstacles();

    // reset the color buffer bit (all colors on screen) and depth bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update camera
    camera->setup();

    // Setup background
    glPushMatrix();

        glScalef(1, 1, 1);
        glDisable(GL_LIGHTING);

        glBegin(GL_POLYGON);
            glColor3f(165, 42, 42);

            glVertex3f(-2, 0, -1);
            glVertex3f(-2, 0, 1);
            glVertex3f(2, 0, 1);
            glVertex3f(2, 0, -1);

        glEnd();

        drawRoadHorizontal(-2, 0, -0.5, 0.1);
        drawRoadHorizontal(-2, 0, 0.5, 0.1);

        drawRoadVertical(-0.55, -0.10, 0, 0.1);
        drawRoadVertical(0.55, 0.10, 0, 0.1);

        drawRoadHorizontal(0, 2, -0.15, 0.1);
        drawRoadHorizontal(0, 2, 0.15, 0.1);

        glColor3f(1, 1, 1);


        // Advance enemies
        if(currentSceneState == SCENE_RUNNING || currentSceneState == SCENE_RECOVERY)
        {
            advanceEnemies();
        }

        if(currentSceneState == SCENE_PAUSE)
        {
            //float ratX = dim.x / dim.y;
            glBindTexture(GL_TEXTURE_2D, img_popup);
            glBegin(GL_POLYGON);
                glTexCoord2f(0, 1); glVertex3f(-1, -1, 0);
                glTexCoord2f(1, 1); glVertex3f(1, -1, 0);
                glTexCoord2f(1, 0); glVertex3f(1, 1, -0.5);
                glTexCoord2f(0, 0); glVertex3f(-1.0, 1, -0.5);
            glEnd();
        }
        else if (currentSceneState == SCENE_VICTORY)
        {
            glBindTexture(GL_TEXTURE_2D, img_victory);
            glBegin(GL_POLYGON);
                glTexCoord2f(0, 1); glVertex3f(-1, -1, 0);
                glTexCoord2f(1, 1); glVertex3f(1, -1, 0);
                glTexCoord2f(1, 0); glVertex3f(1, 1, -0.5);
                glTexCoord2f(0, 0); glVertex3f(-1.0, 1, -0.5);
            glEnd();
        }
        else if (currentSceneState == SCENE_FAILURE)
        {
            glBindTexture(GL_TEXTURE_2D, img_defeat);
            glBegin(GL_POLYGON);
                glTexCoord2f(0, 1); glVertex3f(-1, -1, 0);
                glTexCoord2f(1, 1); glVertex3f(1, -1, 0);
                glTexCoord2f(1, 0); glVertex3f(1, 1, -0.5);
                glTexCoord2f(0, 0); glVertex3f(-1.0, 1, -0.5);
            glEnd();
        }
        glEnable(GL_LIGHTING);

    glPopMatrix();


    // Draw enemies on the map
    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        if (obstacles[i].model->pathStep < 0) continue;
        glPushMatrix();
            glTranslatef(obstacles[i].model->pos.x, obstacles[i].model->pos.y, obstacles[i].model->pos.z);
            glScalef(
                obstacles[i].model->scale.x,
                obstacles[i].model->scale.y,
                obstacles[i].model->scale.y);

            glRotatef(obstacles[i].model->rotation.x, 1, 0, 0);
            glRotatef(obstacles[i].model->rotation.z, 0, 0, 1);
            obstacles[i].model->actions();
            obstacles[i].weapon->actions();

            obstacles[i].model->Draw();
            obstacles[i].weapon->Draw();

        glPopMatrix();
    }

}

void _sceneA::drawRoadHorizontal(float xStart, float xEnd, float z, float width)
{
    float w = width / 2.0;

    glBegin(GL_POLYGON);
            glColor3f(0, 0, 0);

            glVertex3f(xStart, 0, z - w);
            glVertex3f(xStart, 0, z + w);
            glVertex3f(xEnd, 0, z + w);
            glVertex3f(xEnd, 0, z - w);

    glEnd();
}

void _sceneA::drawRoadVertical(float zStart, float zEnd, float x, float width)
{
    float w = width / 2.0;

    glBegin(GL_POLYGON);
            glColor3f(0, 0, 0);

            glVertex3f(x-w, 0, zStart);
            glVertex3f(x-w, 0, zEnd);
            glVertex3f(x+w, 0, zEnd);
            glVertex3f(x+w, 0, zStart);

    glEnd();
}

void _sceneA::advanceEnemies()
{
    for(int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        if (obstacles[i].model->pathStep < 0) continue;

        if(obstacles[i].model->path == 0)
        {
            switch(obstacles[i].model->pathStep)
            {
                case 0:
                    obstacles[i].model->FaceRight();
                    obstacles[i].model->pos.x += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x >= 0)
                        obstacles[i].model->pathStep = 1;

                    break;

                case 1:
                    obstacles[i].model->FaceDown();
                    obstacles[i].model->pos.z += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z >= -0.15)
                        obstacles[i].model->pathStep = 2;

                    break;

                case 2:
                    obstacles[i].model->FaceRight();
                    obstacles[i].model->pos.x += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x >= 2)
                        obstacles[i].model->pathStep = -1;

                    break;
            }
        }
        else if (obstacles[i].model->path == 1)
        {
            switch(obstacles[i].model->pathStep)
            {
                case 0:
                    obstacles[i].model->FaceRight();
                    obstacles[i].model->pos.x += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x >= 0)
                        obstacles[i].model->pathStep = 1;

                    break;

                case 1:
                    obstacles[i].model->FaceUp();
                    obstacles[i].model->pos.z -= OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z <= 0.15)
                        obstacles[i].model->pathStep = 2;

                    break;

                case 2:
                    obstacles[i].model->FaceRight();
                    obstacles[i].model->pos.x += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x >= 2)
                        obstacles[i].model->pathStep = -1;

                    break;
            }
        }
    }
}


void _sceneA::transitionSceneState()
{

    int newState = currentSceneState;

    if (currentSceneState == SCENE_PAUSE)
    {
        return;
    }
    else if (currentSceneState == SCENE_TRANSITION)
    {
        return;
    }
    else if (currentSceneState == SCENE_FAILURE)
    {
        if (transitionDelayTimer->getTicks() >= TRANSITION_TIMER_MS)
        {
            reset();
            return;
        }
    }
    else if (currentSceneState == SCENE_VICTORY)
    {
        if (transitionDelayTimer->getTicks() >= TRANSITION_TIMER_MS)
        {
            newState = SCENE_TRANSITION;
        }
    }
    else if(playerHitCount > MAX_HITS)
    {
        snds->playSound(SOUND_FAIL);
        newState = SCENE_FAILURE;
        transitionDelayTimer->reset();
    }
    /*else if(victoryTimer->getTicks() >= VICTORY_TIMER_MS)
    {
        snds->playSound(SOUND_SUCCESS);
        newState = SCENE_VICTORY;
        transitionDelayTimer->reset();
    }*/
    else if(
        currentSceneState == SCENE_COLLISION &&
        immunityTimer->getTicks() < RECOVER_TIMER_MS)
    {
        newState = SCENE_COLLISION;
    }
    else if (
        (
            currentSceneState == SCENE_COLLISION ||
            currentSceneState == SCENE_RECOVERY)
        && immunityTimer->getTicks() < IMMUNITY_TIMER_MS)
    {
        newState = SCENE_RECOVERY;
    }
    else if(
        currentSceneState == SCENE_RUNNING
        && hasCollided())
    {
        newState = SCENE_COLLISION;
        playerHitCount++;
        immunityTimer->reset();
        snds->playSound(SOUND_COLLISION_FILE);
        cout << "collision with: " << collidedObstacle->debugId << endl;
    }
    else newState = SCENE_RUNNING;

    currentSceneState = newState;
}

void _sceneA::spawnObstacles()
{
    // Check if we're within timer range
    if(spawnTimer->getTicks() >= spawnTimerDelayMs)
    {
        // A new obstacle can be spawned
        int spawnLocation = (rand() % 100);


        _3dmodelloader *obj1 = getAvailableObstacleModel();
        if(obj1 == nullptr)
        {
            cout << "*** No obstacles available ***" << endl;
        }
        else
        {

            if (spawnLocation < 50)
            {
                obj1->pos.x = -2;
                obj1->pos.z = -0.5;
                obj1->path = 0;
            }
            else
            {
                // Location B
                obj1->pos.x = -2;
                obj1->pos.z = 0.5;
                obj1->path = 1;
            }

            obj1->pathStep = 0;

            cout << "Obstacle: " << obj1->debugId << " at spawn location " << obj1->path << endl;

        }

        // Choose new spawn delay interval and reset timer
        spawnTimer->reset();
        spawnTimerDelayMs = (rand() % spawnTimerDelayRange) + spawnTimerDelayMinimumDuration;

        cout << "New Delay: " << spawnTimerDelayMs << endl;
    }
}

_3dmodelloader* _sceneA::getAvailableObstacleModel()
{

    // Find the first obstacle that is off the screen
    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        if (obstacles[i].model->pathStep < 0)
            return obstacles[i].model;
    }

    return nullptr;
}

bool _sceneA::hasCollided()
{

    return false;
}

void _sceneA::reset()
{
    playerHitCount = 0;
    currentSceneState = SCENE_START;
    victoryTimer->reset();

    collidedObstacle = nullptr;
    collidedObstacleW = nullptr;

    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        obstacles[i].model->pos.z = 2;
        obstacles[i].weapon->pos.z = 2;
        obstacles[i].model->actionTrigger == obstacles[i].model->RUN;
        obstacles[i].weapon->actionTrigger == obstacles[i].weapon->RUN;
    }
}


int _sceneA::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            myInputs->wParam = wParam;

            if(wParam == VK_ESCAPE)
            {
                if (currentSceneState == SCENE_PAUSE)
                {
                    victoryTimer->resumeTime();
                    immunityTimer->resumeTime();
                    spawnTimer->resumeTime();
                    transitionDelayTimer->resumeTime();
                    currentSceneState = returnToStateAfterPause;
                }
                else
                {
                    returnToStateAfterPause = currentSceneState;
                    currentSceneState = SCENE_PAUSE;
                    victoryTimer->pauseTime();
                    immunityTimer->pauseTime();
                    spawnTimer->pauseTime();
                    transitionDelayTimer->pauseTime();
                }

            }
            else if (wParam == 13) // Enter
            {
                if(currentSceneState == SCENE_PAUSE)
                    currentSceneState = SCENE_EXIT;
            }
            else if(currentSceneState == SCENE_RUNNING || currentSceneState == SCENE_RECOVERY)
            {
                //myInputs->keyPressed(player, playerW);
            }
            else
            {
                myInputs->keyPressed(camera);
            }

            // Quickly exit to next level
            if (wParam == VK_END)
                currentSceneState = SCENE_VICTORY;
            break;

        case WM_KEYUP: break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            break;

        case WM_MOUSEMOVE:
            mouseMapping(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOUSEWHEEL:
            break;
    }
}
