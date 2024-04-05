#include "PreCompiledHeader.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<std::shared_ptr<Texture>> textures)
{
    this->textures = textures;

	vertexArray = std::make_unique<VertexArray>();
	vertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), static_cast<unsigned int>(vertices.size() * sizeof(Vertex)));
	VertexBufferLayout bufferLayout;
	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(2);
	vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

	indexBuffer = std::make_unique<IndexBuffer>(indices.data(), static_cast<unsigned int>(indices.size()));
}
