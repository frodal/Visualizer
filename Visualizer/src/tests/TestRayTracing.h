#pragma once
#include "PreCompiledHeader.h"
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "RayTracingCamera.h"
#include "RayTracingRenderer.h"

namespace Test
{
	class TestRayTracing : public Test
	{
	public:
		TestRayTracing(std::string& name);
		virtual ~TestRayTracing();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

		virtual void SetWindow(Window* window) override;
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		unsigned int width, height;
		unsigned int textureSlot;
		RayTracingCamera camera;
		RayTracingRenderer rayTracingRenderer;
		Scene scene;
	};
}