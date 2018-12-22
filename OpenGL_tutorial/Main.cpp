#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

std::string SringFromFile(const std::string& file_link)
{
	std::ifstream ifs(file_link);
	std::string s;
	std::getline(ifs, s, (char)ifs.eof());
	return s;
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile"<<
			(type==GL_VERTEX_SHADER? "vertex": "fragment")
			<<" Shader"<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;

	}

	return id;
}
static unsigned int CreateShader(const std::string& vertexSchader, const std::string& fragmentSchader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSchader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSchader);

	glAttachShader(program,vs);
	glAttachShader(program,fs);
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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "glew error" << std::endl;
	else 
		std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[6] =
	{
		-0.5f, -0.5f,
		 0.0f, 0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer;//id for buffer
	glGenBuffers(1, &buffer);//assigns id
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//selects buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);//set size f buffer, static or dinamic, and give a data, that is oprional (can be NULL, and data can be assigned later)

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);//aattribute, how many variables, data type, no normalize, size of one vertex, offset where attribute starts 
	



	unsigned int shader = CreateShader(SringFromFile("res/vertexShader.shader"), SringFromFile("res/fragmentShader.shader"));
	glUseProgram(shader);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

	
		//since we dont have index buffer yet:
		glDrawArrays(GL_TRIANGLES,0,3);//type, start position, number of vertexes


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}