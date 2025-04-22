#include "_sceneA.h"


_sceneA::_sceneA()
{
    //ctor

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

    img_ground = textureLoader->loadImages("images/ground.jpg");




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
            drawPlacementCircle(0.05, 0.18);
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

    // Draw towers on the map
    glColor4f(0, 0, 255, 0.8);
    for (int i = 0; i < TOTAL_TOWERS; i++)
    {
        if (!towers[i].isActive) continue;

        glBegin(GL_QUADS);


            // Top
            glVertex3f(towers[i].xMin, towers[i].yMax, towers[i].zMin);
            glVertex3f(towers[i].xMin, towers[i].yMax, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMax, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMax, towers[i].zMin);

            // Bottom
            glVertex3f(towers[i].xMin, towers[i].yMin, towers[i].zMin);
            glVertex3f(towers[i].xMin, towers[i].yMin, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMin, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMin, towers[i].zMin);

            // Front
            glVertex3f(towers[i].xMin, towers[i].yMax, towers[i].zMax);
            glVertex3f(towers[i].xMin, towers[i].yMin, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMin, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMax, towers[i].zMax);

            // Back
            glVertex3f(towers[i].xMin, towers[i].yMax, towers[i].zMin);
            glVertex3f(towers[i].xMin, towers[i].yMin, towers[i].zMin);
            glVertex3f(towers[i].xMax, towers[i].yMin, towers[i].zMin);
            glVertex3f(towers[i].xMax, towers[i].yMax, towers[i].zMin);

            // Left
            glVertex3f(towers[i].xMin, towers[i].yMax, towers[i].zMin);
            glVertex3f(towers[i].xMin, towers[i].yMin, towers[i].zMin);
            glVertex3f(towers[i].xMin, towers[i].yMin, towers[i].zMax);
            glVertex3f(towers[i].xMin, towers[i].yMax, towers[i].zMax);

            // Right
            glVertex3f(towers[i].xMax, towers[i].yMax, towers[i].zMin);
            glVertex3f(towers[i].xMax, towers[i].yMin, towers[i].zMin);
            glVertex3f(towers[i].xMax, towers[i].yMin, towers[i].zMax);
            glVertex3f(towers[i].xMax, towers[i].yMax, towers[i].zMax);

        glEnd();
    }
    glColor3f(1, 1, 1);

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

void _sceneA::drawPlacementCircle(float towerSize, float towerRange)
{
    if (!isPlacingTower) return;
    // Is the mouse within the bounds?
    if (
        mouseX < -2 || mouseX > 2 ||
        mouseZ < -1 || mouseZ > 1)
        return;

    if (
        mouseX >= -2 - towerSize && mouseX <= 0 + towerSize &&
        mouseZ <= -0.45 + towerSize && mouseZ >= -0.55 - towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -2 - towerSize && mouseX <= 0 + towerSize &&
        mouseZ >= 0.45 - towerSize && mouseZ <= 0.55 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= 0 - towerSize && mouseX <= 2 + towerSize &&
        mouseZ <= -0.1 + towerSize && mouseZ >= -0.2 - towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= 0 - towerSize && mouseX <= 2 + towerSize &&
        mouseZ >= 0.1 - towerSize && mouseZ <= 0.2 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -0.05 - towerSize && mouseX <= 0.05 + towerSize &&
        mouseZ <= -0.10 + towerSize && mouseZ >= -0.55 - towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -0.05 - towerSize && mouseX <= 0.05 + towerSize &&
        mouseZ >= 0.10 - towerSize && mouseZ <= 0.55 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else isTowerPlaceable = true;
    // TODO: Add check of existing towers as well.

    if (isTowerPlaceable)
    {
        glBegin(GL_POLYGON);
            glColor4f(0, 255, 0, 0.3);
            glVertex3f(mouseX - towerRange, 0, mouseZ - towerRange);
            glVertex3f(mouseX - towerRange, 0, mouseZ + towerRange);
            glVertex3f(mouseX + towerRange, 0, mouseZ + towerRange);
            glVertex3f(mouseX + towerRange, 0, mouseZ - towerRange);
        glEnd();

    }

    glBegin(GL_POLYGON);
        if(isTowerPlaceable)
        {
            glColor4f(0, 255, 0, 0.75);
        }
        else glColor4f(255, 0, 0, 0.5);

        glVertex3f(mouseX - towerSize, 0, mouseZ - towerSize);
        glVertex3f(mouseX - towerSize, 0, mouseZ + towerSize);
        glVertex3f(mouseX + towerSize, 0, mouseZ + towerSize);
        glVertex3f(mouseX + towerSize, 0, mouseZ - towerSize);

    glEnd();
    glColor3f(1, 1, 1);
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

void _sceneA::createTowerAtPoint(int towerType, float x, float z)
{
    if(!isPlacingTower || !isTowerPlaceable) return;

    //find the first available tower slot
    for (int i = 0; i < TOTAL_TOWERS; i++)
    {
        if (towers[i].isActive) continue;

        towers[i].health = 100;
        towers[i].isActive = true;
        towers[i].type = towerType;

        towers[i].xMin = x - 0.05;
        towers[i].xMax = x + 0.05;
        towers[i].yMin = 0;
        towers[i].yMax = 0.15;
        towers[i].zMin = z - 0.05;
        towers[i].zMax = z + 0.05;

        return;
    }

    cout << "*** No available towers! ***" << endl;
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
    //cout << "wParam = " << wParam << endl;
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
                if(wParam == 49) // 1 on keyboard
                    isPlacingTower = !isPlacingTower;
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
            if(currentSceneState == SCENE_RUNNING)
                createTowerAtPoint(0, mouseX, mouseZ);

            break;

        case WM_RBUTTONUP:
            isPlacingTower = false;
            break;
        case WM_MBUTTONUP:
            break;

        case WM_MOUSEMOVE:
            mouseMapping(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOUSEWHEEL:
            break;
    }
}
