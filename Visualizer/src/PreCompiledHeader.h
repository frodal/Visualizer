#pragma once

#include <GL/glew.h> // See http://glew.sourceforge.net/ for the documentation and to download the newest version!
#include <GLFW/glfw3.h> // See https://www.glfw.org/ for the documentation and to download the newest version!

#include <iostream> // Standard I/O library for C++
#include <fstream> // Used for reading files such as the vertex and fragment shader source
#include <string> // Standard string library for C++
#include <sstream> // Standard String stream library for C++
#include <vector> // Standard vector library for C++
#include <unordered_map> // Standard C++ library

#include "glm/glm/glm.hpp" // OpenGL Math library, see https://glm.g-truc.net/0.9.9/index.html
#include "glm/glm/gtc/matrix_transform.hpp" // OpenGL Math library



template<typename T, std::size_t N>
constexpr std::size_t lengthof(T(&)[N]) { return N; }

/* Defines some error checking macros to be used with OpenGL*/
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


