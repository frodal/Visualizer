#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:
	void Clear() const;
	void SetClearColor(const glm::vec4& color) const;
	void SetClearColor(const float& r, const float& g, const float& b, const float& a) const;
	void EnableBlend() const;
	void EnableDepth() const;
	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
};

