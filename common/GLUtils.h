#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLUtils
{
	public:
		static GLuint CreateShader(GLenum shaderType, char data[]);
		static GLuint CreateProgram(GLenum vertexShader, GLenum fragmentShader);
};
