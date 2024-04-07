#pragma once

#ifndef DEBUG
#include <Windows.h>
#endif // !DEBUG


// Helper libraries for creating a OpenGL contex windows and retriving OpenGL functions from the graphics driver
#include <GL/glew.h> // See http://glew.sourceforge.net/ for the documentation and to download the newest version!
#include <GLFW/glfw3.h> // See https://www.glfw.org/ for the documentation and to download the newest version!

// Standard C++ libraries
#include <iostream> // Standard I/O library
#include <fstream> // Used for reading files such as the vertex and fragment shader source
#include <string> // Standard string library
#include <sstream> // Standard String stream library
#include <vector> // Standard vector library
#include <array> // Standard array library
#include <unordered_map> // Standard unordered map library
#include <functional> // Standard functional library
#include <thread> // Standard threading library
#include <future> // Standard library part of the thread support library

// OpenGL Math library, see https://glm.g-truc.net/0.9.9/index.html
#define GLM_FORCE_MESSAGES
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// Dear ImGui: Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies, see https://github.com/ocornut/imgui
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

// Handy template to get the length of an array
template<typename T, std::size_t N>
constexpr std::size_t lengthof(T(&)[N]) { return N; }

// Defines some error checking macros to be used with OpenGL
#ifdef DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define ASSERT(x)
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// A standing wave function (unit wave with wave length and height = 1, approximation of abs(sin(pi * value)))
float StandingWave(float value);
