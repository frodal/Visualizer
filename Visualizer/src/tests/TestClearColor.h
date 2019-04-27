#pragma once
#include "Test.h"

namespace Test {

	class TestClearColor : public Test
	{
	public:
		TestClearColor(std::string& name);
		virtual ~TestClearColor();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
	private:
		float clearColor[4];
	};

}