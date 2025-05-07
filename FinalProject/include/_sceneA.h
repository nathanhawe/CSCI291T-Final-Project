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
#define TOWER_FIRE_DELAY 1500
#define LASER_DURATION 300



#define MUSIC_FILE "sounds/military-drums.mp3"
#define SOUND_COLLISION_FILE "sounds/Glass Windows Breaking.mp3"
#define SOUND_SUCCESS "sounds/Magic Chime.mp3"
#define SOUND_FAIL "sounds/Sad Trombone Wah Wah Wah Fail Sound - Sound Effect #35.mp3"
#define SOUND_LASER "sounds/laser.mp3"
#define SOUND_ELECTRIC "sounds/electric.mp3"



#define TOTAL_OBSTACLES 10
#define OBSTACLE_SPEED 0.00025


#define TOTAL_TOWERS 10
#define TOWER_BASE_COST 4
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

        void createTowerAtPoint(int towerType, float x, float z);
        void drawTowerAt(float x, float y, float z, float width, float height);
        void drawTeslaTowerAt(float x, float y, float z, float width, float height);
        void drawCylinder(float baseRadius, float topRadius, float height, int slices);
        void drawGround();
        void drawLasers();
        void checkAndUpdateTargets();
        void attackTargets();

        void drawOverlay();

        bool isPlacingTower = false;
        bool isTowerPlaceable = false;

        int returnToStateAfterPause;
        GLuint img_popup, img_defeat, img_victory, ground_tex, tower_tex, roof_tex, dirt_tex;
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
        _timer *globalTimer = new _timer();

        int spawnTimerDelayMs = 0;
        int spawnTimerDelayMinimumDuration = 3000;
        int spawnTimerDelayRange = 2500;

        _textureLoader *textureLoader = new _textureLoader();
        vec3 roadRotation;

        _camera *camera = new _camera();
        _skyBox *sky = new _skyBox();

        // Models
        _enemyFactory *enemyFactory = new _enemyFactory();

        _fort *fort1 = new _fort();



        struct enemyModel
        {
            _3dmodelloader *model;
            _3dmodelloader *weapon;
        };

        _3dmodelloader *collidedObstacle = nullptr;
        _3dmodelloader *collidedObstacleW = nullptr;

        enemyModel obstacles[TOTAL_OBSTACLES];

        int istowerType;

        // Towers
        struct tower
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

        tower towers[TOTAL_TOWERS];

};

#endif // _SCENEA_H
