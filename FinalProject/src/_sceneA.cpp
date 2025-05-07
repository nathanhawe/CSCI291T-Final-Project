#include "_sceneA.h"


_sceneA::_sceneA()
{
    istowerType = 0; //0 for tower, 1 for tesla
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
    delete globalTimer;
    delete camera;
    delete enemyFactory;

    delete backgroundMusic;
    delete laserSoundSource;
    delete electricSoundSource;

    delete fort1;

    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        delete obstacles[i].model;
        delete obstacles[i].weapon;
    }
}


GLint _sceneA::IniGL()
{
    _baseScene::init();

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
        enemyFactory->GenerateTekk(&obstacles[i].model, &obstacles[i].weapon, i);

    // Load popup images
    img_popup = textureLoader->loadImages("images/popup.png");
    img_defeat = textureLoader->loadImages("images/defeat.png");
    img_victory = textureLoader->loadImages("images/victory.png");

    ground_tex = textureLoader->loadImages("images/ground.jpg");
    dirt_tex = textureLoader->loadImages("images/dirt.jpg");


    // Load overlay images
    overlay1_notReady = textureLoader->loadImages("images/overlay/1-not_ready.png");
    overlay1_ready = textureLoader->loadImages("images/overlay/1-ready.png");
    overlay1_selected = textureLoader->loadImages("images/overlay/1-selected.png");
    overlay2_disabled = textureLoader->loadImages("images/overlay/2-disabled.png");
    overlay3_disabled = textureLoader->loadImages("images/overlay/3-disabled.png");




    // Start background music
    snds->initSound();
    sky->skyBoxInit();
    fort1->initFortassets();
    backgroundMusic = snds->playMusic(MUSIC_FILE);
    backgroundMusic->setVolume(0.15f);
    laserSoundSource = snds->loadSoundSource(SOUND_LASER);
    laserSoundSource->setDefaultVolume(0.30f);
    electricSoundSource = snds->loadSoundSource(SOUND_ELECTRIC);
    electricSoundSource->setDefaultVolume(0.30f);

    // Setup game state
    waveSize = WAVE_SIZE;
    enemiesDefeatedCount = 0;
    totalEnemiesSpawned = 0;
    availableResources = TOWER_BASE_COST;
    totalSpentResources = 0;

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

    sky->drawSkyBox();

    // Setup background
    glPushMatrix();

        glScalef(1, 1, 1);
        glDisable(GL_LIGHTING);

        drawOverlay();
        drawGround();


        drawRoadHorizontal(-2, 0, -0.5, 0.1);
        drawRoadHorizontal(-2, 0, 0.5, 0.1);

        drawRoadVertical(-0.55, -0.10, 0, 0.1);
        drawRoadVertical(0.55, 0.10, 0, 0.1);

        drawRoadHorizontal(0, 2, -0.15, 0.1);
        drawRoadHorizontal(0, 2, 0.15, 0.1);

        glPushMatrix();
        glTranslatef(-0.8f, 0.2f, 0.4f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.002f, 0.002f, 0.002f);

        fort1->drawfort();
        glPopMatrix();

        glColor3f(1, 1, 1);


        // Advance enemies
        if(currentSceneState == SCENE_RUNNING)
        {
            advanceEnemies();
            drawPlacementCircle(0.05, 0.18);
            advanceAndDrawBullets(bullets, BULLET_TIMER_DELAY, BULLET_SPEED);
            checkBulletCollision(bullets, towers);
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

    for (int i = 0; i < TOTAL_TOWERS; i++)
        {
            if (!towers[i].isActive) continue;

            // Calculate tower center position
            float posX = (towers[i].xMin + towers[i].xMax) / 2.0f;
            float posY = towers[i].yMin; // base Y
            float posZ = (towers[i].zMin + towers[i].zMax) / 2.0f;

            // Calculate size (used for scaling)
            float towerWidth = towers[i].xMax - towers[i].xMin;
            float towerHeight = towers[i].yMax - towers[i].yMin;

            drawTowerAt(posX, posY, posZ, towerWidth, towerHeight);
            //drawTeslaTowerAt(posX, posY, posZ, towerWidth, towerHeight);
        }

    checkAndUpdateEnemyTargets(obstacles, towers);
    attackEnemyTargets(obstacles, towers, bullets, OBSTACLE_FIRE_DELAY);

    checkAndUpdateTargets();
    attackTargets();
    drawLasers();


    glColor3f(1, 1, 1);

}

void _sceneA::drawOverlay()
{

    if(isPlacingTower)
    {
        glBindTexture(GL_TEXTURE_2D, overlay1_selected);
    }
    else if (availableResources >= TOWER_BASE_COST)
    {
        glBindTexture(GL_TEXTURE_2D, overlay1_ready);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, overlay1_notReady);
    }

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-0.35, 1.00, -0.5);
        glTexCoord2f(0, 1); glVertex3f(-0.35, 0.95, 0);
        glTexCoord2f(1, 1); glVertex3f(-0.2, 0.95, 0);
        glTexCoord2f(1, 0); glVertex3f(-0.2, 1.00, -0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, overlay2_disabled);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-0.075, 1.00, -0.5);
        glTexCoord2f(0, 1); glVertex3f(-0.075, 0.95, 0);
        glTexCoord2f(1, 1); glVertex3f(0.075, 0.95, 0);
        glTexCoord2f(1, 0); glVertex3f(0.075, 1.00, -0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, overlay3_disabled);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(0.2, 1.00, -0.5);
        glTexCoord2f(0, 1); glVertex3f(0.2, 0.95, 0);
        glTexCoord2f(1, 1); glVertex3f(0.35, 0.95, 0);
        glTexCoord2f(1, 0); glVertex3f(0.35, 1.00, -0.5);
    glEnd();

}


