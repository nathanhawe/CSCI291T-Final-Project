#ifndef _SCENEC_H
#define _SCENEC_H

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

#define WAVE_SIZE 20

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

class _sceneC: public _baseScene
{
    public:
        _sceneC();
        virtual ~_sceneC();


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

        void drawPlacementCircle(float towerSize, float towerRange);

        void advanceEnemies();

        void checkAndUpdateTargets();
        void attackTargets();

        void drawOverlay();

        int placingTowerType;

        int returnToStateAfterPause;
        GLuint img_popup, img_defeat, img_victory, dirt_tex;
        GLuint overlay1_notReady, overlay1_ready, overlay1_selected;
        GLuint overlay2_notReady, overlay2_ready, overlay2_selected;
        GLuint overlay3_disabled;

        _inputs *myInputs = new _inputs();

        _timer *victoryTimer = new _timer();
        _timer *immunityTimer = new _timer();
        _timer *spawnTimer = new _timer();
        _timer *transitionDelayTimer = new _timer();

        int spawnTimerDelayMs = 0;
        int spawnTimerDelayMinimumDuration = 2000;
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

        // Towers
        tower towers[TOTAL_TOWERS];

        // Enemy attacks
        bullet bullets[TOTAL_BULLETS];

};

#endif // _SCENEC_H
