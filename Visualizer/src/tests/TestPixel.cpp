#include "PreCompiledHeader.h"
#include "TestPixel.h"
#include "Renderer.h"
#include "Window.h"

namespace Test {

	TestPixel::TestPixel(std::string& name)
		: Test(name), width(1280), height(720), pixelSize(32),
		projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 1000.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -500.0f)))
	{
		vertex = new Vertex[static_cast<size_t>(width) * static_cast<size_t>(height) * 4];
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
		vertexBuffer = std::make_unique<VertexBuffer>(static_cast<size_t>(width) * static_cast<size_t>(height) * 4 * sizeof(Vertex));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(3);
		bufferLayout.Push<float>(3);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, width * height * 6);

		shader = std::make_unique<Shader>("resources/shaders/Pixel.glsl");
	}

	TestPixel::~TestPixel()
	{
		delete[] vertex;
		delete[] indices;
	}

	void TestPixel::OnUpdate(float deltaTime)
	{
	}

	void TestPixel::OnRender()
	{
		unsigned int horizontalPixelCount = width / pixelSize;
		unsigned int verticalPixelCount = height / pixelSize;

		for (size_t y = 0; y < verticalPixelCount; y++)
		{
			for (size_t x = 0; x < horizontalPixelCount; x++)
			{
				vertex[4 * (x + y * horizontalPixelCount) + 0] = { static_cast<float>(x * pixelSize)      , static_cast<float>(y * pixelSize)      , 0.0f, static_cast<float>(x) / horizontalPixelCount, static_cast<float>(y) / verticalPixelCount, 0.0f };
				vertex[4 * (x + y * horizontalPixelCount) + 1] = { static_cast<float>((x + 1) * pixelSize), static_cast<float>(y * pixelSize)      , 0.0f, static_cast<float>(x) / horizontalPixelCount, static_cast<float>(y) / verticalPixelCount, 0.0f };
				vertex[4 * (x + y * horizontalPixelCount) + 2] = { static_cast<float>((x + 1) * pixelSize), static_cast<float>((y + 1) * pixelSize), 0.0f, static_cast<float>(x) / horizontalPixelCount, static_cast<float>(y) / verticalPixelCount, 0.0f };
				vertex[4 * (x + y * horizontalPixelCount) + 3] = { static_cast<float>(x * pixelSize)      , static_cast<float>((y + 1) * pixelSize), 0.0f, static_cast<float>(x) / horizontalPixelCount, static_cast<float>(y) / verticalPixelCount, 0.0f };
			}
		}
		vertexBuffer->SetData(vertex, verticalPixelCount * horizontalPixelCount * 4 * sizeof(Vertex));

		Renderer renderer;

		renderer.SetClearColor(0, 0, 0, 1);
		renderer.Clear();

		shader->Bind();
		shader->SetUniformMat4f("u_MVP", projection * view);

		renderer.Draw(*vertexArray, *indexBuffer, *shader, horizontalPixelCount * verticalPixelCount * 6);
	}

	void TestPixel::OnImGuiRender()
	{
		constexpr unsigned int min = 1;
		constexpr unsigned int max = 128;
		ImGui::SliderScalar("Pixel Size", ImGuiDataType_U32, &pixelSize, &min, &max);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}