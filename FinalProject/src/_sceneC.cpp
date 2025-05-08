#include "_sceneC.h"


_sceneC::_sceneC()
{
    //ctor

}

_sceneC::~_sceneC()
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


GLint _sceneC::IniGL()
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

    // Load overlay images
    overlay1_notReady = textureLoader->loadImages("images/overlay/1-not_ready.png");
    overlay1_ready = textureLoader->loadImages("images/overlay/1-ready.png");
    overlay1_selected = textureLoader->loadImages("images/overlay/1-selected.png");
    overlay2_notReady = textureLoader->loadImages("images/overlay/2-not_ready.png");
    overlay2_ready = textureLoader->loadImages("images/overlay/2-ready.png");
    overlay2_selected = textureLoader->loadImages("images/overlay/2-selected.png");
    overlay3_notReady = textureLoader->loadImages("images/overlay/3-not_ready.png");
    overlay3_ready = textureLoader->loadImages("images/overlay/3-ready.png");
    overlay3_selected = textureLoader->loadImages("images/overlay/3-selected.png");



    // Start background music
    sky->skyBoxInit();
    //fort1->initFortassets();
    backgroundMusic = snds->playMusic(MUSIC_FILE);
    backgroundMusic->setVolume(0.15f);

    // Setup game state
    waveSize = WAVE_SIZE;
    totalEnemiesSpawned = 0;
    enemiesDefeatedCount = 0;
    availableResources = TOWER_BASE_COST * 2 + (2 * TOWER_BOMB_COST) + TOWER_TESLA_COST;
    totalSpentResources = -2 * TOWER_BOMB_COST - TOWER_TESLA_COST;

    // Load demo bombs
    isTowerPlaceable = true;
    isPlacingTower = true; createTowerAtPoint(1, 0, -0.50, towers);
    isPlacingTower = true; createTowerAtPoint(1, 0, -0.40, towers);
    isPlacingTower = true; createTowerAtPoint(2, 0, 0, towers);

    return true;
}


GLvoid _sceneC::renderScene()
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


        drawRoadHorizontal(-0.40,  0.40, -0.25, 0.1);

        drawRoadHorizontal(-2.00, -0.35,  0.25, 0.1);
        drawRoadHorizontal( 0.35,  2.00,  0.25, 0.1);

        drawRoadHorizontal(-2.00, -0.15,  0.60, 0.1);
        drawRoadHorizontal( 0.15,  2.00,  0.60, 0.1);

        drawRoadVertical(-1.00, 0.25, -1.30, 0.1);
        drawRoadVertical(-1.00, 0.25,  1.30, 0.1);

        drawRoadVertical(0.60, 1.00, -0.60, 0.1);
        drawRoadVertical(0.60, 1.00,  0.60, 0.1);

        drawRoadVertical(0.30, -0.25, -0.35, 0.1);
        drawRoadVertical(0.30, -0.25,  0.35, 0.1);

        drawRoadVertical(0.65, -0.25, -0.15, 0.1);
        drawRoadVertical(0.65, -0.25,  0.15, 0.1);

        drawRoadVertical(-0.25, -0.75, 0, 0.1);

        //glColor3f(1, 1, 1);


        // Advance enemies
        if(currentSceneState == SCENE_RUNNING)
        {
            advanceEnemies();
            if(placingTowerType == 2)
            {
                drawPlacementCircle(0.05, 0.27);
            }
            else drawPlacementCircle(0.05, 0.18);
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

            if (towers[i].type == 0)
            {
                // Calculate tower center position
                float posX = (towers[i].xMin + towers[i].xMax) / 2.0f;
                float posY = towers[i].yMin; // base Y
                float posZ = (towers[i].zMin + towers[i].zMax) / 2.0f;

                // Calculate size (used for scaling)
                float towerWidth = towers[i].xMax - towers[i].xMin;
                float towerHeight = towers[i].yMax - towers[i].yMin;

                drawTowerAt(posX, posY, posZ, towerWidth, towerHeight);
            }
            else if (towers[i].type == 1)
            {
                // Calculate tower center position
                float posX = (towers[i].xMin + towers[i].xMax) / 2.0f;
                float posY = towers[i].yMin; // base Y
                float posZ = (towers[i].zMin + towers[i].zMax) / 2.0f;

                // Calculate size (used for scaling)
                float towerWidth = towers[i].xMax - towers[i].xMin;
                float towerHeight = towers[i].yMax - towers[i].yMin;

                drawBombAt(posX, posY, posZ, towerWidth, towerHeight);
            }
            else if (towers[i].type == 2)
            {
                // Calculate tower center position
                float posX = (towers[i].xMin + towers[i].xMax) / 2.0f;
                float posY = towers[i].yMin; // base Y
                float posZ = (towers[i].zMin + towers[i].zMax) / 2.0f;

                // Calculate size (used for scaling)
                float towerWidth = towers[i].xMax - towers[i].xMin;
                float towerHeight = towers[i].yMax - towers[i].yMin;

                drawTeslaTowerAt(posX, posY, posZ, towerWidth, towerHeight);
            }
        }

    checkAndUpdateEnemyTargets(obstacles, towers);
    attackEnemyTargets(obstacles, towers, bullets, OBSTACLE_FIRE_DELAY);
    checkAndUpdateTargets();
    attackTargets();
    drawLasers(towers, obstacles, LASER_DURATION);


    glColor3f(1, 1, 1);

}

