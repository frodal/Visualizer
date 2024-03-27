#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
//#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Test {

	class Test3DCube : public Test
	{
	public:
		Test3DCube(std::string& name);
		virtual ~Test3DCube();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		glm::mat4 projection, view, model;
		float size, speed, rotationAngle;
		float cubeColor[4];
		float backgroundColor[4];
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
	};

}