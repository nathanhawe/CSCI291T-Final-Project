#include "_enemyFactory.h"

_enemyFactory::_enemyFactory()
{
    //ctor
}

_enemyFactory::~_enemyFactory()
{
    //dtor
}

void _enemyFactory::GenerateVegeta(_3dmodelloader** mdl, _3dmodelloader** weaponMdl, int id = 0)
{
    *mdl = new _3dmodelloader();
    *weaponMdl = new _3dmodelloader();
    loadRandomSkins(*mdl, *weaponMdl);

    //(*mdl)->initModel(MODEL_TRIS_VEGETA, filename);

    (*mdl)->pos.y = 0.1;
    (*mdl)->pos.z = 3;

    (*mdl)->scale.x = (*mdl)->scale.y = (*mdl)->scale.z = 0.0035;

    (*mdl)->FaceRight();

    (*mdl)->maxHits = 3;
    (*mdl)->hitCount = 0;

    // Action frames
    (*mdl)->SetActionFrameRange((*mdl)->STAND, 0, 39);
    (*mdl)->SetActionFrameRange((*mdl)->RUN, 40, 45);
    (*mdl)->SetActionFrameRange((*mdl)->ATTACK, 46, 53);
    (*mdl)->SetActionFrameRange((*mdl)->PAIN, 54, 65);
    (*mdl)->SetActionFrameRange((*mdl)->DEATH, 178, 197);
    (*mdl)->SetActionFrameRange((*mdl)->TAUNT, 95, 111);
    (*mdl)->debugId = id;




    //(*weaponMdl)->initModel(WEAPON_TRIS_VEGETA, filename);

    // Action frames
    (*weaponMdl)->SetActionFrameRange((*mdl)->STAND, 0, 39);
    (*weaponMdl)->SetActionFrameRange((*mdl)->RUN, 40, 45);
    (*weaponMdl)->SetActionFrameRange((*mdl)->ATTACK, 46, 53);
    (*weaponMdl)->SetActionFrameRange((*mdl)->PAIN, 54, 65);
    (*weaponMdl)->SetActionFrameRange((*mdl)->DEATH, 178, 197);
    (*weaponMdl)->SetActionFrameRange((*mdl)->TAUNT, 95, 111);
    (*weaponMdl)->debugId = 100 + id;
}

void _enemyFactory::loadRandomSkins(_3dmodelloader *mdl, _3dmodelloader *weaponMdl)
{

    switch(rand() % 4)
    {
        case 0: mdl->initModel(MODEL_TRIS_VEGETA, "models/vegeta2/black.jpg"); weaponMdl->initModel(WEAPON_TRIS_VEGETA, "models/vegeta2/weapon.jpg"); break;
        case 1: mdl->initModel(MODEL_TRIS_VEGETA, "models/vegeta2/blue.jpg"); weaponMdl->initModel(WEAPON_TRIS_VEGETA, "models/vegeta2/weapon.jpg"); break;
        case 2: mdl->initModel(MODEL_TRIS_VEGETA, "models/vegeta2/ssvegeta.jpg"); weaponMdl->initModel(WEAPON_TRIS_VEGETA, "models/vegeta2/w_bfg.jpg"); break;
        case 3: mdl->initModel(MODEL_TRIS_VEGETA, "models/vegeta2/vegeta.jpg"); weaponMdl->initModel(WEAPON_TRIS_VEGETA, "models/vegeta2/weapon.jpg"); break;
    }
}
