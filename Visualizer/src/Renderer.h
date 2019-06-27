#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:
	void Clear() const;
	void SetClearColor(const glm::vec4& color) const;
	void EnableBlend() const;
	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
};

