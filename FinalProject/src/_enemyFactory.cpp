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

void _enemyFactory::GenerateTekk(_3dmodelloader** mdl, _3dmodelloader** weaponMdl, int id)
{
    *mdl = new _3dmodelloader();
    *weaponMdl = new _3dmodelloader();

    //if (id == 0) (*mdl)->printFrames = true;

    loadRandomTekkSkins(*mdl, *weaponMdl);

    (*mdl)->pos.y = 0.1;
    (*mdl)->pos.z = 3;

    (*mdl)->scale.x = (*mdl)->scale.y = (*mdl)->scale.z = 0.0025;

    (*mdl)->FaceRight();

    (*mdl)->maxHits = 2;
    (*mdl)->hitCount = 0;

    // Action frames
    (*mdl)->SetActionFrameRange((*mdl)->STAND, 0, 39);
    (*mdl)->SetActionFrameRange((*mdl)->RUN, 40, 45);
    (*mdl)->SetActionFrameRange((*mdl)->ATTACK, 46, 53);
    (*mdl)->SetActionFrameRange((*mdl)->PAIN, 54, 65);
    (*mdl)->SetActionFrameRange((*mdl)->DEATH, 178, 183);
    (*mdl)->SetActionFrameRange((*mdl)->TAUNT, 95, 111);
    (*mdl)->debugId = id;


    // Action frames
    (*weaponMdl)->SetActionFrameRange((*mdl)->STAND, 0, 39);
    (*weaponMdl)->SetActionFrameRange((*mdl)->RUN, 40, 45);
    (*weaponMdl)->SetActionFrameRange((*mdl)->ATTACK, 46, 53);
    (*weaponMdl)->SetActionFrameRange((*mdl)->PAIN, 54, 65);
    (*weaponMdl)->SetActionFrameRange((*mdl)->DEATH, 178, 183);
    (*weaponMdl)->SetActionFrameRange((*mdl)->TAUNT, 95, 111);
    (*weaponMdl)->debugId = 100 + id;
}

void _enemyFactory::loadRandomTekkSkins(_3dmodelloader* mdl, _3dmodelloader* weaponMdl)
{
    switch(rand() % 9)
    {
        case 0:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade.jpg");
            mdl->bulletColor.x = 217/255.0;
            mdl->bulletColor.y = 165/255.0;
            mdl->bulletColor.z = 0;
            break;

        case 1:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_black.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_black.jpg");
            mdl->bulletColor.x = 0;
            mdl->bulletColor.y = 0;
            mdl->bulletColor.z = 100/255.0;
            break;

        case 2:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_blue.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_blue.jpg");
            mdl->bulletColor.x = 0;
            mdl->bulletColor.y = 0;
            mdl->bulletColor.z = 200/255.0;
            break;

        case 3:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_blue2.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_blue2.jpg");
            mdl->bulletColor.x = 0;
            mdl->bulletColor.y = 0;
            mdl->bulletColor.z = 255/255.0;
            break;

        case 4:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_brown.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_brown.jpg");
            mdl->bulletColor.x = 255/255.0;
            mdl->bulletColor.y = 247/255.0;
            mdl->bulletColor.z = 218/255.0;
            break;

        case 5:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_brown2.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_brown2.jpg");
            mdl->bulletColor.x = 150/255.0;
            mdl->bulletColor.y = 93/255.0;
            mdl->bulletColor.z = 0;
            break;

        case 6:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_green.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_green.jpg");
            mdl->bulletColor.x = 0;
            mdl->bulletColor.y = 200/255.0;
            mdl->bulletColor.z = 0;
            break;

        case 7:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_green2.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_green2.jpg");
            mdl->bulletColor.x = 0;
            mdl->bulletColor.y = 255/255.0;
            mdl->bulletColor.z = 0;
            break;

        case 8:
            mdl->initModel(MODEL_TRIS_TEKK, "models/Tekk/blade_yellow.jpg"); weaponMdl->initModel(WEAPON_TRIS_TEKK, "models/Tekk/blade_yellow.jpg");
            mdl->bulletColor.x = 217/255.0;
            mdl->bulletColor.y = 221/255.0;
            mdl->bulletColor.z = 0;
            break;

    }


}
