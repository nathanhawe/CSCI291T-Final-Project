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
    loadVegetaModel(0, MODEL_SKIN_VEGETA_BASE, &obstacles[0].model); loadVegetaWeapon(100, WEAPON_SKIN_VEGETA_BASE, &obstacles[0].weapon);
    loadVegetaModel(1, MODEL_SKIN_VEGETA_SS, &obstacles[1].model); loadVegetaWeapon(101, WEAPON_SKIN_VEGETA_SS, &obstacles[1].weapon);
    loadVegetaModel(2, MODEL_SKIN_VEGETA_BASE, &obstacles[2].model); loadVegetaWeapon(102, WEAPON_SKIN_VEGETA_BASE, &obstacles[2].weapon);
    loadVegetaModel(3, MODEL_SKIN_VEGETA_SS, &obstacles[3].model); loadVegetaWeapon(103, WEAPON_SKIN_VEGETA_SS, &obstacles[3].weapon);

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

void _sceneA::loadVegetaModel(int id, char* filename, _3dmodelloader **mdl)
{
        *mdl = new _3dmodelloader();

        (*mdl)->initModel(MODEL_TRIS_VEGETA, filename);
        (*mdl)->pos.y = 0.1;
        (*mdl)->pos.z = 3;

        // Action frames
        (*mdl)->SetActionFrameRange((*mdl)->STAND, 0, 39);
        (*mdl)->SetActionFrameRange((*mdl)->RUN, 40, 45);
        (*mdl)->SetActionFrameRange((*mdl)->ATTACK, 46, 53);
        (*mdl)->SetActionFrameRange((*mdl)->PAIN, 54, 65);
        (*mdl)->SetActionFrameRange((*mdl)->DEATH, 178, 197);
        (*mdl)->SetActionFrameRange((*mdl)->TAUNT, 95, 111);
        (*mdl)->debugId = id;
}

void _sceneA::loadVegetaWeapon(int id, char* filename, _3dmodelloader **mdl)
{
    *mdl = new _3dmodelloader();

        (*mdl)->initModel(WEAPON_TRIS_VEGETA, filename);

        // Action frames
        (*mdl)->SetActionFrameRange((*mdl)->STAND, 0, 39);
        (*mdl)->SetActionFrameRange((*mdl)->RUN, 40, 45);
        (*mdl)->SetActionFrameRange((*mdl)->ATTACK, 46, 53);
        (*mdl)->SetActionFrameRange((*mdl)->PAIN, 54, 65);
        (*mdl)->SetActionFrameRange((*mdl)->DEATH, 178, 197);
        (*mdl)->SetActionFrameRange((*mdl)->TAUNT, 95, 111);
        (*mdl)->debugId = id;
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

        glPushMatrix();

        //glEnable(GL_LIGHTING);

        glBindTexture(GL_TEXTURE_2D, img_ground);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-2, 0, -1);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 0,  1);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 2, 0,  1);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 2, 0, -1);
        glEnd();

        glPopMatrix();

        //glDisable(GL_TEXTURE_2D);




        if(currentSceneState == SCENE_RUNNING || currentSceneState == SCENE_RECOVERY)
        {
            for(int i = 0; i < TOTAL_OBSTACLES; i++)
            {
                obstacles[i].model->pos.z += OBSTACLE_SPEED;
            }
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


    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        glPushMatrix();
            glTranslatef(obstacles[i].model->pos.x, obstacles[i].model->pos.y, obstacles[i].model->pos.z);
            glScalef(0.002, 0.002, 0.002);
            glRotatef(-90.0, 1, 0, 0);
            glRotatef(-90.0, 0, 0, 1);
            obstacles[i].model->actions();
            obstacles[i].weapon->actions();

            obstacles[i].model->Draw();
            obstacles[i].weapon->Draw();

        glPopMatrix();
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
    else if(victoryTimer->getTicks() >= VICTORY_TIMER_MS)
    {
        snds->playSound(SOUND_SUCCESS);
        newState = SCENE_VICTORY;
        transitionDelayTimer->reset();
    }
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
        int lane = (rand() % 3);
        int pattern = (rand() % 100);


        if (pattern < 75)
        {
            /* Single Obstacle Pattern */
            _3dmodelloader *obj1 = getAvailableObstacleModel();
            if(obj1 == nullptr)
            {
                cout << "No obstacles available (pattern 1)." << endl;
            }
            else
            {
                obj1->pos.z = -2;
                obj1->SetLane(lane);
                cout << "Obstacle: " << obj1->debugId << " in lane " << lane << endl;
            }
        }
        else
        {
            /* Double Obstacle Pattern */
            // Double obstacle pattern placed on either side of lane
            _3dmodelloader *obj1 = getAvailableObstacleModel();
            if(obj1 == nullptr)
            {
                cout << "No obstacles available (pattern 1/2)." << endl;
            }
            else
            {
                obj1->pos.z = -2;

                if(lane == 0)
                {
                    obj1->SetLane(1);
                }
                else obj1->SetLane(0);

                cout << "Obstacle: " << obj1->debugId << " lane " << obj1->GetLane() << endl;
            }

            _3dmodelloader *obj2 = getAvailableObstacleModel();
            if(obj2 == nullptr)
            {
                cout << "No obstacles available (pattern 2/2)." << endl;
            }
            else
            {
                obj2->pos.z = -2;

                if(lane == 2)
                {
                    obj2->SetLane(1);
                }
                else obj2->SetLane(2);

                cout << "Obstacle: " << obj2->debugId << " lane " << obj2->GetLane() << endl;
            }
        }


        // Choose new spawn delay interval and reset timer
        spawnTimer->reset();
        spawnTimerDelayMs = (rand() % spawnTimerDelayRange) + spawnTimerDelayMinimumDuration;

        cout << "New Obstacle Pattern: " << pattern << ", Lane: " << lane << ", New Delay: " << spawnTimerDelayMs << endl;
    }
}

_3dmodelloader* _sceneA::getAvailableObstacleModel()
{

    // Find the first obstacle that is off the screen (z position is > 2)
    // or nullptr if none are available
    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        if (obstacles[i].model->pos.z > 2)
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
            break;

        case WM_MOUSEWHEEL:
            break;
    }
}
