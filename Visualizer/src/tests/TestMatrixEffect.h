#pragma once
#include "PreCompiledHeader.h"
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Test {

	class TestMatrixEffect : public Test
	{
	public:
		TestMatrixEffect(std::string& name);
		virtual ~TestMatrixEffect();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

		struct Pixel {
			uint8_t r, g, b, a;
		};
		struct MatrixLetter
		{
			glm::vec2 position = { 0.0f, -10000.0f };
			Pixel primaryColor = { 255, 255, 255, 255 };
			Pixel secondaryColor = { 0, 255, 0, 255 };
			float length = 10.0f;
			float speed = 10.0f;
		};
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		Pixel* pixels;
		std::array<MatrixLetter,2000> letters;
		unsigned int nextLetter;
		unsigned int width, height, pixelSize, horizontalPixelCount, verticalPixelCount;
		float speed, timeLastSpawn, updateTime, primaryColor[4], secondaryColor[4];
		unsigned int textureSlot;
		glm::mat4 projection, view;
	};

}