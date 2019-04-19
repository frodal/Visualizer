/*
This is the main .cpp file for the Visulazier application using OpenGL to render stuff.
*/
#include "PreCompiledHeader.h" // Includes precompiled header




int main(void)
{
	GLFWwindow* window;

	/* Initialize the GLFW library */
	if (!glfwInit())
	{
		std::cout << "Error: Could not set up the GLFW library!" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Visulazier", NULL, NULL);
	if (!window)
	{
		std::cout << "Error: Could not set up a window with an openGL context!" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize the GLEW library (Needs a valid OpenGL rendering context)*/
	GLenum message = glewInit();
	if (message != GLEW_OK)
	{
		std::cout << "Error: Could not set up the GLEW library!" << std::endl;
		std::cout << glewGetErrorString(message) << std::endl;
		return -1;
	}

	/* Prints the current versions used to the console*/
	std::cout << "Current GLFW version:   " << glfwGetVersionString() << std::endl;
	std::cout << "Current GLEW version:   " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Current OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/*Drawing a triangle using Legacy openGL*/
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(-0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}