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

#define RECOVER_TIMER_MS 1000
#define RECOVER_TIMER_MS 1000
#define IMMUNITY_TIMER_MS 2000
#define VICTORY_TIMER_MS 30000
#define TRANSITION_TIMER_MS 5000

#define MUSIC_FILE "sounds/Skariachi - Inaban _ Nabani.mp3"
#define SOUND_COLLISION_FILE "sounds/Glass Windows Breaking.mp3"
#define SOUND_SUCCESS "sounds/Magic Chime.mp3"
#define SOUND_FAIL "sounds/Sad Trombone Wah Wah Wah Fail Sound - Sound Effect #35.mp3"


#define TOTAL_OBSTACLES 4
#define OBSTACLE_SPEED 0.00015

#define MODEL_TRIS_VEGETA "models/vegeta2/tris.md2"
#define MODEL_SKIN_VEGETA_BASE "models/vegeta2/vegeta.jpg"
#define MODEL_SKIN_VEGETA_BLACK "models/vegeta2/black.jpg"
#define MODEL_SKIN_VEGETA_BLUE "models/vegeta2/blue.jpg"
#define MODEL_SKIN_VEGETA_SS "models/vegeta2/ssvegeta.jpg"
#define WEAPON_TRIS_VEGETA "models/vegeta2/weapon.md2"
#define WEAPON_SKIN_VEGETA_BASE "models/vegeta2/weapon.jpg"
#define WEAPON_SKIN_VEGETA_SS "models/vegeta2/w_bfg.jpg"

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
        void loadVegetaModel(int id, char *filename, _3dmodelloader **mdl);
        void loadVegetaWeapon(int id, char *filename, _3dmodelloader **mdl);

        void drawRoadHorizontal(float xStart, float xEnd, float z, float width);
        void drawRoadVertical(float zStart, float zEnd, float x, float width);

        void advanceEnemies();

        bool isAutoScroll;
        int returnToStateAfterPause;
        GLuint img_popup, img_defeat, img_victory;

        _sounds *snds = new _sounds();
        ISound *backgroundMusic;

        _inputs *myInputs = new _inputs();

        _timer *victoryTimer = new _timer();
        _timer *immunityTimer = new _timer();
        _timer *spawnTimer = new _timer();
        _timer *transitionDelayTimer = new _timer();

        int spawnTimerDelayMs = 0;
        int spawnTimerDelayMinimumDuration = 1000;
        int spawnTimerDelayRange = 2500;

        _textureLoader *textureLoader = new _textureLoader();
        vec3 roadRotation;

        _camera *camera = new _camera();

        // Models
        struct enemyModel
        {
            _3dmodelloader *model;
            _3dmodelloader *weapon;
        };

        _3dmodelloader *collidedObstacle = nullptr;
        _3dmodelloader *collidedObstacleW = nullptr;

        enemyModel obstacles[TOTAL_OBSTACLES];

};

#endif // _SCENEA_H
