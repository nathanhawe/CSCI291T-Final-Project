#ifndef _BASESCENE_H
#define _BASESCENE_H

#include <_common.h>
#include <_3dmodelloader.h>
#include <_timer.h>
#include <_textureloader.h>
#include <_sounds.h>

#define MAX_HITS 0
#define TOTAL_TOWERS 10
#define TOTAL_OBSTACLES 20
#define TOTAL_BULLETS 250

#define SOUND_COLLISION_FILE "sounds/Glass Windows Breaking.mp3"
#define SOUND_SUCCESS "sounds/Magic Chime.mp3"
#define SOUND_FAIL "sounds/Sad Trombone Wah Wah Wah Fail Sound - Sound Effect #35.mp3"
#define SOUND_LASER "sounds/laser.mp3"
#define SOUND_ELECTRIC "sounds/electric.mp3"
#define SOUND_BULLET "sounds/laser.mp3"
#define SOUND_EXPLOSION "sounds/Big Explosion Distant.mp3"

#define TOWER_BASE_COST 3
#define TOWER_BOMB_COST 2
#define TOWER_TESLA_COST 5

typedef struct enemyModel
{
    _3dmodelloader *model;
    _3dmodelloader *weapon;
    int targetEnemyIndex = -1;
    clock_t lastAttackTicks;
};

typedef struct tower
{
    bool isActive = false;
    int type;
    int health;
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;
    int targetEnemyIndex = -1;
    clock_t lastAttackTicks;
    bool hasFirstAttack;
};

typedef struct bullet
{
    bool isActive;
    float t;
    vec3 color;
    vec3 position;
    vec3 start;
    vec3 stop;
    clock_t lastMoveTicks;

};

class _baseScene
{
    public:

        _baseScene()
        {
            //ctor
            textureLoader = new _textureLoader();
        }

        ~_baseScene()
        {
            delete globalTimer;
            delete textureLoader;
            delete snds;
            delete backgroundMusic;
            delete laserSoundSource;
            delete electricSoundSource;
            delete explosionSoundSource;
        }

        void init()
        {
            tower_tex = textureLoader->loadImages("images/tower.jpg");
            roof_tex = textureLoader->loadImages("images/roof.jpg");
            ground_tex = textureLoader->loadImages("images/ground.jpg");
            dirt_tex = textureLoader->loadImages("images/dirt.jpg");

            snds->initSound();
            laserSoundSource = snds->loadSoundSource(SOUND_LASER);
            laserSoundSource->setDefaultVolume(0.30f);
            electricSoundSource = snds->loadSoundSource(SOUND_ELECTRIC);
            electricSoundSource->setDefaultVolume(0.30f);
            explosionSoundSource = snds->loadSoundSource(SOUND_EXPLOSION);
            explosionSoundSource->setDefaultVolume(0.50f);
        }

        GLvoid resizeWindow(GLsizei width, GLsizei height)
        {
            GLfloat Ratio = (GLfloat) width / (GLfloat) height;
            glViewport(0, 0, width, height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, Ratio, 0.1, 1000);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Capture new window dimensions after resize
            dim.x = width;
            dim.y = height;
        }

        GLvoid mouseMapping(int x, int y)
        {
            // dimension.x and dimension.y will give screen width and height
            GLint viewPort[4];              // to store view
            GLdouble modelViewMatrix[16];   // to store model view (camera times the model)
            GLdouble projectionMatrix[16];  // to store projection
            GLfloat winX, winY, winZ;       // to get dimensions

            glGetIntegerv(GL_VIEWPORT, viewPort);
            glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
            glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

            winX = (GLfloat)x;
            winY = (GLfloat) (viewPort[3] - y);
            glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

            gluUnProject(winX, winY, winZ, modelViewMatrix, projectionMatrix, viewPort, &mouseX, &mouseY, &mouseZ);

            //cout << "Mouse Mappings (x,y,z): " << mouseX << ", " << mouseY << ", " << mouseZ << endl;
        }

        /* COMMON DRAWING FUNCTIONS */

        void drawGround()
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

        void drawRoadHorizontal(float xStart, float xEnd, float z, float width)
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


        void drawRoadVertical(float zStart, float zEnd, float x, float width)
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

