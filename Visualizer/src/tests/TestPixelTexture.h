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

	struct Pixel {
		uint8_t r, g, b, a;
	};

	class TestPixelTexture : public Test
	{
	public:
		TestPixelTexture(std::string& name);
		virtual ~TestPixelTexture();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		Pixel* pixels;
		unsigned int width, height, pixelSize;
		float speed, distance;
		unsigned int textureSlot;
		glm::mat4 projection, view;
	};

}