void _sceneC::drawOverlay()
{

    if(isPlacingTower && placingTowerType == 0)
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

    if(isPlacingTower && placingTowerType == 1)
    {
        glBindTexture(GL_TEXTURE_2D, overlay2_selected);
    }
    else if (availableResources >= TOWER_BOMB_COST)
    {
        glBindTexture(GL_TEXTURE_2D, overlay2_ready);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, overlay2_notReady);
    }
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-0.075, 1.00, -0.5);
        glTexCoord2f(0, 1); glVertex3f(-0.075, 0.95, 0);
        glTexCoord2f(1, 1); glVertex3f(0.075, 0.95, 0);
        glTexCoord2f(1, 0); glVertex3f(0.075, 1.00, -0.5);
    glEnd();

     if(isPlacingTower && placingTowerType == 2)
    {
        glBindTexture(GL_TEXTURE_2D, overlay3_selected);
    }
    else if (availableResources >= TOWER_TESLA_COST)
    {
        glBindTexture(GL_TEXTURE_2D, overlay3_ready);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, overlay3_notReady);
    }
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(0.2, 1.00, -0.5);
        glTexCoord2f(0, 1); glVertex3f(0.2, 0.95, 0);
        glTexCoord2f(1, 1); glVertex3f(0.35, 0.95, 0);
        glTexCoord2f(1, 0); glVertex3f(0.35, 1.00, -0.5);
    glEnd();

}

void _sceneC::checkAndUpdateTargets()
{
    for (int i = 0; i < TOTAL_TOWERS; i++) {
        if (!towers[i].isActive) continue;

        float tx = (towers[i].xMin + towers[i].xMax) / 2.0f;
        float ty = (towers[i].yMin + towers[i].yMax) / 2.0f;
        float tz = (towers[i].zMin + towers[i].zMax) / 2.0f;

        float closestDist = 9999.0f;
        int closestEnemy = -1;

        if(towers[i].type == 1)
            closestDist = 0.05;

        for (int j = 0; j < TOTAL_OBSTACLES; j++) {
            if (obstacles[j].model->pathStep < 0) continue;

            float ex = obstacles[j].model->pos.x;
            float ez = obstacles[j].model->pos.z;

            float distSq = (ex - tx) * (ex - tx) + (ez - tz) * (ez - tz);
            float range = towers[i].type == 0 ? 0.25f : 0.375f;

            if (distSq <= range * range && distSq < closestDist) {
                closestDist = distSq;
                closestEnemy = j;
            }
        }

        towers[i].targetEnemyIndex = closestEnemy;
    }
}

