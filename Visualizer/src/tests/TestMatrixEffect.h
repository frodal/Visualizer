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
			glm::vec2 position;
			Pixel primaryColor;
			Pixel secondaryColor;
			float length;
		};
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		Pixel* pixels;
		std::list<MatrixLetter> letters;
		unsigned int width, height, pixelSize, horizontalPixelCount, verticalPixelCount;
		float speed;
		unsigned int textureSlot;
		glm::mat4 projection, view;
	};

}