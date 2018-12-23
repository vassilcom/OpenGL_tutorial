#pragma once

#include <GL/glew.h>
//OpenGL Debuging
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())
void GLClearError();
bool GLLogCall();

class Renderer
{
public:
	Renderer();
	~Renderer();
};