        void drawLasers(tower (&towers)[TOTAL_TOWERS], enemyModel (&obstacles)[TOTAL_OBSTACLES], clock_t laserDuration)
        {
            glDisable(GL_LIGHTING);
            glLineWidth(2.5f);
            glBegin(GL_LINES);
            for (int i = 0; i < TOTAL_TOWERS; i++) {
                // Don't draw lasers for bombs
                if (towers[i].type == 1) continue;

                if (towers[i].type == 0) {
                    glColor3f(1.0f, 0.0f, 0.0f); // Red for regular tower
                } else  {
                    glColor3f(0.2f, 0.5f, 0.9f);
                }

                int idx = towers[i].targetEnemyIndex;
                if (
                    idx == -1
                    || !towers[i].isActive
                    || obstacles[idx].model->pathStep < 0
                    || !towers[i].hasFirstAttack
                    || (towers[i].lastAttackTicks + laserDuration) > globalTimer->getTicks())
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

        void createTowerAtPoint(int towerType, float x, float z, tower (&towers)[TOTAL_TOWERS])
        {
            if(!isPlacingTower || !isTowerPlaceable) return;

            //find the first available tower slot
            for (int i = 0; i < TOTAL_TOWERS; i++)
            {
                if (towers[i].isActive) continue;

                towers[i].health = 4;
                towers[i].isActive = true;
                towers[i].type = towerType;

                if (towerType == 0)
                {
                    towers[i].xMin = x - 0.05;
                    towers[i].xMax = x + 0.05;
                    towers[i].yMin = 0;
                    towers[i].yMax = 0.15;
                    towers[i].zMin = z - 0.05;
                    towers[i].zMax = z + 0.05;
                    towers[i].targetEnemyIndex = -1;
                    towers[i].lastAttackTicks = globalTimer->getTicks();
                    towers[i].hasFirstAttack = false;

                    totalSpentResources += TOWER_BASE_COST;
                    availableResources -= TOWER_BASE_COST;
                }
                else if (towerType == 1)
                {
                    towers[i].xMin = x - 0.01;
                    towers[i].xMax = x + 0.01;
                    towers[i].yMin = 0;
                    towers[i].yMax = 0.02;
                    towers[i].zMin = z - 0.01;
                    towers[i].zMax = z + 0.01;
                    towers[i].targetEnemyIndex = -1;
                    towers[i].lastAttackTicks = globalTimer->getTicks();
                    towers[i].hasFirstAttack = false;

                    totalSpentResources += TOWER_BOMB_COST;
                    availableResources -= TOWER_BOMB_COST;
                }
                else
                {
                    towers[i].xMin = x - 0.05;
                    towers[i].xMax = x + 0.05;
                    towers[i].yMin = 0;
                    towers[i].yMax = 0.15;
                    towers[i].zMin = z - 0.05;
                    towers[i].zMax = z + 0.05;
                    towers[i].targetEnemyIndex = -1;
                    towers[i].lastAttackTicks = globalTimer->getTicks();
                    towers[i].hasFirstAttack = false;

                    totalSpentResources += TOWER_TESLA_COST;
                    availableResources -= TOWER_TESLA_COST;
                }


                isPlacingTower = false;

                debug();

                return;
            }

            cout << "*** No available towers! ***" << endl;
        }

        void drawTowerAt(float x, float y, float z, float width, float height)
        {
            glPushMatrix();

            glTranslatef(x, y, z);
            glScalef(width / 2.5f, height / 20.0f, width / 2.5f); // Scale

            glBindTexture(GL_TEXTURE_2D, tower_tex);

            glBegin(GL_QUADS);
            // Front
            glNormal3f(0, 0, 1);
            glTexCoord2f(0, 1); glVertex3f(-1.25f, 0.0f, 1.25f);
            glTexCoord2f(0, 0); glVertex3f(-1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 0); glVertex3f(1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 1); glVertex3f(1.25f, 0.0f, 1.25f);
            // Back
            glNormal3f(0, 0, -1);
            glTexCoord2f(0, 1); glVertex3f(-1.25f, 0.0f, -1.25f);
            glTexCoord2f(0, 0); glVertex3f(-1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 0); glVertex3f(1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 1); glVertex3f(1.25f, 0.0f, -1.25f);
            // Right
            glNormal3f(1, 0, 0);
            glTexCoord2f(0, 1); glVertex3f(1.25f, 0.0f, -1.25f);
            glTexCoord2f(0, 0); glVertex3f(1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 0); glVertex3f(1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 1); glVertex3f(1.25f, 0.0f, 1.25f);
            // Left
            glNormal3f(-1, 0, 0);
            glTexCoord2f(0, 1); glVertex3f(-1.25f, 0.0f, -1.25f);
            glTexCoord2f(0, 0); glVertex3f(-1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 0); glVertex3f(-1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 1); glVertex3f(-1.25f, 0.0f, 1.25f);
            glEnd();

            // Draw roof
            glBindTexture(GL_TEXTURE_2D, roof_tex);
            glBegin(GL_TRIANGLES);
            float apexY = 25.0f;
            float apexX = 0.0f, apexZ = 0.0f;

            glNormal3f(0, 0.5f, 0.5f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.25f, 20.0f, 1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.25f, 20.0f, 1.25f);

            glNormal3f(0.5f, 0.5f, 0.0f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.25f, 20.0f, 1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.25f, 20.0f, -1.25f);

            glNormal3f(0, 0.5f, -0.5f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.25f, 20.0f, -1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.25f, 20.0f, -1.25f);

            glNormal3f(-0.5f, 0.5f, 0.0f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.25f, 20.0f, -1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.25f, 20.0f, 1.25f);
            glEnd();

            glPopMatrix();
        }


        void drawBombAt(float x, float y, float z, float width, float height)
        {
            glPushMatrix();

            glTranslatef(x, y, z);
            glScalef(width / 2.5f, height / 20.0f, width / 2.5f); // Scale

            glBindTexture(GL_TEXTURE_2D, tower_tex);

            glBegin(GL_QUADS);
            // Front
            glNormal3f(0, 0, 1);
            glTexCoord2f(0, 1); glVertex3f(-1.25f, 0.0f, 1.25f);
            glTexCoord2f(0, 0); glVertex3f(-1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 0); glVertex3f(1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 1); glVertex3f(1.25f, 0.0f, 1.25f);
            // Back
            glNormal3f(0, 0, -1);
            glTexCoord2f(0, 1); glVertex3f(-1.25f, 0.0f, -1.25f);
            glTexCoord2f(0, 0); glVertex3f(-1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 0); glVertex3f(1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 1); glVertex3f(1.25f, 0.0f, -1.25f);
            // Right
            glNormal3f(1, 0, 0);
            glTexCoord2f(0, 1); glVertex3f(1.25f, 0.0f, -1.25f);
            glTexCoord2f(0, 0); glVertex3f(1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 0); glVertex3f(1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 1); glVertex3f(1.25f, 0.0f, 1.25f);
            // Left
            glNormal3f(-1, 0, 0);
            glTexCoord2f(0, 1); glVertex3f(-1.25f, 0.0f, -1.25f);
            glTexCoord2f(0, 0); glVertex3f(-1.25f, 20.0f, -1.25f);
            glTexCoord2f(1, 0); glVertex3f(-1.25f, 20.0f, 1.25f);
            glTexCoord2f(1, 1); glVertex3f(-1.25f, 0.0f, 1.25f);
            glEnd();

            // Draw roof
            glBindTexture(GL_TEXTURE_2D, roof_tex);
            glBegin(GL_TRIANGLES);
            float apexY = 25.0f;
            float apexX = 0.0f, apexZ = 0.0f;

            glNormal3f(0, 0.5f, 0.5f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.25f, 20.0f, 1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.25f, 20.0f, 1.25f);

            glNormal3f(0.5f, 0.5f, 0.0f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.25f, 20.0f, 1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.25f, 20.0f, -1.25f);

            glNormal3f(0, 0.5f, -0.5f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.25f, 20.0f, -1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.25f, 20.0f, -1.25f);

            glNormal3f(-0.5f, 0.5f, 0.0f);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(apexX, apexY, apexZ);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.25f, 20.0f, -1.25f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.25f, 20.0f, 1.25f);
            glEnd();

            glPopMatrix();
        }

        void drawTeslaTowerAt(float x, float y, float z, float width, float height)
        {
            glPushMatrix();

            // glDisable(GL_LIGHTING);

            glTranslatef(x, y, z);
            glScalef(width / 2.0f, height / 6.0f, width / 2.0f);

            // Base
            glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                drawCylinder(1.0f, 1.0f, 1.0f, 16);
            glPopMatrix();

            // Coil
            glTranslatef(0.0, 2.0, 0.0);
            glColor3f(0.83f, 0.69f, 0.22f);
            glPushMatrix();
                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                drawCylinder(0.3f, 0.3f, 3.0f, 16);
            glPopMatrix();

            // Top Sphere
            glTranslatef(0.0, 3.0, 0.0);
            glColor3f(0.9f, 0.91f, 0.98f);
            glutSolidSphere(0.6, 16, 16);

            //glEnable(GL_LIGHTING); // Re-enable if disabled

            glPopMatrix();
        }

        void drawCylinder(float baseRadius, float topRadius, float height, int slices)
        {
            GLUquadric* quad = gluNewQuadric();
            gluQuadricNormals(quad, GLU_SMOOTH);
            gluQuadricTexture(quad, GL_TRUE);

            gluCylinder(quad, baseRadius, topRadius, height, slices, 1);

            // Draw base cap
            glPushMatrix();
                glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
                gluDisk(quad, 0.0f, baseRadius, slices, 1);
            glPopMatrix();

            // Draw top cap
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, height);
                gluDisk(quad, 0.0f, topRadius, slices, 1);
            glPopMatrix();

            gluDeleteQuadric(quad);
        }


