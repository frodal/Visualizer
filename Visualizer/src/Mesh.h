#pragma once
#include "PreCompiledHeader.h"

#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<std::shared_ptr<Texture>> textures);

    inline std::vector<std::shared_ptr<Texture>> GetTextures() const { return textures; }
    inline std::shared_ptr<Texture> GetTexture(unsigned int i) const { return textures[i]; }
    inline const std::unique_ptr<VertexArray>& GetVertexArray() const { return vertexArray; }
    inline const std::unique_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }

private:
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::vector<std::shared_ptr<Texture>> textures;
};