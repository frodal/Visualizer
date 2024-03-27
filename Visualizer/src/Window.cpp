#include "PreCompiledHeader.h"
#include "Window.h"

static bool isGLFWInitialized = false;

Window::Window(const WindowProps& props)
	: window(nullptr), windowprops(props)
{
	Init(props);
}

Window::~Window()
{
	Shutdown();
}

void Window::OnUpdate()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	windowprops.Vsync = enabled;
}

double Window::GetDeltaTime()
{
	/* Gets the time since last call to this function. 
	   This might be unwanted. 
	   TODO: Do this in the OnUpdate call? */
	double oldTime = timeLastGetDeltaTime;
	timeLastGetDeltaTime = glfwGetTime();
	return timeLastGetDeltaTime - oldTime;
}

void Window::TerminateWindow()
{
	glfwTerminate();
}

void Window::Init(const WindowProps& props)
{
	/* Initialize the GLFW library */
	if (!isGLFWInitialized) 
	{
		if (!glfwInit())
		{
			std::cout << "Error: Could not set up the GLFW library!" << std::endl;
			ASSERT(false);
			isGLFWInitialized = true;
		}
	}

	/* Sets the version of OpenGL to use and sets the openGL profile*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
	if (!window)
	{
		std::cout << "Error: Could not set up a window with an openGL context!" << std::endl;
		glfwTerminate();
		ASSERT(false);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Sets Vsync based on input */
	SetVSync(props.Vsync);

	/* Sets a user pointer to be used with GLFW*/
	glfwSetWindowUserPointer(window, &windowprops);

	/* Set GLFW Callbacks*/
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow * window, int width, int height)
	{
		WindowProps& props = *static_cast<WindowProps*>(glfwGetWindowUserPointer(window));
		props.Width = width;
		props.Height = height;
		glViewport(0, 0, width, height);
	});

	/* Initialize the GLEW library (Needs a valid OpenGL rendering context)*/
	GLenum message = glewInit();
	if (message != GLEW_OK)
	{
		std::cout << "Error: Could not set up the GLEW library!" << std::endl;
		std::cout << glewGetErrorString(message) << std::endl;
		glfwTerminate();
		ASSERT(false);
	}

#ifdef DEBUG
	/* Prints the current versions used to the console*/
	std::cout << "Current GLFW version:   " << glfwGetVersionString() << std::endl;
	std::cout << "Current GLEW version:   " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Current OpenGL version: " << glGetString(GL_VERSION) << std::endl;
#endif // DEBUG
}

void Window::Shutdown()
{
	/* Closes and destroys the GLFW window*/
	glfwDestroyWindow(window);
}
