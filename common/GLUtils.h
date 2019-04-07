#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

class GLUtils
{
	public:
		static GLuint CreateShader(GLenum shaderType, char data[]);
		static GLuint CreateProgram(GLenum vertexShader, GLenum fragmentShader);
};

class GLMath
{
	public:
		static inline double PI()
		{
			return std::atan(1) * 4.0;
		}
		
		static double DegreesToRadians(double degrees)
		{
			double result = degrees * GLMath::PI() / 180.0;
			return result;
		}

		static double RadiansToDegrees(double radians)
		{
			double result = radians * 180.0 / GLMath::PI();
			return result;
		}
};
