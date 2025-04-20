#ifndef _LIGHTING_H
#define _LIGHTING_H

#include<_common.h>

class _lighting
{
    public:
        _lighting();
        virtual ~_lighting();

        const GLfloat light_ambient[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
        const GLfloat light_diffuse[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        const GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        const GLfloat light_position[4] = { 2.0f, 5.0f, 5.0f, 0.0f };

        const GLfloat mat_ambient[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
        const GLfloat mat_diffuse[4]    = { 0.8f, 0.8f, 0.8f, 1.0f };
        const GLfloat mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
        const GLfloat high_shininess[1] = { 100.0f };

        void setupLight(GLenum);

    protected:

    private:
};

#endif // _LIGHTING_H
