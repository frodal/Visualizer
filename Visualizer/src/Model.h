#pragma once
#include "PreCompiledHeader.h"

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const std::string& path)
    {
        loadModel(path);
    }
    const std::vector<Mesh>& GetMeshes() const { return meshes; }

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<std::shared_ptr<Texture>> textures_loaded;
};
