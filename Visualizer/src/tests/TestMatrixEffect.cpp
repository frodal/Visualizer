#include "PreCompiledHeader.h"
#include "TestMatrixEffect.h"

#include "Renderer.h"
#include "Window.h"

namespace Test {

	TestMatrixEffect::TestMatrixEffect(std::string& name)
		: Test(name), nextLetter(0), width(1280), height(720), pixelSize(8),
		horizontalPixelCount(width / pixelSize), verticalPixelCount(height / pixelSize),
		minSpeed(5.0f), maxSpeed(25.0f), minLength(10.0f), maxLength(30.0f),
		timeLastSpawn(0), updateTime(0),
		primaryColor{ 1.0f, 1.0f, 1.0f, 1.0f }, secondaryColor{ 0.0f, 1.0f, 0.0f, 1.0f },
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
		memset(pixels, 0, static_cast<size_t>(verticalPixelCount) * static_cast<size_t>(horizontalPixelCount) * sizeof(Pixel));

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
		// Clearing the screen
		memset(pixels, 0, static_cast<size_t>(verticalPixelCount) * static_cast<size_t>(horizontalPixelCount) * sizeof(Pixel));

		// Handling letters
		for (auto& letter : letters)
		{
			// Casting to integers
			int startHeight = static_cast<int>(letter.position.y + 0.5f);
			unsigned int length = static_cast<unsigned int>(letter.length);
			int endHeight = startHeight + length;
			if (endHeight < 0)
				continue;
			unsigned int x = static_cast<unsigned int>(letter.position.x);

			// Updating screen pixels
			for (unsigned int i = 0; i < length; i++)
			{
				int y = startHeight + i;
				if (y < verticalPixelCount && y >= 0)
				{
					unsigned int pos = x + y * horizontalPixelCount;
					Pixel oldPixelData = pixels[pos];
					Pixel newPixelData = i == 0 ? letter.primaryColor : letter.secondaryColor;
					float newPixelAlpha = static_cast<float>(newPixelData.a) * (1.0f - static_cast<float>(i) / length);
					float alpha = 255.0f * (1.0f - (1.0f - newPixelAlpha / 255.0f) * (1.0f - static_cast<float>(oldPixelData.a) / 255.0f));
					pixels[pos].r = static_cast<unsigned char>(static_cast<float>(newPixelData.r) * newPixelAlpha / alpha + static_cast<float>(oldPixelData.r) * (1.0f - newPixelAlpha / alpha));
					pixels[pos].g = static_cast<unsigned char>(static_cast<float>(newPixelData.g) * newPixelAlpha / alpha + static_cast<float>(oldPixelData.g) * (1.0f - newPixelAlpha / alpha));
					pixels[pos].b = static_cast<unsigned char>(static_cast<float>(newPixelData.b) * newPixelAlpha / alpha + static_cast<float>(oldPixelData.b) * (1.0f - newPixelAlpha / alpha));
					pixels[pos].a = static_cast<unsigned char>(alpha);
				}
			}

			// Update letter position
			letter.position.y -= letter.speed * deltaTime;
		}

		// Spawning letters
		float time = Window::GetTime();
		if (time > updateTime + 1.0f / 60.0f)
		{
			if (time > timeLastSpawn + (rand() % 5))
			{
				letters[nextLetter].position = { rand() % horizontalPixelCount, verticalPixelCount };
				letters[nextLetter].primaryColor = { static_cast<unsigned char>(255.0f * primaryColor[0]), static_cast<unsigned char>(255.0f * primaryColor[1]) , static_cast<unsigned char>(255.0f * primaryColor[2]), static_cast<unsigned char>(255.0f * primaryColor[3]) };
				letters[nextLetter].secondaryColor = { static_cast<unsigned char>(255.0f * secondaryColor[0]), static_cast<unsigned char>(255.0f * secondaryColor[1]) , static_cast<unsigned char>(255.0f * secondaryColor[2]), static_cast<unsigned char>(255.0f * secondaryColor[3]) };
				letters[nextLetter].length = rand() % static_cast<int>(maxLength - minLength + 1.0f) + minLength;
				letters[nextLetter].speed = rand() % static_cast<int>(maxSpeed - minSpeed + 1.0f) + minSpeed;
				nextLetter = (static_cast<size_t>(nextLetter) + 1) % letters.max_size();
				timeLastSpawn = time;
			}
			updateTime = time;
		}

		// Upload pixels to the GPU
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
		ImGui::SliderFloat("Minimum speed", &minSpeed, 0.0f, 20.0f);
		ImGui::SliderFloat("Maximum speed", &maxSpeed, minSpeed, 50.0f);
		maxSpeed = maxSpeed - minSpeed < 0.0f ? minSpeed : maxSpeed;
		ImGui::SliderFloat("Minimum length", &minLength, 0.0f, 20.0f);
		ImGui::SliderFloat("Maximum length", &maxLength, minLength, 50.0f);
		maxLength = maxLength - minLength < 0.0f ? minLength : maxLength;
		ImGui::ColorEdit4("Primary color", primaryColor);
		ImGui::ColorEdit4("Trail color", secondaryColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}