        /* Enemy Attacking Functions */
        void checkAndUpdateEnemyTargets(enemyModel (&obstacles)[TOTAL_OBSTACLES], tower (&towers)[TOTAL_TOWERS])
        {
            for (int i = 0; i < TOTAL_OBSTACLES; i++)
            {
                if (obstacles[i].model->pathStep < 0) continue;

                float ex = obstacles[i].model->pos.x;
                float ez = obstacles[i].model->pos.z;

                float closestDist = 9999.0f;
                int closestEnemy = -1;

                for (int j = 0; j < TOTAL_TOWERS; j++) {
                    if (!towers[j].isActive) continue;

                    float tx = (towers[j].xMin + towers[j].xMax) / 2.0f;
                    float tz = (towers[j].zMin + towers[j].zMax) / 2.0f;

                    float distSq = (ex - tx) * (ex - tx) + (ez - tz) * (ez - tz);
                    float range = 0.20f;

                    if (distSq <= range * range && distSq < closestDist) {
                        closestDist = distSq;
                        closestEnemy = j;
                    }
                }

                obstacles[i].targetEnemyIndex = closestEnemy;
            }
        }

        void checkBulletCollision(bullet (&bullets)[TOTAL_BULLETS], tower (&towers)[TOTAL_TOWERS])
        {
            for (int i = 0; i < TOTAL_BULLETS; i++)
            {
                if (!bullets[i].isActive) continue;

                for (int j = 0; j < TOTAL_TOWERS; j++)
                {
                    if (!towers[j].isActive) continue;

                    if (
                        bullets[i].position.x >= towers[j].xMin
                        && bullets[i].position.x <= towers[j].xMax
                        && bullets[i].position.y >= towers[j].yMin
                        && bullets[i].position.y <= towers[j].yMax
                        && bullets[i].position.z >= towers[j].zMin
                        && bullets[i].position.z <= towers[j].zMax
                        )
                    {
                        // Bullet has collided with a tower and causes damage.
                        bullets[i].isActive = false;
                        if(--towers[j].health <= 0)
                        {
                            // tower has been destroyed
                            towers[j].isActive = false;
                        }

                        cout << "Bullet #" << i << " collided with Tower #" << j << ". Current Tower Health: " << towers[j].health << " | " << towers[j].isActive << endl;
                    }
                }
            }
        }

