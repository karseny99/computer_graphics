#include "config.h"
#include <iostream>


const int width = 1280;
const int height = 720;

GLfloat rect_x = 0;
GLfloat rect_y = 0;
GLfloat speed_x = 0.001;
GLfloat speed_y = 0.003;

GLfloat aspect = (GLfloat)width / (GLfloat)height;

GLfloat maxXrect = aspect - 1.25;
GLfloat minXrect = -aspect + 1.25;
GLfloat maxYrect = .75;
GLfloat minYrect = -.75;

GLfloat angle = 0;

GLfloat scale_x = 1, scale_y = 1;

bool mode = false;

void drawRect() {
	/*
		Draws a simple rectangle
	*/

	glBegin(GL_QUADS);
	glColor3f(0, 1.0, 1.0);
	glVertex2f(-0.5, 0.25);
	glVertex2f(0.5, 0.25);
	glVertex2f(0.5, -0.25);
	glVertex2f(-0.5, -0.25);
	glEnd();
	glFlush();
}
void drawMovingRect() {
	/*
		Draws a moving, rotating 'n' scaling rectangle 
	*/

	glLoadIdentity();
	glTranslatef(rect_x, rect_y, 0);
	glScalef(scale_x, scale_y, 1);
	glRotatef(angle, rect_x, rect_x, 1);

	glBegin(GL_QUADS);
	glColor3f(0, 1.0, 1.0);
	glVertex2f(-0.5, 0.25);
	glVertex2f(0.5, 0.25);
	glVertex2f(0.5, -0.25);
	glVertex2f(-0.5, -0.25);

	glEnd();

	if(mode) {
		rect_x += speed_x;
		rect_y += speed_y;
		angle += 0.003;
		scale_x = sin(angle);
		scale_y = cos(angle);
	}

	if(rect_x > maxXrect or rect_x < minXrect) {
		speed_x = -speed_x;
		rect_x = (rect_x > maxXrect) ? maxXrect : minXrect;
	}
	
	if(rect_y > maxYrect or rect_y < minYrect){
		speed_y = -speed_y;
		rect_y = (rect_y > maxYrect) ? maxYrect : minYrect;
	}

	glFlush();
}

void keyPressed (GLFWwindow* window, int key, int scancode, int action, int mods) { 
	if(action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			rect_x -= 0.05;
		} else if(key == GLFW_KEY_RIGHT) {
			rect_x += 0.05;
		} else if(key == GLFW_KEY_UP) {
			rect_y += 0.05;
		} else if(key == GLFW_KEY_DOWN) {
			rect_y -= 0.05;
		} else if(key == GLFW_KEY_R) {
			angle += 0.7;
		} else if(key == GLFW_KEY_S) {
			scale_x -= 0.5;
			scale_y -= 0.5;
		} else if(key == GLFW_KEY_W) {
			scale_x += 0.5;
			scale_y += 0.5;
		} else if(key == GLFW_KEY_M) {
			mode = !mode;
		}
	}
}

int main() {
	
	GLFWwindow* window;

	if (!glfwInit()) {
        std::cerr << "glfw failed" << std::endl;
		return -1;
	}
	
	window = glfwCreateWindow(width, height, "321", nullptr, nullptr);
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

	glfwSetKeyCallback(window, keyPressed);

    glfwMakeContextCurrent(window);
	glClearColor(0, 0, 0, 0);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
        drawMovingRect();
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}