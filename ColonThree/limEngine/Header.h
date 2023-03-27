#pragma once
#define _USE_MATH_DEFINES

// standards
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <array>

// openGL
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Key definitions (limited)
#define LiminalKey_esc 0
#define LiminalKey_1 1
#define LiminalKey_2 2
#define LiminalKey_w 12
#define LiminalKey_a 21
#define LiminalKey_s 22
#define LiminalKey_d 23
#define LiminalKey_c 24

// Window type commands
#define LimFullscreen 200
#define LimBottom 301
#define LimMid 302
#define LimTop 303

// For error checking
#define GL(x) \
		x;\
		CheckError(__LINE__, __FILE__)

static void ClearError()
{
	while (glGetError() != GL_NO_ERROR) {};
};

static void CheckError(int line, const char* file)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL ERROR] (" << error << ") on line: " << line << "\tFile: " << file << std::endl;
	};
};




