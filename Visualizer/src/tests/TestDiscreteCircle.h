#pragma once
#include "PreCompiledHeader.h"
#include "Test.h"

#include "Shader.h"

namespace Test {

	class TestDiscreteCircle : public Test
	{
	public:
		TestDiscreteCircle(std::string& name);
		virtual ~TestDiscreteCircle();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		glm::mat4 projection, view, model;
		int Ntriangles;
		float radius;
		float circleColor[4];
		float backgroundColor[4];
		std::unique_ptr<Shader> shader;
	};

}