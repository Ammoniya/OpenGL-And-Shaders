#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <istream>
#include <string>

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	/*Errors hadling*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Faild to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int  CreatShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Ravindu De Silva", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*GLEW TEST*/
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW is not working" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	/*Vertex data*/
	float positions[] = {
	-0.5f,  0.5f, 0.0f, // Top-left
	 0.5f,  0.5f, 0.0f, // Top-right
	 0.5f, -0.5f, 0.0f, // Bottom-right
	-0.5f, -0.5f, 0.0f  // Bottom-left
	};

	/*graphics pipeline*/
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	std::string vertexShader = "#version 330 core\n"
		"layout(location = 0) in vec4 position;\n"
		"void main(){\n"
		"			gl_Position = position;\n"
		"}\n";

	std::string fragmentShader = "#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"void main(){\n"
		"		color = vec4(1.0,1.0,1.0,1.0);\n"
		"}\n";

	unsigned int  shader = CreatShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw polygon[rectangle] by vertex array;
		glDrawArrays(GL_POLYGON, 0, 4);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}