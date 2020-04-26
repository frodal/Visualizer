#include "PreCompiledHeader.h"
#include "TestPixelQuads.h"
#include "Renderer.h"

namespace Test {

	struct Color {
		float r, g, b;
	};

	TestPixelQuads::TestPixelQuads(std::string& name)
		: Test(name), width(1280), height(720), pixelSize(32),
		horizontalPixelCount(width / pixelSize), verticalPixelCount(height / pixelSize),
		speed(250.0f), distance(0.0f),
		projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 1000.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -500.0f)))
	{
		vertex = new Vertex2D[static_cast<size_t>(width) * static_cast<size_t>(height) * 4];
		indices = new unsigned int[static_cast<size_t>(width) * static_cast<size_t>(height) * 6];

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				indices[6 * (x + y * width) + 0] = 0 + 4 * (x + y * width);
				indices[6 * (x + y * width) + 1] = 1 + 4 * (x + y * width);
				indices[6 * (x + y * width) + 2] = 2 + 4 * (x + y * width);
				indices[6 * (x + y * width) + 3] = 2 + 4 * (x + y * width);
				indices[6 * (x + y * width) + 4] = 3 + 4 * (x + y * width);
				indices[6 * (x + y * width) + 5] = 0 + 4 * (x + y * width);
			}
		}

		Renderer renderer;
		renderer.EnableBlend();
		renderer.EnableDepth();

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(width * height * 4 * static_cast<unsigned int>(sizeof(Vertex2D)));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(3);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, width * height * 6);

		shader = std::make_unique<Shader>("resources/shaders/Pixel.glsl");
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", projection * view);
	}

	TestPixelQuads::~TestPixelQuads()
	{
		delete[] vertex;
		delete[] indices;
	}

	void CreateVertexData(Vertex2D* vertex, unsigned int width, unsigned int height, unsigned int pixelSize, unsigned int verticalPixelCount, unsigned int horizontalPixelCount, float distance, int threadID, int numberOfThreads)
	{
		unsigned int startY = threadID * verticalPixelCount / numberOfThreads;
		unsigned int endY = (threadID + 1) * verticalPixelCount / numberOfThreads;

		for (unsigned int y = startY; y < endY; y++)
		{
			for (unsigned int x = 0; x < horizontalPixelCount; x++)
			{
				unsigned int pos = 4 * (x + y * horizontalPixelCount);
				Color color = { StandingWave((static_cast<float>(x * pixelSize) + distance) / width), static_cast<float>(y * pixelSize) / height, 0.0f };
				vertex[pos + 0] = { static_cast<float>(x * pixelSize)      , static_cast<float>(y * pixelSize)      , color.r, color.g, color.b };
				vertex[pos + 1] = { static_cast<float>((x + 1) * pixelSize), static_cast<float>(y * pixelSize)      , color.r, color.g, color.b };
				vertex[pos + 2] = { static_cast<float>((x + 1) * pixelSize), static_cast<float>((y + 1) * pixelSize), color.r, color.g, color.b };
				vertex[pos + 3] = { static_cast<float>(x * pixelSize)      , static_cast<float>((y + 1) * pixelSize), color.r, color.g, color.b };
			}
		}
	}

	void TestPixelQuads::OnUpdate(float deltaTime)
	{
		distance += speed * deltaTime;

		horizontalPixelCount = width / pixelSize;
		verticalPixelCount = height / pixelSize;

		for (int i = 1; i < numberOfThreads; i++)
		{
			threads[i-1] = std::thread(CreateVertexData, vertex, width, height, pixelSize, verticalPixelCount, horizontalPixelCount, distance, i, numberOfThreads);
		}

		CreateVertexData(vertex, width, height, pixelSize, verticalPixelCount, horizontalPixelCount, distance, 0, numberOfThreads);

		for (int i = 1; i < numberOfThreads; i++)
		{
			threads[i-1].join();
		}
		
		vertexBuffer->SetData(vertex, verticalPixelCount * horizontalPixelCount * 4 * sizeof(Vertex2D));
	}

	void TestPixelQuads::OnRender()
	{
		Renderer renderer;

		renderer.Draw(*vertexArray, *indexBuffer, *shader, horizontalPixelCount * verticalPixelCount * 6);
	}

	void TestPixelQuads::OnImGuiRender()
	{
		constexpr unsigned int minPS = 1;
		constexpr unsigned int maxPS = 128;
		ImGui::SliderScalar("Pixel Size", ImGuiDataType_U32, &pixelSize, &minPS, &maxPS);
		ImGui::SliderFloat("Scroll speed", &speed, 0.0f, 500.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}