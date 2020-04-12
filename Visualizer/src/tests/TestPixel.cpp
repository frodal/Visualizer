#include "PreCompiledHeader.h"
#include "TestPixel.h"
#include "Renderer.h"

namespace Test {

	struct Color {
		float r, g, b;
	};

	TestPixel::TestPixel(std::string& name)
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
		vertexBuffer = std::make_unique<VertexBuffer>(static_cast<size_t>(width) * static_cast<size_t>(height) * 4 * sizeof(Vertex2D));
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(3);
		vertexArray->AddBuffer(*vertexBuffer, bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, width * height * 6);

		shader = std::make_unique<Shader>("resources/shaders/Pixel.glsl");
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", projection * view);
	}

	TestPixel::~TestPixel()
	{
		delete[] vertex;
		delete[] indices;
	}

	void TestPixel::OnUpdate(float deltaTime)
	{
		distance += speed * deltaTime;

		horizontalPixelCount = width / pixelSize;
		verticalPixelCount = height / pixelSize;

		unsigned int offset = 0;
		for (unsigned int y = 0; y < verticalPixelCount; y++)
		{
			for (unsigned int x = 0; x < horizontalPixelCount; x++)
			{
				Color color = { abs(sin(3.141592f * static_cast<float>((x * pixelSize + static_cast<unsigned int>(distance))) / width)), static_cast<float>(y * pixelSize) / height, 0.0f };
				vertex[offset + 0] = { static_cast<float>(x * pixelSize)      , static_cast<float>(y * pixelSize)      , color.r, color.g, color.b };
				vertex[offset + 1] = { static_cast<float>((x + 1) * pixelSize), static_cast<float>(y * pixelSize)      , color.r, color.g, color.b };
				vertex[offset + 2] = { static_cast<float>((x + 1) * pixelSize), static_cast<float>((y + 1) * pixelSize), color.r, color.g, color.b };
				vertex[offset + 3] = { static_cast<float>(x * pixelSize)      , static_cast<float>((y + 1) * pixelSize), color.r, color.g, color.b };
				offset += 4;
			}
		}
		vertexBuffer->SetData(vertex, verticalPixelCount * horizontalPixelCount * 4 * sizeof(Vertex2D));
	}

	void TestPixel::OnRender()
	{
		Renderer renderer;

		renderer.Draw(*vertexArray, *indexBuffer, *shader, horizontalPixelCount * verticalPixelCount * 6);
	}

	void TestPixel::OnImGuiRender()
	{
		constexpr unsigned int minPS = 1;
		constexpr unsigned int maxPS = 128;
		ImGui::SliderScalar("Pixel Size", ImGuiDataType_U32, &pixelSize, &minPS, &maxPS);
		ImGui::SliderFloat("Scroll speed", &speed, 0.0f, 500.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}