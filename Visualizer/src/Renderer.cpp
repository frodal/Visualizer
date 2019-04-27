#include "PreCompiledHeader.h"
#include "Renderer.h"


void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(const glm::vec4& color) const
{
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.Bind();

	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
