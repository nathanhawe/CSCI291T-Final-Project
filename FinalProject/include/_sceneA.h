#ifndef _SCENEA_H
#define _SCENEA_H

#include<_baseScene.h>
#include<_common.h>
#include<_inputs.h>
#include<_timer.h>
#include<_camera.h>
#include<_parallax.h>
#include<_textureloader.h>
#include<_3dmodelloader.h>
#include<_sounds.h>
#include<_enemyFactory.h>
#include<_skybox.h>
#include<_objLoader.h>
#include<_fort.h>


#define WAVE_SIZE 10

#define RECOVER_TIMER_MS 1000
#define RECOVER_TIMER_MS 1000
#define IMMUNITY_TIMER_MS 2000
#define VICTORY_TIMER_MS 30000
#define TRANSITION_TIMER_MS 5000
#define TOWER_FIRE_DELAY 750
#define LASER_DURATION 300



#define MUSIC_FILE "sounds/military-drums.mp3"

#define OBSTACLE_SPEED 0.01

#define OBSTACLE_TIMER_DELAY 30
#define OBSTACLE_FIRE_DELAY 3000

#define BULLET_SPEED 0.003
#define BULLET_TIMER_DELAY 30

class _sceneA: public _baseScene
{
    public:
        _sceneA();
        virtual ~_sceneA();


        GLint IniGL();                                    //initialization
        GLvoid renderScene();                              // Draw Scene

        int winMsg(
            HWND	hWnd,			// Handle For This Window
            UINT	uMsg,			// Message For This Window
            WPARAM  wParam,			// Additional Message Information
            LPARAM	lParam
        );

        void reset();

        vec2 dim;

    protected:

    private:
        void transitionSceneState();
        void spawnObstacles();
        _3dmodelloader* getAvailableObstacleModel();
        bool hasCollided();

        void drawRoadHorizontal(float xStart, float xEnd, float z, float width);
        void drawRoadVertical(float zStart, float zEnd, float x, float width);
        void drawPlacementCircle(float towerSize, float towerRange);

        void advanceEnemies();
        //void checkAndUpdateEnemyTargets();
        //void attackEnemyTargets();

        //void createTowerAtPoint(int towerType, float x, float z);
        //void drawTowerAt(float x, float y, float z, float width, float height);
        //void drawTeslaTowerAt(float x, float y, float z, float width, float height);
        //void drawCylinder(float baseRadius, float topRadius, float height, int slices);
        void drawGround();
        void drawLasers();
        void checkAndUpdateTargets();
        void attackTargets();

        //void advanceAndDrawBullets();
        //void checkBulletCollision();

        void drawOverlay();

        int returnToStateAfterPause;
        GLuint img_popup, img_defeat, img_victory, ground_tex, dirt_tex;
        GLuint overlay1_notReady, overlay1_ready, overlay1_selected;
        GLuint overlay2_disabled;
        GLuint overlay3_disabled;


        _sounds *snds = new _sounds();
        ISound *backgroundMusic;
        ISoundSource *laserSoundSource;
        ISoundSource *electricSoundSource;

        _inputs *myInputs = new _inputs();

        _timer *victoryTimer = new _timer();
        _timer *immunityTimer = new _timer();
        _timer *spawnTimer = new _timer();
        _timer *transitionDelayTimer = new _timer();

        int spawnTimerDelayMs = 0;
        int spawnTimerDelayMinimumDuration = 3000;
        int spawnTimerDelayRange = 2500;

        vec3 roadRotation;

        _camera *camera = new _camera();
        _skyBox *sky = new _skyBox();

        // Models
        _enemyFactory *enemyFactory = new _enemyFactory();

        _fort *fort1 = new _fort();


        _3dmodelloader *collidedObstacle = nullptr;
        _3dmodelloader *collidedObstacleW = nullptr;

        enemyModel obstacles[TOTAL_OBSTACLES];

        int istowerType;

        // Towers
        tower towers[TOTAL_TOWERS];

        // Enemy attacks
        bullet bullets[TOTAL_BULLETS];

};

#endif // _SCENEA_H
