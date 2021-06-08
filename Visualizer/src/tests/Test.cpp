#include "PreCompiledHeader.h"
#include "Test.h"

namespace Test {

	TestMenu::TestMenu(Test*& currentTestPointer, Window* window)
		: Test("Menu"), currentTest(currentTestPointer)
	{
		this->window = window;
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				currentTest = test.second(test.first);
				currentTest->SetWindow(window);
			}
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	std::string TestMenu::GetTestName()
	{
		return testName;
	}

}