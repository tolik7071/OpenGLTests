#include "pch.h"
#include <iostream>
#include "GLUtils.h"

static void error_callback(int error, const char* description);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

static char vertex_shader_source[] =
"#version 450 core										\n"
"														\n"
"layout(location = 0) in vec3 pos;						\n"
"														\n"
"void main()											\n"
"{														\n"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);		\n"
"}														\n";

static char fragment_shader_source[] =
"#version 450 core										\n"
"														\n"
"out vec4 FragColor;									\n"
"														\n"
"void main()											\n"
"{														\n"
"	FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);			\n"
"}														\n";

int main(int argc, char** argv)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	GLFWwindow *window = glfwCreateWindow(640, 480, "Testing...", NULL, NULL);
	if (!window)
	{
		glfwTerminate();

		std::cerr << "Failed to open GLFW window" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	GLuint vertexShader = GLUtils::CreateShader(GL_VERTEX_SHADER, vertex_shader_source);
	GLuint fragmentShader = GLUtils::CreateShader(GL_FRAGMENT_SHADER, fragment_shader_source);

	GLuint program = GLUtils::CreateProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	while (!glfwWindowShouldClose(window))
	{
		std::cout << glfwGetTime() << std::endl;

		glClearColor(0.2f, 0.3f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(program);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	}
	}
}
