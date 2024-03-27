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

	class TestMandelbrot : public Test
	{
	public:
		TestMandelbrot(std::string& name);
		virtual ~TestMandelbrot();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

		virtual void SetWindow(Window* window) override;
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		unsigned int width, height;
		glm::mat4 projection, view;
		glm::dvec2 position;
		double scale;
		int MaxIterations;
		float aspectRatio;
	};

}