/*
This is the main .cpp file for the Visulazier application using OpenGL to render stuff.
See the documentation of OpenGL, e.g., http://docs.gl/
*/
#include "PreCompiledHeader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"



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
	window = glfwCreateWindow(640, 480, "Visulazier", NULL, NULL);
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
		/* Vertex positions*/
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
		};

		/* Triangle vertex indices (position[index])*/
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		/* Allocates and sets a vertex array to be used togheter with the array buffer below*/
		VertexArray vertexArray;

		/* Allocates and sets an array buffer for the vertex data*/
		VertexBuffer vertexBuffer(positions, lengthof(positions) * sizeof(float));

		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, bufferLayout);

		/* Allocates and sets an index buffer for the data*/
		IndexBuffer indexBuffer(indices, lengthof(indices));

		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		/* Retrives the location of the uniform "u_Color", used in the fragment shader*/
		shader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);

		/* Clears the bound buffers and program/shader*/
		vertexArray.UnBind();
		shader.UnBind();
		vertexBuffer.UnBind();
		indexBuffer.UnBind();

		float rgb[] = { 0.0f, 0.0f, 0.0f };
		float increment;
		int channel = 2;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			/*Binding shader and sets the value of "u_Color" at location*/
			shader.Bind();
			/* Sets the uniform used in the shader with name "u_Color"*/
			shader.SetUniform4f("u_Color", rgb[0], rgb[1], rgb[2], 1.0f);

			/* Binding vertex array and index buffer*/
			vertexArray.Bind();
			indexBuffer.Bind();

			/*Drawing using openGL*/
			GLCall(glDrawElements(GL_TRIANGLES, lengthof(indices), GL_UNSIGNED_INT, nullptr));

			if (rgb[channel] <= 0)
				increment = 0.01f;
			else if (rgb[channel] >= 1.0f)
				increment = -0.01f;
			rgb[channel] += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	/* Closes and destroys the GLFW window*/
	glfwDestroyWindow(window);

	/* Terminates the GLFW context*/
	glfwTerminate();
	return 0;
}