#include "config.h"
#include <iostream>


void drawRect() {
	glBegin(GL_QUADS);
	glColor3f(0, 1.0, 1.0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(-0.5, -0.5);
	glEnd();
	glFlush();
}


int main() {
	
	GLFWwindow* window;

	if (!glfwInit()) {
        std::cerr << "glfw failed" << std::endl;
		return -1;
	}
	
	window = glfwCreateWindow(640, 480, "321", nullptr, nullptr);
	glfwMakeContextCurrent(window);
    if (!window) {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(window);

	glClearColor(0, 0, 0, 0);
       
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

        drawRect();
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}