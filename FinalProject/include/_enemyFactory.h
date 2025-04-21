#ifndef _ENEMYFACTORY_H
#define _ENEMYFACTORY_H

#define MODEL_TRIS_VEGETA "models/vegeta2/tris.md2"
#define WEAPON_TRIS_VEGETA "models/vegeta2/weapon.md2"

#include <_3dmodelloader.h>

class _enemyFactory
{
    public:
        _enemyFactory();
        virtual ~_enemyFactory();

        void GenerateVegeta(_3dmodelloader **mdl, _3dmodelloader **weaponMdl, int id);

    protected:
        void loadRandomSkins(_3dmodelloader *mdl, _3dmodelloader *weaponMdl);

    private:
};

#endif // _ENEMYFACTORY_H