void _sceneC::attackTargets()
{
    clock_t nextFireTicks;
    int idx;
    for (int i = 0; i < TOTAL_TOWERS; i++)
    {
        idx = towers[i].targetEnemyIndex;
        nextFireTicks = towers[i].lastAttackTicks + (towers[i].type == 2 ? 2.5 * TOWER_FIRE_DELAY : TOWER_FIRE_DELAY);
        if (
            idx == -1
            || !towers[i].isActive
            || obstacles[idx].model->pathStep < 0
            || nextFireTicks > globalTimer->getTicks())
            continue;

        towers[i].lastAttackTicks = globalTimer->getTicks();
        towers[i].hasFirstAttack = true;
        if (towers[i].type == 0){
            snds->playSoundSource(laserSoundSource);
        }
        else if (towers[i].type == 1)
        {
            snds->playSoundSource(explosionSoundSource);
        }
        else {
            snds->playSoundSource(electricSoundSource);
        }

        cout << "TOWER " << i << " --> " << idx;

        // Add damage to enemy
        if (towers[i].type == 1)
        {
            // Detonate bomb killing a single enemy and removing the tower
            obstacles[idx].model->hitCount += 10;
            towers[i].isActive = false;

        }
        else obstacles[idx].model->hitCount++;

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


void _sceneC::drawPlacementCircle(float towerSize, float towerRange)
{
    if (!isPlacingTower) return;
    // Is the mouse within the bounds?
    if (
        mouseX < -2 || mouseX > 2 ||
        mouseZ < -1 || mouseZ > 1)
        return;

    if (
        mouseX >= -0.40 - towerSize && mouseX <= 0.40 + towerSize &&
        mouseZ >= -0.25 - towerSize && mouseZ <= -0.25 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -2.00 - towerSize && mouseX <= -0.35 + towerSize &&
        mouseZ >=  0.25 - towerSize && mouseZ <=  0.25 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  0.35 - towerSize && mouseX <=  2.00 + towerSize &&
        mouseZ >=  0.25 - towerSize && mouseZ <=  0.25 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -2.00 - towerSize && mouseX <= -0.15 + towerSize &&
        mouseZ >=  0.60 - towerSize && mouseZ <=  0.60 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  0.15 - towerSize && mouseX <=  2.00 + towerSize &&
        mouseZ >=  0.60 - towerSize && mouseZ <=  0.60 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -1.30 - towerSize && mouseX <= -1.30 + towerSize &&
        mouseZ >= -1.00 - towerSize && mouseZ <=  0.25 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  1.30 - towerSize && mouseX <=  1.30 + towerSize &&
        mouseZ >= -1.00 - towerSize && mouseZ <=  0.25 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -0.60 - towerSize && mouseX <= -0.60 + towerSize &&
        mouseZ >=  0.60 - towerSize && mouseZ <=  1.00 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  0.60 - towerSize && mouseX <=  0.60 + towerSize &&
        mouseZ >=  0.60 - towerSize && mouseZ <=  1.00 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -0.35 - towerSize && mouseX <= -0.35 + towerSize &&
        mouseZ >= -0.25 - towerSize && mouseZ <=  0.30 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  0.35 - towerSize && mouseX <=  0.35 + towerSize &&
        mouseZ >= -0.25 - towerSize && mouseZ <=  0.30 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >= -0.15 - towerSize && mouseX <= -0.15 + towerSize &&
        mouseZ >= -0.25 - towerSize && mouseZ <=  0.65 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  0.15 - towerSize && mouseX <=  0.15 + towerSize &&
        mouseZ >= -0.25 - towerSize && mouseZ <=  0.65 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else if (
        mouseX >=  0 - towerSize && mouseX <=  0 + towerSize &&
        mouseZ >= -0.75 - towerSize && mouseZ <= -0.25 + towerSize)
    {
        isTowerPlaceable = false;
    }
    else isTowerPlaceable = true;

    // TODO: Add check of existing towers as well.

    // Bombs are only placeable on roads.
    if (placingTowerType == 1) isTowerPlaceable = !isTowerPlaceable;

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


void _sceneC::advanceEnemies()
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
                    obstacles[i].model->FaceDown();
                    obstacles[i].model->pos.z += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z >= 0.25)
                        obstacles[i].model->pathStep = 1;

                    break;

                case 1:
                    obstacles[i].model->FaceRight();
                    obstacles[i].model->pos.x += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x >= -0.35)
                        obstacles[i].model->pathStep = 2;

                    break;

                case 2:
                    obstacles[i].model->FaceUp();
                    obstacles[i].model->pos.z -= OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z <= -0.25)
                        obstacles[i].model->pathStep = 3;

                    break;
                case 3:
                    obstacles[i].model->FaceRight();
                    obstacles[i].model->pos.x += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x >= 0)
                        obstacles[i].model->pathStep = 4;

                    break;

                case 4:
                    obstacles[i].model->FaceUp();
                    obstacles[i].model->pos.z -= OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z <= -0.75)
                    {
                        obstacles[i].model->pathStep = -1;

                        playerHitCount++;
                        debug();
                    }
            }
        }
        else if(obstacles[i].model->path == 1)
        {
            obstacles[i].model->path = 0;
            obstacles[i].model->pathStep = 1;
        }
        else if(obstacles[i].model->path == 2)
        {
            if (obstacles[i].model->pos.x >= -0.15)
            {
                obstacles[i].model->path = 0;
                obstacles[i].model->pathStep = 2;
            }
            else
            {
                obstacles[i].model->FaceRight();
                obstacles[i].model->pos.x += OBSTACLE_SPEED;
            }
        }
        else if(obstacles[i].model->path == 3)
        {
            if (obstacles[i].model->pos.z <= 0.6)
            {
                obstacles[i].model->path = 2;
                obstacles[i].model->pathStep = 0;
            }
            else
            {
                obstacles[i].model->FaceUp();
                obstacles[i].model->pos.z -= OBSTACLE_SPEED;
            }
        }
        else if(obstacles[i].model->path == 4)
        {
            if (obstacles[i].model->pos.z <= 0.6)
            {
                obstacles[i].model->path = 5;
                obstacles[i].model->pathStep = 0;
            }
            else
            {
                obstacles[i].model->FaceUp();
                obstacles[i].model->pos.z -= OBSTACLE_SPEED;
            }
        }
        else if(obstacles[i].model->path == 5)
        {
            if (obstacles[i].model->pos.x <= 0.15)
            {
                obstacles[i].model->path = 7;
                obstacles[i].model->pathStep = 2;
            }
            else
            {
                obstacles[i].model->FaceLeft();
                obstacles[i].model->pos.x -= OBSTACLE_SPEED;
            }
        }
        else if(obstacles[i].model->path == 6)
        {
            obstacles[i].model->path = 7;
            obstacles[i].model->pathStep = 1;
        }
        else if(obstacles[i].model->path == 7)
        {
            switch(obstacles[i].model->pathStep)
            {
                case 0:
                    obstacles[i].model->FaceDown();
                    obstacles[i].model->pos.z += OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z >= 0.25)
                        obstacles[i].model->pathStep = 1;

                    break;

                case 1:
                    obstacles[i].model->FaceLeft();
                    obstacles[i].model->pos.x -= OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x <= 0.35)
                        obstacles[i].model->pathStep = 2;

                    break;

                case 2:
                    obstacles[i].model->FaceUp();
                    obstacles[i].model->pos.z -= OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.z <= -0.25)
                        obstacles[i].model->pathStep = 3;

                    break;

                case 3:
                    obstacles[i].model->FaceLeft();
                    obstacles[i].model->pos.x -= OBSTACLE_SPEED;

                    if(obstacles[i].model->pos.x <= 0)
                    {
                        obstacles[i].model->path = 0;
                        obstacles[i].model->pathStep = 4;
                    }

                    break;
            }
        }
    }
}



