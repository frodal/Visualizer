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

	class TestTexture2D : public Test
	{
	public:
		TestTexture2D(std::string& name);
		virtual ~TestTexture2D();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		unsigned int textureSlot;
		glm::mat4 projection, view;
		glm::vec3 translationA, translationB;
	};

}