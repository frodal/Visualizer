#include "PreCompiledHeader.h"
#include "TestClearColor.h"
#include "Renderer.h"

namespace Test {

	TestClearColor::TestClearColor(std::string& name)
		: Test(name), clearColor{ 0.7f, 0.7f, 0.7f, 1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void TestClearColor::OnRender()
	{
		Renderer renderer;
		renderer.SetClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		renderer.Clear();
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear color", clearColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}