        void advanceAndDrawBullets(bullet (&bullets)[TOTAL_BULLETS], clock_t bulletTimerDelay, float bulletSpeed)
        {
            for (int i = 0; i < TOTAL_BULLETS; i++)
            {
                if (!bullets[i].isActive
                    || bullets[i].lastMoveTicks + bulletTimerDelay < globalTimer->getTicks())
                    continue;

                // Advance bullet position
                bullets[i].lastMoveTicks = globalTimer->getTicks();
                bullets[i].t += bulletSpeed;
                bullets[i].position.x = bullets[i].start.x + bullets[i].t * (bullets[i].stop.x - bullets[i].start.x);
                bullets[i].position.y = bullets[i].start.y + bullets[i].t * (bullets[i].stop.y - bullets[i].start.y);
                bullets[i].position.z = bullets[i].start.z + bullets[i].t * (bullets[i].stop.z - bullets[i].start.z);


                // Draw bullet
                glPushMatrix();
                    glColor3f(bullets[i].color.x, bullets[i].color.y, bullets[i].color.z);
                    glTranslatef(bullets[i].position.x, bullets[i].position.y, bullets[i].position.z);
                    glutSolidSphere(0.015, 40, 40);

                    glColor3f(1, 1, 1);
                glPopMatrix();



            }
        }

