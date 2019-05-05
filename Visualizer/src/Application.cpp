/*
This is the main .cpp file for the Visulazier application using OpenGL to render stuff.
See the documentation of OpenGL, e.g., http://docs.gl/
*/
#include "PreCompiledHeader.h"
#include "Renderer.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestDiscreteCircle.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the GLFW library */
	if (!glfwInit())
	{
		std::cout << "Error: Could not set up the GLFW library!" << std::endl;
		return -1;
	}

	/* Sets the version of OpenGL to use and sets the openGL profile*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Visulazier", NULL, NULL);
	if (!window)
	{
		std::cout << "Error: Could not set up a window with an openGL context!" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	/* Initialize the GLEW library (Needs a valid OpenGL rendering context)*/
	GLenum message = glewInit();
	if (message != GLEW_OK)
	{
		std::cout << "Error: Could not set up the GLEW library!" << std::endl;
		std::cout << glewGetErrorString(message) << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Prints the current versions used to the console*/
	std::cout << "Current GLFW version:   " << glfwGetVersionString() << std::endl;
	std::cout << "Current GLEW version:   " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Current OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	{
		Renderer renderer;

		// Setup Dear ImGui context
		ImGui::CreateContext();
		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsLight();
		// Setup Platform/Renderer bindings for ImGui
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		Test::Test* currentTest = nullptr;
		Test::TestMenu* testMenu = new Test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<Test::TestClearColor>("Clear color");
		testMenu->RegisterTest<Test::TestTexture2D>("2D texture");
		testMenu->RegisterTest<Test::TestDiscreteCircle>("Discrete circle");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin(currentTest->GetTestName().c_str());
				currentTest->OnImGuiRender();
				if (currentTest != testMenu && ImGui::Button("<- back"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				ImGui::End();
			}

			// Rendering ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		if (currentTest != testMenu)
			delete testMenu;
		delete currentTest;
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	/* Closes and destroys the GLFW window*/
	glfwDestroyWindow(window);

	/* Terminates the GLFW context*/
	glfwTerminate();
	return 0;
}