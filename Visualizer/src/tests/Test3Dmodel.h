#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
//#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"

namespace Test {

	class Test3Dmodel : public Test
	{
	public:
		Test3Dmodel(std::string& name);
		virtual ~Test3Dmodel();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		glm::mat4 projection, view, model;
		float size, speed, rotationAngle;
		float modelColor[4];
		float backgroundColor[4];
		std::unique_ptr<Model> model3D;
		std::unique_ptr<Shader> shader;
	};

}