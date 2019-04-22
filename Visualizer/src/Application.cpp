/*
This is the main .cpp file for the Visulazier application using OpenGL to render stuff.
See the documentation of OpenGL, e.g., http://docs.gl/
*/
#include "PreCompiledHeader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"



struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::fstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		_freea(message);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

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

		VertexBufferLayout layout;
		layout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, layout);

		/* Allocates and sets an index buffer for the data*/
		IndexBuffer indexBuffer(indices, lengthof(indices));

		ShaderProgramSource source = ParseShader("resources/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCall(glUseProgram(shader));

		/* Retrives the location of the uniform "u_Color", used in the fragment shader*/
		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != 1);

		/* Clears the bound buffers and program/shader*/
		vertexArray.UnBind();
		GLCall(glUseProgram(0));
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
			GLCall(glUseProgram(shader));
			/* Sets the uniform used in the shader with location "location"*/
			GLCall(glUniform4f(location, rgb[0], rgb[1], rgb[2], 1.0f));

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

		/* Deletes the shader from the GPU*/
		GLCall(glDeleteProgram(shader));
	}
	/* Closes and destroys the GLFW window*/
	glfwDestroyWindow(window);

	/* Terminates the GLFW context*/
	glfwTerminate();
	return 0;
}