        void attackEnemyTargets(enemyModel (&obstacles)[TOTAL_OBSTACLES], tower (&towers)[TOTAL_TOWERS], bullet (&bullets)[TOTAL_BULLETS], clock_t obstacleFireDelay)
        {
            clock_t nextFireTicks;
            int idx, bulletIdx;
            for (int i = 0; i < TOTAL_OBSTACLES; i++)
            {
                bulletIdx = -1;
                idx = obstacles[i].targetEnemyIndex;
                nextFireTicks = obstacles[i].lastAttackTicks + obstacleFireDelay;
                if (
                    idx == -1
                    || !towers[idx].isActive
                    || obstacles[i].model->pathStep < 0
                    || nextFireTicks > globalTimer->getTicks())
                    continue;

                obstacles[i].lastAttackTicks = globalTimer->getTicks();
                //snds->playSoundSource(laserSoundSource);

                for (int j = 0; j < TOTAL_BULLETS; j++)
                {
                    if (!bullets[j].isActive)
                    {
                        bullets[j].isActive = true;
                        bullets[j].lastMoveTicks = globalTimer->getTicks();
                        bullets[j].t = 0;

                        bullets[j].start.x = bullets[j].position.x = obstacles[i].model->pos.x;
                        bullets[j].start.y = bullets[j].position.y = obstacles[i].model->pos.y;
                        bullets[j].start.z = bullets[j].position.z = obstacles[i].model->pos.z;

                        bullets[j].stop.x = (towers[idx].xMin + towers[idx].xMax ) / 2.0;
                        bullets[j].stop.y = (towers[idx].yMin + towers[idx].yMax ) / 2.0;
                        bullets[j].stop.z = (towers[idx].zMin + towers[idx].zMax ) / 2.0;

                        bullets[j].color.x = obstacles[i].model->bulletColor.x;
                        bullets[j].color.y = obstacles[i].model->bulletColor.y;
                        bullets[j].color.z = obstacles[i].model->bulletColor.z;

                        cout << "Bullet #" << j << "R:" << bullets[j].color.x << ", G:" << bullets[j].color.y << ", B:" << bullets[j].color.z << " | ";
                        break;
                    }
                }

                cout << "ENEMY " << i << " --> T " << idx << endl;
            }
        }

        void debug()
        {
            //cout << "Wave Size: " << waveSize << ", Spawned: " << totalEnemiesSpawned << ", DMG: " << playerHitCount << ", Score: " << enemiesDefeatedCount;
            //cout << " Resources Available/Spent: " << availableResources << "/" << totalSpentResources << endl;
        }

        GLuint tower_tex, roof_tex, ground_tex, dirt_tex;

        _timer *globalTimer = new _timer();
        _textureLoader *textureLoader;

        virtual GLint IniGL() = 0;
        virtual GLvoid renderScene() = 0;
        virtual int winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

        enum {SCENE_START, SCENE_RUNNING, SCENE_COLLISION, SCENE_RECOVERY, SCENE_FAILURE, SCENE_VICTORY, SCENE_EXIT, SCENE_PAUSE, SCENE_TRANSITION} SceneState;
        int currentSceneState=0;

        GLdouble mouseX, mouseY, mouseZ;

        bool isPlacingTower = false;
        bool isTowerPlaceable = false;

        _sounds *snds = new _sounds();
        ISound *backgroundMusic;
        ISoundSource *laserSoundSource;
        ISoundSource *electricSoundSource;
        ISoundSource *explosionSoundSource;

    protected:
        vec2 dim;
        int playerHitCount = 0;
        int enemiesDefeatedCount = 0;
        int waveSize;
        int totalEnemiesSpawned;
        int availableResources;
        int totalSpentResources;


    private:

};

#endif // _BASESCENE_H
