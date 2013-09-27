#ifndef GLFW_INCLUDE_HPP
#define GLFW_INCLUDE_HPP

#include <glfw3.h>

#ifdef _WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#else
#	define GLFW_EXPOSE_NATIVE_X11
#	define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <glfw3native.h>

#endif