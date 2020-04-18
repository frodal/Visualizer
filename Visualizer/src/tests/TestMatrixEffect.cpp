#include "PreCompiledHeader.h"
#include "TestMatrixEffect.h"

#include "Renderer.h"

namespace Test {

	TestMatrixEffect::TestMatrixEffect(std::string& name)
		: Test(name), width(1280), height(720), pixelSize(16),
		horizontalPixelCount(width / pixelSize), verticalPixelCount(height / pixelSize),
		speed(20.0f),
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

		pixels = new Pixel[static_cast<size_t>(verticalPixelCount) * static_cast<size_t>(horizontalPixelCount)];
		for (unsigned int y = 0; y < verticalPixelCount; y++)
		{
			for (unsigned int x = 0; x < horizontalPixelCount; x++)
			{
				pixels[x + y * horizontalPixelCount] = { 0, 0, 0, 0 };
			}
		}
		
		for (float i = 0.0f; i < horizontalPixelCount; i+=10.0f)
		{
			letters.push_back({ {i,static_cast<float>(verticalPixelCount)}, {255,255,255,255}, {0,255,0,255}, 10.0f });
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

	TestMatrixEffect::~TestMatrixEffect()
	{
		delete[] pixels;
	}

	void TestMatrixEffect::OnUpdate(float deltaTime)
	{
		
		for (auto& letter : letters)
		{
			int startHeight = static_cast<int>(letter.position.y);
			unsigned int length = static_cast<unsigned int>(letter.length);
			int endHeight = startHeight + length;
			unsigned int x = static_cast<unsigned int>(letter.position.x);
			if (startHeight < static_cast<int>(verticalPixelCount) && endHeight >= 0)
			{
				if (startHeight < verticalPixelCount && startHeight >= 0)
				{
					pixels[x + startHeight * horizontalPixelCount] = letter.primaryColor;
				}
				for (unsigned int i = 1; i < length; i++)
				{
					int y = startHeight + i;
					if (y < verticalPixelCount && y >= 0)
					{
						pixels[x + y * horizontalPixelCount] = letter.secondaryColor;
					}
				}
				if (endHeight < static_cast<int>(verticalPixelCount) && endHeight >= 0)
				{
					pixels[x + endHeight * horizontalPixelCount].a = 0;
				}
			}
			letter.position.y -= speed * deltaTime;
		}

		texture->UpdateTexture(reinterpret_cast<unsigned char*>(pixels), horizontalPixelCount, verticalPixelCount);
	}

	void TestMatrixEffect::OnRender()
	{
		Renderer renderer;

		texture->Bind(textureSlot);

		renderer.Draw(*vertexArray, *indexBuffer, *shader);
	}

	void TestMatrixEffect::OnImGuiRender()
	{
		ImGui::SliderFloat("Scroll speed", &speed, 0.0f, 500.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}