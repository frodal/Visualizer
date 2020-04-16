#include "PreCompiledHeader.h"
#include "TestPixelTexture.h"

#include "Renderer.h"

namespace Test {

	TestPixelTexture::TestPixelTexture(std::string& name)
		: Test(name), width(1280), height(720), pixelSize(32),
		speed(250.0f), distance(0.0f),
		textureSlot(0), projection(glm::ortho(0.0f,static_cast<float>(width), 0.0f, static_cast<float>(height))),
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
		vertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, lengthof(indices));

		shader = std::make_unique<Shader>("resources/shaders/Texture.glsl");
		shader->Bind();
		shader->SetUniform1i("u_Texture", textureSlot);
		shader->SetUniformMat4f("u_MVP", projection * view);

		texture = std::make_unique<Texture>();
	}

	TestPixelTexture::~TestPixelTexture()
	{
		delete[] pixels;
	}

	void TestPixelTexture::OnUpdate(float deltaTime)
	{
		distance += speed * deltaTime;

		unsigned int verticalPixelCount = height / pixelSize;
		unsigned int horizontalPixelCount = width / pixelSize;

		for (unsigned int y = 0; y < verticalPixelCount; y++)
		{
			for (unsigned int x = 0; x < horizontalPixelCount; x++)
			{
				pixels[x + y * horizontalPixelCount] = { static_cast<uint8_t>(255 * abs(sin(3.141592f * static_cast<float>((x * pixelSize + static_cast<unsigned int>(distance))) / width))), static_cast<uint8_t>(255 * y * pixelSize / height), 0, 255 };
			}
		}

		texture->UpdateTexture(reinterpret_cast<unsigned char*>(pixels), horizontalPixelCount, verticalPixelCount);
	}

	void TestPixelTexture::OnRender()
	{
		Renderer renderer;

		texture->Bind(textureSlot);

		renderer.Draw(*vertexArray, *indexBuffer, *shader);
	}

	void TestPixelTexture::OnImGuiRender()
	{
		constexpr unsigned int minPS = 1;
		constexpr unsigned int maxPS = 128;
		ImGui::SliderScalar("Pixel Size", ImGuiDataType_U32, &pixelSize, &minPS, &maxPS);
		ImGui::SliderFloat("Scroll speed", &speed, 0.0f, 500.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}