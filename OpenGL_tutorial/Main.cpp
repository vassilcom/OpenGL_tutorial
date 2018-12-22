#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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