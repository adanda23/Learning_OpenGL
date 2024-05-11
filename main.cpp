#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader 
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader 
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
	//Initalizing GLFW
	glfwInit();
	//Helping GLFW know what version of OPENGL we are using (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Using Core profile for OpenGL, uses modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating the vertices of our triangle
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	//Creating a window with resolution 800x800 with a title
	GLFWwindow* window = glfwCreateWindow(800, 800, "Learning OpenGL", NULL, NULL);
	//If window fails to create, terminate
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Telling GLFW since we created the wiindow, we have to use it in the current context
	glfwMakeContextCurrent(window);


	//Tell openGl the area of the window we want openGL to render in (bottom left to top right coords)
	gladLoadGL();
	glViewport(0, 0, 800, 800);


	//Creating a reference to store our vertex shader in
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Sourcing the shader for our vertexShader, with 1 string, from the vertexShaderSource 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compiile source code to machine code
	glCompileShader(vertexShader);

	//Creating a refernce to store our fragment shader in
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Sourcing the shader for our fragmentShader, with 1 string, from the fragmentShaderSource
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compile source code to machine code
	glCompileShader(fragmentShader);

	//Wrap shaders in a shader program to actually use them
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Wrap up the shader program
	glLinkProgram(shaderProgram);

	//delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Transferring Data from CPU to GPU is slow, so we want to send it in bulk as a buffer (not the same as the front and back buffers)
	//Creating a vertext array object which points to VBOs and tells how to interpret them
	//Creating a vertex buffer object (VBO) with our vertex data
	GLuint VAO, VBO;
	//Make sure to generate VAO always BEFORE VBO
	//Creating a buffer with 1 object and our vertex buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//Binding = making a object the current object, meaning any modifications modify this binded object
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Store our vertices in the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Configure VAO so OpenGL knows how to read it:
	//First pass index of vertex attribute (A way of communnicating with a vertex shader from the outside)
	//Then the # of values in the vertex (3 floats)
	//Then what kind of values
	//Coordinate are not integers so set False
	//Stride of vertices (amount of data between each vertex) 
	//Point to where vertice begin in array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//enable the vertex attribute array
	glEnableVertexAttribArray(0);

	//in order to not change vba or vio with a function
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//We want to clear the back buffer and give it a new color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Swap the front and back buffers
	glfwSwapBuffers(window);


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		//Proccesses all inputs while window is open
		glfwPollEvents();
	}

	//delete all the created objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//Closing Window
	glfwDestroyWindow(window);
	//Terminating GLFW
	glfwTerminate();
	return 0;
}