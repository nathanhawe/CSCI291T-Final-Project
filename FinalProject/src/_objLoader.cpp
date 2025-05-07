#include "_objLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <SOIL.h>

_objLoader::_objLoader() {}

_objLoader::~_objLoader() {}

bool _objLoader::loadOBJ(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << path << std::endl;
        return false;
    }

    std::string line;
    std::string basePath = std::string(path).substr(0, std::string(path).find_last_of("/\\") + 1);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "mtllib") {
            std::string mtlFile;
            ss >> mtlFile;
            loadMTL((basePath + mtlFile).c_str());
        }
        else if (prefix == "v") {
            Vertex vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            TextureCoord texCoord;
            ss >> texCoord.u >> texCoord.v;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "vn") {
            Normal normal;
            ss >> normal.nx >> normal.ny >> normal.nz;
            normals.push_back(normal);
        }
        else if (prefix == "usemtl") {
            ss >> currentMaterial;
        }
        else if (prefix == "f") {
            Face face;
            face.materialName = currentMaterial;
            for (int i = 0; i < 3; ++i) {
                std::string token;
                ss >> token;
                std::replace(token.begin(), token.end(), '/', ' ');
                std::stringstream fs(token);
                fs >> face.v[i] >> face.vt[i] >> face.vn[i];
                face.v[i]--;
                face.vt[i]--;
                face.vn[i]--;
            }
            faces.push_back(face);
        }
    }

    file.close();
    return true;
}

bool _objLoader::loadMTL(const char* mtlPath) {
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
        return false;
    }

    std::string line, currentMtl;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            ss >> currentMtl;
            materials[currentMtl] = Material{ currentMtl };
        }
        else if (prefix == "map_Kd") {
            std::string texFile;
            ss >> texFile;

            GLuint texId;
            glGenTextures(1, &texId);
            glBindTexture(GL_TEXTURE_2D, texId);

            int w, h;
            unsigned char* image = SOIL_load_image(texFile.c_str(), &w, &h, 0, SOIL_LOAD_RGBA);
            if (!image) {
                std::cerr << "Failed to load texture: " << texFile << std::endl;
                continue;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            SOIL_free_image_data(image);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            materials[currentMtl].textureID = texId;
        }
    }

    file.close();
    return true;
}

void _objLoader::renderModel() {
    //glEnable(GL_TEXTURE_2D);
    std::string activeMaterial = "";

    glBegin(GL_QUADS);
    for (const auto& face : faces) {
        if (face.materialName != activeMaterial) {
            glEnd();
            activeMaterial = face.materialName;
            GLuint tex = materials[activeMaterial].textureID;
            if (tex) glBindTexture(GL_TEXTURE_2D, tex);
            else glBindTexture(GL_TEXTURE_2D, 0); // No texture
            glBegin(GL_TRIANGLES);
        }

        for (int i = 0; i < 3; ++i) {
            const Normal& normal = normals[face.vn[i]];
            glNormal3f(normal.nx, normal.ny, normal.nz);

            const TextureCoord& texCoord = texCoords[face.vt[i]];
            glTexCoord2f(texCoord.u, texCoord.v);

            const Vertex& vertex = vertices[face.v[i]];
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();
    //`glDisable(GL_TEXTURE_2D);
}
