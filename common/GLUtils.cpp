#include "pch.h"
#include "GLUtils.h"
#include <iostream>

/*static*/ GLuint GLUtils::CreateShader(GLenum shaderType, char data[])
{
	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &data, NULL);

	glCompileShader(shader);

	GLint success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (0 == success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "SHADER COMPILATION FAILED\n" << infoLog << std::endl;

		if (shader != 0)
		{
			glDeleteShader(shader);
			shader = 0;
		}
	}

	return shader;
}

/*static*/ GLuint GLUtils::CreateProgram(GLenum vertexShader, GLenum fragmentShader)
{
	GLuint program = glCreateProgram();
	if (0 != program)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (0 == success)
		{
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "CANNOT LINK PROGRAM\n" << infoLog << std::endl;
		}
	}
	else
	{
		std::cout << "CANNOT CREATE PROGRAM" << std::endl;
	}

	return program;
}

/*static */std::filesystem::path GLFile::GetFullPathForResource(const std::string& resource)
{
	return (std::filesystem::current_path() / resource);
}
