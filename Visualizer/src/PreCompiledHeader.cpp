#include "PreCompiledHeader.h"

// Error checking functions to be used with OpenGL in Debug
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

// A standing wave function (unit wave with wave length and height = 1, approximation of abs(sin(pi * value)))
float StandingWave(float value)
{
	int a = static_cast<int>(value);
	float x = value - a;
	return 4.0f * x * (1 - x);

}