void _sceneA::drawRoadHorizontal(float xStart, float xEnd, float z, float width)
{
    float w = width / 2.0f;

    glBindTexture(GL_TEXTURE_2D, dirt_tex);

    glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(xStart, 0, z - w);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(xStart, 0, z + w);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(xEnd, 0, z + w);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(xEnd, 0, z - w);
    glEnd();
}


void _sceneA::drawRoadVertical(float zStart, float zEnd, float x, float width)
{
    float w = width / 2.0f;

    glBindTexture(GL_TEXTURE_2D, dirt_tex);

    glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - w, 0, zStart);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - w, 0, zEnd);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, 0, zEnd);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, 0, zStart);
    glEnd();
}

void _sceneA::drawGround()
{
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ground_tex);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2, 0, -1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 0,  1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 2, 0,  1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 2, 0, -1);
    glEnd();

    //glDisable(GL_TEXTURE_2D);
}

void _sceneA::drawLasers()
{
    glDisable(GL_LIGHTING);
    glLineWidth(2.5f);
    if (istowerType == 0) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red for regular tower
        } else  {
            glColor3f(0.2f, 0.5f, 0.9f);
        }
    glBegin(GL_LINES);
    for (int i = 0; i < TOTAL_TOWERS; i++) {
        int idx = towers[i].targetEnemyIndex;
        if (
            idx == -1
            || !towers[i].isActive
            || obstacles[idx].model->pathStep < 0
            || !towers[i].hasFirstAttack
            || (towers[i].lastAttackTicks + LASER_DURATION) > globalTimer->getTicks())
            continue;

        float tx = (towers[i].xMin + towers[i].xMax) / 2.0f;
        float ty = towers[i].yMax;
        float tz = (towers[i].zMin + towers[i].zMax) / 2.0f;

        float ex = obstacles[idx].model->pos.x;
        float ey = obstacles[idx].model->pos.y;
        float ez = obstacles[idx].model->pos.z;

        glVertex3f(tx, ty, tz);  // From tower
        glVertex3f(ex, ey, ez);  // To enemy
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void _sceneA::checkAndUpdateTargets()
{
    for (int i = 0; i < TOTAL_TOWERS; i++) {
        if (!towers[i].isActive) continue;

        float tx = (towers[i].xMin + towers[i].xMax) / 2.0f;
        float ty = (towers[i].yMin + towers[i].yMax) / 2.0f;
        float tz = (towers[i].zMin + towers[i].zMax) / 2.0f;

        float closestDist = 9999.0f;
        int closestEnemy = -1;

        for (int j = 0; j < TOTAL_OBSTACLES; j++) {
            if (obstacles[j].model->pathStep < 0) continue;

            float ex = obstacles[j].model->pos.x;
            float ez = obstacles[j].model->pos.z;

            float distSq = (ex - tx) * (ex - tx) + (ez - tz) * (ez - tz);
            float range = 0.25f;

            if (distSq <= range * range && distSq < closestDist) {
                closestDist = distSq;
                closestEnemy = j;
            }
        }

        towers[i].targetEnemyIndex = closestEnemy;
    }
}

void _sceneA::attackTargets()
{
    clock_t nextFireTicks;
    int idx;
    for (int i = 0; i < TOTAL_TOWERS; i++)
    {
        idx = towers[i].targetEnemyIndex;
        nextFireTicks = towers[i].lastAttackTicks + TOWER_FIRE_DELAY;
        if (
            idx == -1
            || !towers[i].isActive
            || obstacles[idx].model->pathStep < 0
            || nextFireTicks > globalTimer->getTicks())
            continue;

        towers[i].lastAttackTicks = globalTimer->getTicks();
        towers[i].hasFirstAttack = true;
        if (istowerType == 0){
        snds->playSoundSource(laserSoundSource);
        }else {
        snds->playSoundSource(electricSoundSource);
        }

        cout << "TOWER " << i << " --> " << idx;

        // Add damage to enemy
        obstacles[idx].model->hitCount++;
        if (obstacles[idx].model->hitCount >= obstacles[idx].model->maxHits)
        {
            enemiesDefeatedCount++;
            availableResources++;
            obstacles[idx].model->pathStep = -1;
        }

        cout << " " << obstacles[idx].model->hitCount << "/" << obstacles[idx].model->maxHits << "     " << enemiesDefeatedCount << endl;
        debug();


    }
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

        if (obstacles[i].model->lastMovementTime + OBSTACLE_TIMER_DELAY > globalTimer->getTicks()) continue;
        obstacles[i].model->lastMovementTime = globalTimer->getTicks();

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
                    {
                        obstacles[i].model->pathStep = -1;

                        playerHitCount++;
                        debug();
                    }

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
                    {
                        obstacles[i].model->pathStep = -1;
                        playerHitCount++;
                        debug();
                    }

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
    else if((enemiesDefeatedCount + playerHitCount) >= WAVE_SIZE)
    {
        snds->playSound(SOUND_SUCCESS);
        newState = SCENE_VICTORY;
        transitionDelayTimer->reset();
    }
    else newState = SCENE_RUNNING;

    currentSceneState = newState;
}

void _sceneA::spawnObstacles()
{
    // Check if we're within timer range and haven't maxed out level spawns
    if(totalEnemiesSpawned < WAVE_SIZE && spawnTimer->getTicks() >= spawnTimerDelayMs)
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
            obj1->hitCount = 0;

            totalEnemiesSpawned++;

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
        {
            obstacles[i].targetEnemyIndex = -1;
            return obstacles[i].model;
        }

    }

    return nullptr;
}

