#pragma once

#include <GL/glew.h> // See http://glew.sourceforge.net/ for the documentation and to download the newest version!
#include <GLFW/glfw3.h> // See https://www.glfw.org/ for the documentation and to download the newest version!

#include <iostream> // Standard I/O library for C++
#include <fstream> // Used for reading files such as the vertex and fragment shader source
#include <string> // Standard string library for C++
#include <sstream> // Standard String stream library for C++

template<typename T, std::size_t N>
constexpr std::size_t lengthof(T(&)[N]) { return N; }


