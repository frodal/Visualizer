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

void Renderer::Draw(const Model& model, Shader& shader) const
{
	const std::vector<Mesh>& meshes = model.GetMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		Draw(meshes[i], shader);
	}
}

void Renderer::Draw(const Mesh& mesh, Shader& shader) const
{
	shader.Bind();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	for (unsigned int i = 0; i < mesh.GetTextures().size(); i++)
	{
		mesh.GetTexture(i)->Bind(i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = mesh.GetTexture(i)->GetTextureType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);

		shader.SetUniform1i(("u_" + name + number).c_str(), i);
		shader.SetUniform1i(("u_flag_" + name + number).c_str(), 1);
	}

	Draw(*mesh.GetVertexArray(), *mesh.GetIndexBuffer(), shader);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	Draw(vertexArray, indexBuffer, shader, indexBuffer.GetCount());
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

void Renderer::DrawWireFrame(const Model& model, const Shader& shader) const
{
	const std::vector<Mesh>& meshes = model.GetMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		DrawWireFrame(meshes[i], shader);
	}
}

void Renderer::DrawWireFrame(const Mesh& mesh, const Shader& shader) const
{
	DrawWireFrame(*mesh.GetVertexArray(), *mesh.GetIndexBuffer(), shader);
}
