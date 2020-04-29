#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Test {

	struct Vertex2D {
		float x, y;
		float r, g, b;
	};

	class TestPixelQuads : public Test
	{
	public:
		TestPixelQuads(std::string& name);
		virtual ~TestPixelQuads();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		unsigned int width, height, pixelSize, horizontalPixelCount, verticalPixelCount;
		float speed, distance;
		glm::mat4 projection, view;
		Vertex2D* vertex;
		unsigned int* indices;
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		static const int numberOfThreads = 2;
		std::thread threads[numberOfThreads];
	};

}