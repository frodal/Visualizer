#pragma once
#include "PreCompiledHeader.h"

namespace Test {

	class Test
	{
	public:
		Test() : testName("Unknown") {}
		Test(std::string& name) : testName(name) {};
		Test(const char* name) : testName(name) {};
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual std::string GetTestName() { return "Test: " + testName; }
	protected:
		std::string testName;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		virtual void OnImGuiRender() override;
		virtual std::string GetTestName() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			tests.push_back(std::make_pair(name, [](std::string& _name) {return new T(_name); }));
		}
	private:
		Test*& currentTest;
		std::vector<std::pair<std::string, std::function<Test* (std::string&)>>> tests;
	};

}