void _sceneC::transitionSceneState()
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

void _sceneC::spawnObstacles()
{
    // Check if we're within timer range and haven't maxed out level spawns
    if(totalEnemiesSpawned < WAVE_SIZE && spawnTimer->getTicks() >= spawnTimerDelayMs)
    {
        // A new obstacle can be spawned
        int spawnLocation = (rand() % 80);


        _3dmodelloader *obj1 = getAvailableObstacleModel();
        if(obj1 == nullptr)
        {
            cout << "*** No obstacles available ***" << endl;
        }
        else
        {

            if (spawnLocation < 10)
            {
                obj1->pos.x = -1.3;
                obj1->pos.z = -1.0;
                obj1->path = 0;
            }
            else if (spawnLocation < 20)
            {
                obj1->pos.x = -2.0;
                obj1->pos.z = 0.25;
                obj1->path = 1;
            }
            else if (spawnLocation < 30)
            {
                obj1->pos.x = -2.0;
                obj1->pos.z = 0.6;
                obj1->path = 2;
            }
            else if (spawnLocation < 40)
            {
                obj1->pos.x = -0.6;
                obj1->pos.z = 1;
                obj1->path = 3;
            }
            else if (spawnLocation < 50)
            {
                obj1->pos.x = 0.6;
                obj1->pos.z = 1;
                obj1->path = 4;
            }
            else if (spawnLocation < 60)
            {
                obj1->pos.x = 2.0;
                obj1->pos.z = 0.6;
                obj1->path = 5;
            }
            else if (spawnLocation < 70)
            {
                obj1->pos.x = 2.0;
                obj1->pos.z = 0.25;
                obj1->path = 6;
            }
            else
            {
                obj1->pos.x = 1.3;
                obj1->pos.z = -1.0;
                obj1->path = 7;
            }

            obj1->pathStep = 0;
            obj1->hitCount = 0;

            totalEnemiesSpawned++;

            cout << "Obstacle #" << totalEnemiesSpawned << ": " << obj1->debugId << " at spawn location " << obj1->path << endl;

        }

        // Choose new spawn delay interval and reset timer
        spawnTimer->reset();
        spawnTimerDelayMs = (rand() % spawnTimerDelayRange) + spawnTimerDelayMinimumDuration;

        cout << "New Delay: " << spawnTimerDelayMs << endl;
    }
}

