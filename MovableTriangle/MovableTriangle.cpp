#include "pch.h"
#include <iostream>
#include "GLUtils.h"
#include "FPSCounter.h"
#include <array>

static void error_callback(int error, const char* description);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

typedef std::array<float, 9> TArrayOfVertices;

enum class RotationDirection
{
	kRotateAroundXAxis,
	kRotateAroundYAxis
};

static TArrayOfVertices RotatedTriangle(
	const TArrayOfVertices& vertices,
	TArrayOfVertices::value_type angleInDegrees,
	RotationDirection direction = RotationDirection::kRotateAroundXAxis)
{
	TArrayOfVertices result =
	{
		TArrayOfVertices::value_type(), TArrayOfVertices::value_type(), TArrayOfVertices::value_type(),
		TArrayOfVertices::value_type(), TArrayOfVertices::value_type(), TArrayOfVertices::value_type(),
		TArrayOfVertices::value_type(), TArrayOfVertices::value_type(), TArrayOfVertices::value_type()
	};

	const TArrayOfVertices::value_type angleInRadians = (TArrayOfVertices::value_type)GLMath::DegreesToRadians(angleInDegrees);

	for (int i = 0; i < vertices.size(); i = i + 3)
	{
		if (RotationDirection::kRotateAroundYAxis == direction)
		{
			result[i] = vertices[i] * std::cos(angleInRadians);
			result[i + 1] = vertices[i + 1];
		}
		else
		{
			result[i] = vertices[i];
			result[i + 1] = vertices[i + 1] * std::sin(angleInRadians);
		}

		result[i + 2] = vertices[i + 2];
	}

	return result;
}

static TArrayOfVertices MovedTriangle(const TArrayOfVertices& vertices)
{
	TArrayOfVertices result =
	{
		TArrayOfVertices::value_type(), TArrayOfVertices::value_type(), TArrayOfVertices::value_type(),
		TArrayOfVertices::value_type(), TArrayOfVertices::value_type(), TArrayOfVertices::value_type(),
		TArrayOfVertices::value_type(), TArrayOfVertices::value_type(), TArrayOfVertices::value_type()
	};

	static TArrayOfVertices::value_type delta = 0.003f;
	static TArrayOfVertices::value_type offset = TArrayOfVertices::value_type();

	for (int i = 0; i < vertices.size(); i = i + 3)
	{
		// Move along X-axis
		result[i] = vertices[i] + offset;
		result[i + 1] = vertices[i + 1];
		result[i + 2] = vertices[i + 2];
	}

	if (result[3] > 1.0f || result[0] < -1.0f)
	{
		delta *= -1.0f;
	}

	offset += delta;

	return result;
}

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

	TArrayOfVertices vertices =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Data is copied below

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	FPSCounter counter;

	while (!glfwWindowShouldClose(window))
	{
		counter.increase();
		std::cout << counter.fps() << " fps" << std::endl;

		glClearColor(0.2f, 0.3f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		{
			float angle = 0.0;
			std::modf((float)(glfwGetTime() * 50.0), &angle);
			TArrayOfVertices movedTriangle = MovedTriangle(vertices);
			TArrayOfVertices rotatedVertices = RotatedTriangle(movedTriangle, angle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rotatedVertices), rotatedVertices.data(), GL_STREAM_DRAW);
		}

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
	{
		return;
	}

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}
	}
}
