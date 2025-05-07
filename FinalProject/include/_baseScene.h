#ifndef _BASESCENE_H
#define _BASESCENE_H

#include <_common.h>
#include <_3dmodelloader.h>
#include <_timer.h>

#define MAX_HITS 1
#define TOTAL_TOWERS 10
#define TOTAL_OBSTACLES 10
#define TOTAL_BULLETS 250

#define SOUND_COLLISION_FILE "sounds/Glass Windows Breaking.mp3"
#define SOUND_SUCCESS "sounds/Magic Chime.mp3"
#define SOUND_FAIL "sounds/Sad Trombone Wah Wah Wah Fail Sound - Sound Effect #35.mp3"
#define SOUND_LASER "sounds/laser.mp3"
#define SOUND_ELECTRIC "sounds/electric.mp3"
#define SOUND_BULLET "sounds/laser.mp3"

typedef struct enemyModel
{
    _3dmodelloader *model;
    _3dmodelloader *weapon;
    int targetEnemyIndex = -1;
    clock_t lastAttackTicks;
};

typedef struct tower
{
    bool isActive;
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
        }

        ~_baseScene()
        {
            delete globalTimer;
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


        _timer *globalTimer = new _timer();

        virtual GLint IniGL() = 0;
        virtual GLvoid renderScene() = 0;
        virtual int winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

        enum {SCENE_START, SCENE_RUNNING, SCENE_COLLISION, SCENE_RECOVERY, SCENE_FAILURE, SCENE_VICTORY, SCENE_EXIT, SCENE_PAUSE, SCENE_TRANSITION} SceneState;
        int currentSceneState=0;

        GLdouble mouseX, mouseY, mouseZ;

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
