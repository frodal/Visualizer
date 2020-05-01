#include "PreCompiledHeader.h"
#include "TestPixelTexture.h"

#include "Renderer.h"

namespace Test {

	void CreatePixelData(Pixel* pixels, unsigned int width, unsigned int height, unsigned int pixelSize, unsigned int verticalPixelCount, unsigned int horizontalPixelCount, float distance, int threadID, int numberOfThreads)
	{
		unsigned int startY = threadID * verticalPixelCount / numberOfThreads;
		unsigned int endY = (threadID + 1) * verticalPixelCount / numberOfThreads;

		for (unsigned int y = startY; y < endY; y++)
		{
			for (unsigned int x = 0; x < horizontalPixelCount; x++)
			{
				pixels[x + y * horizontalPixelCount] = { static_cast<uint8_t>(255 * StandingWave((static_cast<float>(x * pixelSize) + distance) / width)), static_cast<uint8_t>(255 * y * pixelSize / height), 0, 255 };
			}
		}
	}

	TestPixelTexture::TestPixelTexture(std::string& name)
		: Test(name), width(1280), height(720), pixelSize(32), horizontalPixelCount(width / pixelSize), verticalPixelCount(height / pixelSize),
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

		for (int i = 0; i < numberOfThreads; i++)
		{
			threads[i] = std::async(std::launch::async, CreatePixelData, pixels, width, height, pixelSize, verticalPixelCount, horizontalPixelCount, distance, i, numberOfThreads);
		}

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
		for (int i = 0; i < numberOfThreads; i++)
		{
			if (threads[i].valid())
				threads[i].wait();
		}

		delete[] pixels;
	}

	void TestPixelTexture::OnUpdate(float deltaTime)
	{
		distance += speed * deltaTime;

		for (int i = 0; i < numberOfThreads; i++)
		{
			threads[i].wait();
		}

		texture->UpdateTexture(reinterpret_cast<unsigned char*>(pixels), horizontalPixelCount, verticalPixelCount);

		verticalPixelCount = height / pixelSize;
		horizontalPixelCount = width / pixelSize;

		for (int i = 0; i < numberOfThreads; i++)
		{
			threads[i] = std::async(std::launch::async, CreatePixelData, pixels, width, height, pixelSize, verticalPixelCount, horizontalPixelCount, distance, i, numberOfThreads);
		}
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