_3dmodelloader* _sceneC::getAvailableObstacleModel()
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

bool _sceneC::hasCollided()
{

    return false;
}

void _sceneC::reset()
{
    waveSize = WAVE_SIZE;
    enemiesDefeatedCount = 0;
    totalEnemiesSpawned = 0;
    playerHitCount = 0;
    availableResources = TOWER_BASE_COST * 2 + (2 * TOWER_BOMB_COST) + TOWER_TESLA_COST;
    totalSpentResources = -2 * TOWER_BOMB_COST - TOWER_TESLA_COST;

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

    // Load demo bombs
    isTowerPlaceable = true;
    isPlacingTower = true; createTowerAtPoint(1, 0, -0.50, towers);
    isPlacingTower = true; createTowerAtPoint(1, 0, -0.40, towers);
    isPlacingTower = true; createTowerAtPoint(2, 0, 0, towers);
}


int _sceneC::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
                {
                    isPlacingTower = true;
                    placingTowerType = 0;
                }

                if(wParam == 50 && availableResources >= TOWER_BOMB_COST) // 2 on keyboard
                {
                    isPlacingTower = true;
                    placingTowerType = 1;
                }

                if(wParam == 51 && availableResources >= TOWER_TESLA_COST) // 3 on keyboard
                {
                    isPlacingTower = true;
                    placingTowerType = 2;
                }
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
                createTowerAtPoint(placingTowerType, mouseX, mouseZ, towers);

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
