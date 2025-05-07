#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include <vector>
#include <string>
#include <map>

#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Vertex {
    float x, y, z;
};

struct TextureCoord {
    float u, v;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    int v[3], vt[3], vn[3];
    std::string materialName;  // Track the material used for this face
};

struct Material {
    std::string name;
    GLuint textureID = 0;
};

class _objLoader
{
public:
    _objLoader();
    virtual ~_objLoader();

    bool loadOBJ(const char* objPath);   // Load .obj + .mtl automatically
    void renderModel();                 // Render the model

private:
    bool loadMTL(const char* mtlPath);  // Internal: loads materials

    std::vector<Vertex> vertices;
    std::vector<TextureCoord> texCoords;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    std::map<std::string, Material> materials;
    std::string currentMaterial;
};

#endif // _OBJLOADER_H
