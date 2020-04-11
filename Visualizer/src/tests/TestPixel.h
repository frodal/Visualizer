#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Test {

	struct Vertex {
		float x, y, z;
		float r, g, b;
	};

	class TestPixel : public Test
	{
	public:
		TestPixel(std::string& name);
		virtual ~TestPixel();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		unsigned int width, height, pixelSize;
		glm::mat4 projection, view;
		Vertex* vertex;
		unsigned int* indices;
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
	};

}