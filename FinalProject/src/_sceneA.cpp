#include "_sceneA.h"


_sceneA::_sceneA()
{
    istowerType = 0; //0 for tower, 1 for tesla
}

_sceneA::~_sceneA()
{

    //dtor
    delete myInputs;
    delete victoryTimer;
    delete immunityTimer;
    delete spawnTimer;
    delete transitionDelayTimer;
    delete globalTimer;
    delete camera;
    delete enemyFactory;
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
    img_popup = textureLoader->loadImages("images/popup1.png");
    img_help = textureLoader->loadImages("images/help_page.png");
    img_defeat = textureLoader->loadImages("images/defeat.png");
    img_victory = textureLoader->loadImages("images/victory.png");


    // Load overlay images
    overlay1_notReady = textureLoader->loadImages("images/overlay/1-not_ready.png");
    overlay1_ready = textureLoader->loadImages("images/overlay/1-ready.png");
    overlay1_selected = textureLoader->loadImages("images/overlay/1-selected.png");
    overlay2_disabled = textureLoader->loadImages("images/overlay/2-disabled.png");
    overlay3_disabled = textureLoader->loadImages("images/overlay/3-disabled.png");




    // Start background music
    sky->skyBoxInit();
    fort1->initFortassets();
    backgroundMusic = snds->playMusic(MUSIC_FILE);
    backgroundMusic->setVolume(0.15f);

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
            spawnTimer->resumeTime();
        case SCENE_RECOVERY:

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

        fort1->drawTree(-600.0f, 300.0f, 10.0f);
        fort1->drawTree(-500.0f, 300.0f, 10.0f);
        fort1->drawTree(-400.0f, 300.0f, 10.0f);
        fort1->drawTree(-300.0f, 300.0f, 10.0f);
        fort1->drawTree(-200.0f, 300.0f, 10.0f);
        fort1->drawTree(-100.0f, 300.0f, 10.0f);
        fort1->drawTree(0.0f, 300.0f, 10.0f);
        fort1->drawTree(100.0f, 300.0f, 10.0f);
        fort1->drawTree(200.0f, 300.0f, 10.0f);
        fort1->drawTree(300.0f, 300.0f, 10.0f);
        fort1->drawTree(400.0f, 300.0f, 10.0f);
        fort1->drawTree(500.0f, 300.0f, 10.0f);
        fort1->drawTree(600.0f, 300.0f, 10.0f);
        fort1->drawTree(700.0f, 300.0f, 10.0f);
        fort1->drawTree(800.0f, 300.0f, 10.0f);
        fort1->drawTree(900.0f, 300.0f, 10.0f);
        fort1->drawTree(1000.0f, 300.0f, 10.0f);
        fort1->drawTree(1100.0f, 300.0f, 10.0f);
        fort1->drawTree(1200.0f, 300.0f, 10.0f);
        fort1->drawTree(1300.0f, 300.0f, 10.0f);


        fort1->drawTree(-600.0f, -650.0f, 10.0f);
        fort1->drawTree(-500.0f, -650.0f, 10.0f);
        fort1->drawTree(-400.0f, -650.0f, 10.0f);
        fort1->drawTree(-300.0f, -650.0f, 10.0f);
        fort1->drawTree(-200.0f, -650.0f, 10.0f);
        fort1->drawTree(-100.0f, -650.0f, 10.0f);
        fort1->drawTree(0.0f, -650.0f, 10.0f);
        fort1->drawTree(100.0f, -650.0f, 10.0f);
        fort1->drawTree(200.0f, -650.0f, 10.0f);
        fort1->drawTree(300.0f, -650.0f, 10.0f);
        fort1->drawTree(400.0f, -650.0f, 10.0f);
        fort1->drawTree(500.0f, -650.0f, 10.0f);
        fort1->drawTree(600.0f, -650.0f, 10.0f);
        fort1->drawTree(700.0f, -650.0f, 10.0f);
        fort1->drawTree(800.0f, -650.0f, 10.0f);
        fort1->drawTree(900.0f, -650.0f, 10.0f);
        fort1->drawTree(1000.0f, -650.0f, 10.0f);
        fort1->drawTree(1100.0f, -650.0f, 10.0f);
        fort1->drawTree(1200.0f, -650.0f, 10.0f);
        fort1->drawTree(1300.0f, -650.0f, 10.0f);


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
    drawLasers(towers, obstacles, LASER_DURATION);


    glColor3f(1, 1, 1);


    if(currentSceneState == SCENE_PAUSE)
        {
           glDisable(GL_DEPTH_TEST);    // Disable depth test to draw on top
            glDisable(GL_LIGHTING);      // No lighting for 2D texture

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();

            float ratX = dim.x / dim.y;
            gluOrtho2D(-ratX, ratX, -1.0, 1.0);  // Screen-aligned 2D view

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glBindTexture(GL_TEXTURE_2D, img_popup);
            glColor3f(1.0f, 1.0f, 1.0f); // Ensure texture not tinted

            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex2f(-ratX, -1.0f);
                glTexCoord2f(1, 1); glVertex2f( ratX, -1.0f);
                glTexCoord2f(1, 0); glVertex2f( ratX,  1.0f);
                glTexCoord2f(0, 0); glVertex2f(-ratX,  1.0f);
            glEnd();

            glPopMatrix(); // MODELVIEW
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);

            // Restore settings
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);


        }
        else if(currentSceneState == SCENE_RECOVERY)
        {
           glDisable(GL_DEPTH_TEST);    // Disable depth test to draw on top
            glDisable(GL_LIGHTING);      // No lighting for 2D texture

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();

            float ratX = dim.x / dim.y;
            gluOrtho2D(-ratX, ratX, -1.0, 1.0);  // Screen-aligned 2D view

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glBindTexture(GL_TEXTURE_2D, img_help);
            glColor3f(1.0f, 1.0f, 1.0f); // Ensure texture not tinted

            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex2f(-ratX, -1.0f);
                glTexCoord2f(1, 1); glVertex2f( ratX, -1.0f);
                glTexCoord2f(1, 0); glVertex2f( ratX,  1.0f);
                glTexCoord2f(0, 0); glVertex2f(-ratX,  1.0f);
            glEnd();

            glPopMatrix(); // MODELVIEW
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);

            // Restore settings
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);


        }

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
    else if (currentSceneState == SCENE_RECOVERY)
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
        else if (nextFailSpinTicks < globalTimer->getTicks())
        {
            nextFailSpinTicks = globalTimer->getTicks() + FAIL_SPIN_TIMER_DELAY;
            camera->rotateLeft();
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
        nextFailSpinTicks = 0;
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
    camera->reset();
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
                else if (currentSceneState == SCENE_RECOVERY)
                {
                    currentSceneState = SCENE_PAUSE;
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
            else if (wParam == 72) // H key
            {
                if (currentSceneState == SCENE_PAUSE)
                    currentSceneState = SCENE_RECOVERY;
            }
            else if(currentSceneState == SCENE_RUNNING)
            {
                if(wParam == 49 && availableResources >= TOWER_BASE_COST) // 1 on keyboard
                    isPlacingTower = !isPlacingTower;
            }


            myInputs->keyPressed(camera);


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
