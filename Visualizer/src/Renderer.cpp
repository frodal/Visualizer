#include "PreCompiledHeader.h"
#include "Renderer.h"


void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetClearColor(const glm::vec4& color) const
{
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

void Renderer::SetClearColor(const float& r, const float& g, const float& b, const float& a) const
{
	GLCall(glClearColor(r, g, b, a));
}

void Renderer::EnableBlend() const
{
	/* Enable blending*/
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::EnableDepth() const
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.Bind();

	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader, unsigned int indexCount) const
{
	shader.Bind();

	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawWireFrame(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.Bind();

	vertexArray.Bind();
	indexBuffer.Bind();

	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}