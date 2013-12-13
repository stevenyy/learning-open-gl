/*
 * Hello Triangle with OpenGL on OSX
 * http://antongerdelan.net/opengl/hellotriangle.html
 *
 */

#include<cstdio>
#include<fstream>
#include<iostream>

#include<OpenGL/gl3.h>
#include<GLFW/glfw3.h>

using namespace std;

char* loadShaderFromFile(char *fileName) {
	std::ifstream is(fileName, std::ifstream::binary);

	// TODO: Need to check if file is non-existant and for read errors

	/* Get length of file */
	is.seekg(0, is.end);
	int length = is.tellg();
	is.seekg(0, is.beg);

	char *buffer = new char[length];
	/* Read data as a block */
	is.read(buffer, length);

	is.close();

	buffer[length - 1] = '\0'; // Needed to correctly format file (shaders mess up otherwise)

	return buffer;
}

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* OSX specific attributes */
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LESS); // Depth-testing interprets smaller value as closer

	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	/* Create vertex buffer object */
	unsigned int points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	/* Create vertex attribute object */
	unsigned int vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // Assign index 0 the points

	/* Enable attributes of currently bound vertex array object */
	glEnableVertexAttribArray(0);

	/* Load shader programs from text files */
	char* vertex_shader = loadShaderFromFile("../res/vertex_shader.glsl");
	char* fragment_shader = loadShaderFromFile("../res/fragment_shader.glsl");

	/* Generate pointers and compile shaders */
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	/* Create shader program and attach shaders */
	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);

	glLinkProgram(shader_program); // Link shader program

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
