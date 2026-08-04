#pragma once

#include <string>
#include <vector>


#include "Mesh.h"
#include "shader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model 
{
public:
    // model data 
    std::pmr::vector<Texture> textures_loaded;
    std::vector<Mesh>    meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const &path, bool gamma = false);
    void Draw(Shader &shader);
    
private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


extern unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);