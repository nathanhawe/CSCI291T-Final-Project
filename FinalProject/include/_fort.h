#ifndef _FORT_H
#define _FORT_H




#include<_textureloader.h>


class _fort
{
    public:
        _fort();
        virtual ~_fort();

        void drawfort();
        void initFortassets();
        void drawTree(float tr_x, float tr_y, float tr_z);

    protected:

    private:

        GLuint texid1,texid2,texid3,texid4,texid5,texid6,texid7,texid8,texid9,texid10;
        GLuint texid11,texid12,texid13,texid14,texid15,texid16;

        _textureLoader *textureLoader1 = new _textureLoader();


};

#endif // _FORT_H
