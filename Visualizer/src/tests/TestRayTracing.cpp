#include "PreCompiledHeader.h"
#include "TestRayTracing.h"

#include "Renderer.h"

Test::TestRayTracing::TestRayTracing(std::string& name)
	: Test(name), pixels(nullptr), width(1280), height(720),
	textureSlot(0), projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height))),
	view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
	/* Vertex positions*/
	float positions[] = {
							   0.0f,                       0.0f, 0.0f, 0.0f,
		  static_cast<float>(width),                       0.0f, 1.0f, 0.0f,
		  static_cast<float>(width), static_cast<float>(height), 1.0f, 1.0f,
							   0.0f, static_cast<float>(height), 0.0f, 1.0f
	};

	/* Triangle vertex indices (position[index])*/
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	pixels = new Pixel[static_cast<size_t>(height) * static_cast<size_t>(width)];

	Renderer renderer;
	renderer.EnableBlend();

	vertexArray = std::make_unique<VertexArray>();
	vertexBuffer = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(sizeof(positions)));
	VertexBufferLayout bufferLayout;
	bufferLayout.Push<float>(2);
	bufferLayout.Push<float>(2);
	vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

	indexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(lengthof(indices)));

	shader = std::make_unique<Shader>("resources/shaders/Texture.glsl");
	shader->Bind();
	shader->SetUniform1i("u_Texture", textureSlot);
	shader->SetUniformMat4f("u_MVP", projection * view);

	texture = std::make_unique<Texture>();
}

Test::TestRayTracing::~TestRayTracing()
{
	delete[] pixels;
}

void Test::TestRayTracing::OnUpdate(float deltaTime)
{
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			pixels[x + y * width] = { static_cast<uint8_t>(255 * StandingWave(static_cast<float>(x) / width)), static_cast<uint8_t>(255 * y / height), 0, 255 };
		}
	}

	texture->UpdateTexture(reinterpret_cast<unsigned char*>(pixels), width, height);
}

void Test::TestRayTracing::OnRender()
{
	Renderer renderer;

	texture->Bind(textureSlot);

	renderer.Draw(*vertexArray, *indexBuffer, *shader);
}

void Test::TestRayTracing::OnImGuiRender()
{
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
