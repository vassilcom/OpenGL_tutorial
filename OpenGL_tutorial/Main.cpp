#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

//OpenGL Debuging
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())
static void GLClearError() 
{
	while (glGetError()!=GL_NO_ERROR);
}
static bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]"<< error << std::endl;
		return false;
	}
	return true;
}


std::string StringFromFile(const std::string& file_link)
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
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

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
	GLCall(unsigned int program = glCreateProgram());
	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSchader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSchader));
	
	GLCall(glAttachShader(program,vs));
	GLCall(glAttachShader(program,fs));
	GLCall(glLinkProgram(program));	 
	GLCall(glValidateProgram(program));
	
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//adding core proile without default Vertex Arrays
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "glew error" << std::endl;
	else 
		std::cout << glGetString(GL_VERSION) << std::endl;

	
	float positions[] =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};
	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	
	unsigned int buffer;//id for buffer
	GLCall(glGenBuffers(1, &buffer));//assigns id
	//bind vertex buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));//selects buffer
	GLCall(glBufferData(GL_ARRAY_BUFFER, (sizeof(positions) / sizeof(*positions)) * sizeof(float), positions, GL_STATIC_DRAW));//set size f buffer, static or dinamic, and give a data, that is oprional (can be NULL, and data can be assigned later)

	GLCall(glEnableVertexAttribArray(0));
	//setup vertex layout
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));//aattribute, how many variables, data type, no normalize, size of one vertex, offset where attribute starts 
	
	
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	//bind index buffer 
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(indices) / sizeof(*indices)) * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	
	GLCall(unsigned int shader = CreateShader(StringFromFile("res/vertexShader.shader"), StringFromFile("res/fragmentShader.shader")));
	//bind shader
	GLCall(glUseProgram(shader));

	int location = glGetUniformLocation(shader, "u_Color");
	
	//temporary unbinding
	GLCall(glUseProgram(0));//unbind shader
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));//unbind index buffer 
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));//unbind vertex buffer
	GLCall(glBindVertexArray(0));//unbind vertex Array

	/* Loop until the user closes the window */
	float n = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		n =n + 0.01f;
		float animNormalizedFloat = (float)((int)(n*100.0f) % 100) / 100.0f;

		GLCall(glUseProgram(shader));
		glUniform4f(location, animNormalizedFloat, .0f, .0f, 1.0f);
		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		//draw call 	
		//since we dont have index buffer yet:
		GLCall(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, nullptr));//type, number of indexes, data type, pointer to index buffer


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}