bool _sceneA::hasCollided()
{

    return false;
}

void _sceneA::reset()
{
    waveSize = WAVE_SIZE;
    enemiesDefeatedCount = 0;
    totalEnemiesSpawned = 0;
    playerHitCount = 0;
    availableResources = TOWER_BASE_COST;
    totalSpentResources = 0;

    currentSceneState = SCENE_START;
    victoryTimer->reset();

    for (int i = 0; i < TOTAL_OBSTACLES; i++)
    {
        obstacles[i].model->pathStep = -1;
        obstacles[i].targetEnemyIndex = -1;
    }

    for (int i = 0; i < TOTAL_TOWERS; i++)
    {
        towers[i].isActive = false;
    }

    for (int i = 0; i < TOTAL_BULLETS; i++)
    {
        bullets[i].isActive = false;
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
                    globalTimer->resumeTime();
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
                    globalTimer->pauseTime();
                }

            }
            else if (wParam == 13) // Enter
            {
                if(currentSceneState == SCENE_PAUSE)
                    currentSceneState = SCENE_EXIT;
            }
            else if(currentSceneState == SCENE_RUNNING || currentSceneState == SCENE_RECOVERY)
            {
                if(wParam == 49 && availableResources >= TOWER_BASE_COST) // 1 on keyboard
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
                createTowerAtPoint(0, mouseX, mouseZ, towers);

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
