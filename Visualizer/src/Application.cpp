/*
This is the main .cpp file for the Visualizer application using OpenGL to render stuff.
See the documentation of OpenGL, e.g., http://docs.gl/
*/
/* Includes */
#include "PreCompiledHeader.h"
#include "Renderer.h"
#include "Window.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestDiscreteCircle.h"
#include "tests/Test3DCube.h"
#include "tests/TestPixelQuads.h"
#include "tests/TestPixelTexture.h"
#include "tests/TestMatrixEffect.h"
#include "tests/TestMandelbrot.h"
#include "tests/Test3Dmodel.h"


/* Initial window title, width, height and Vsync setting */
constexpr const char* TITLE = "Visualizer";
constexpr unsigned int WIDTH = 1280;
constexpr unsigned int HEIGHT = 720;
constexpr bool VSYNC = true;

/* Entry Point*/
int main(void)
{
	/* Scope for certain variables */
	{
		Window window({ TITLE, WIDTH, HEIGHT, VSYNC });
		Renderer renderer;

		// Setup Dear ImGui context
		ImGui::CreateContext();
		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsLight();
		// Setup Platform/Renderer bindings for ImGui
		ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

		/* Tests */
		Test::Test* currentTest = nullptr;
		Test::TestMenu* testMenu = new Test::TestMenu(currentTest, &window);
		currentTest = testMenu;
		/* Registering tests */
		testMenu->RegisterTest<Test::TestClearColor>("Clear color");
		testMenu->RegisterTest<Test::TestTexture2D>("2D texture");
		testMenu->RegisterTest<Test::TestDiscreteCircle>("Discrete circle");
		testMenu->RegisterTest<Test::Test3DCube>("3D Cube");
		testMenu->RegisterTest<Test::TestPixelQuads>("Pixel Engine quads");
		testMenu->RegisterTest<Test::TestPixelTexture>("Pixel Engine texture");
		testMenu->RegisterTest<Test::TestMatrixEffect>("The Matrix pixel effect");
		testMenu->RegisterTest<Test::TestMandelbrot>("Mandelbrot fractal");
		testMenu->RegisterTest<Test::Test3Dmodel>("3D model");

		/* Loop until the user closes the window */
		while (!window.ShouldClose())
		{
			renderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(static_cast<float>(window.GetDeltaTime()));
				currentTest->OnRender();
				ImGui::Begin(currentTest->GetTestName().c_str());
				currentTest->OnImGuiRender();
				if (currentTest != testMenu && ImGui::Button("<- back"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				else if (currentTest == testMenu && ImGui::Button("Quit")) 
				{
					window.Close();
				}
				ImGui::End();
			}

			// Rendering ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			window.OnUpdate();
		}
		/* Free allocated memory*/
		if (currentTest != testMenu)
			delete testMenu;
		delete currentTest;
		
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Window::TerminateWindow();
	return 0;
}