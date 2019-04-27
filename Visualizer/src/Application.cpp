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
#include "VertexBufferLayout.h"
#include "Texture.h"


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
		/* Vertex positions*/
		float positions[] = {
			 -50.0f,-50.0f, 0.0f, 0.0f,
			  50.0f,-50.0f, 1.0f, 0.0f,
			  50.0f, 50.0f, 1.0f, 1.0f,
			 -50.0f, 50.0f, 0.0f, 1.0f
		};

		/* Triangle vertex indices (position[index])*/
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Allocates and sets a vertex array to be used togheter with the array buffer below*/
		VertexArray vertexArray;

		/* Allocates and sets an array buffer for the vertex data*/
		VertexBuffer vertexBuffer(positions, lengthof(positions) * sizeof(float));

		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, bufferLayout);

		/* Allocates and sets an index buffer for the data*/
		IndexBuffer indexBuffer(indices, lengthof(indices));

		/* Projection matrix*/
		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Translates the camera

		Shader shader("resources/shaders/Texture.shader");
		shader.Bind();
		/* Retrives the location of the uniform "u_Color", used in the fragment shader*/
		//shader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);

		Texture texture("resources/textures/Pyramids.png");
		
		unsigned int textureSlot = 0;
		texture.Bind(textureSlot);
		shader.SetUniform1i("u_Texture", textureSlot);

		/* Clears the bound buffers and program/shader*/
		vertexArray.UnBind();
		shader.UnBind();
		vertexBuffer.UnBind();
		indexBuffer.UnBind();

		Renderer renderer;

		// Setup Dear ImGui context
		ImGui::CreateContext();
		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsLight();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		float rgb[] = { 0.0f, 0.0f, 0.0f };
		float increment;
		int channel = 2;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); // Translates the model
				glm::mat4 MVP = projection * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", MVP);

				renderer.Draw(vertexArray, indexBuffer, shader);
			}
			/* Sets the uniform used in the shader with name "u_Color"*/
			//shader.SetUniform4f("u_Color", rgb[0], rgb[1], rgb[2], 1.0f);


			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // Translates the model
				glm::mat4 MVP = projection * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", MVP);

				renderer.Draw(vertexArray, indexBuffer, shader);
			}

			if (rgb[channel] <= 0)
				increment = 0.01f;
			else if (rgb[channel] >= 1.0f)
				increment = -0.01f;
			rgb[channel] += increment;

			// ImGui
			{
				ImGui::Begin("Window");

				ImGui::SliderFloat2("Translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat2("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			// Rendering ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			//int display_w, display_h;
			//glfwMakeContextCurrent(window);
			//glfwGetFramebufferSize(window, &display_w, &display_h);
			//glViewport(0, 0, display_w, display_h);
			//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			//glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
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