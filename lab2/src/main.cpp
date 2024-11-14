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

GLfloat angle = 0, angleX = 0, angleY = 0, angleZ = 0;

GLfloat scale_x = 1, scale_y = 1;

bool mode = false;

GLfloat vertices[] = {
	1.0f,	1.0f,	0.0f,
	-1.0f,	1.0f,	0.0f,
	-1.0f,	-1.0f,	0.0f,
	1.0f,	-1.0f,	0.0f,
	0.0f,	0.0f,	1.0f
};

void displayPyramid() {
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(0.3, 0.3, 0.3);
	// glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(angle, 1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);

	glColor3f(0.2f, 0.9f, 0.6f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	
	glColor3f(0.7f, 1.0f, 0.9f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	
	glColor3f(0.4f, 1.0f, 0.3f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	
	glColor3f(1.0f, 0.9f, 0.3f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// glColor3f(1.0f, 1.0f, 1.0f);
	// glVertex3f(1.0f, 1.0f, 0.0f);
	// glVertex3f(1.0f, -1.0f, 0.0f);
	// glVertex3f(-1.0f, 1.0f, 0.0f);
	// glVertex3f(-1.0f, -1.0f, 0.0f);

	glEnd();
	angle += 0.01;
	
}

void show() {
	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &vertices);
		glColor3f(0, 0.5, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void moveCamera() {

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);
	glTranslatef(0, 0, -3);
}

void keyPressed (GLFWwindow* window, int key, int scancode, int action, int mods) { 
	if(action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			angleX = ++angleX > 180 ? 180 : angleX;
		} else if(key == GLFW_KEY_RIGHT) {
			angleX = --angleX > 180 ? 180 : angleX;
		} else if(key == GLFW_KEY_UP) {
			angleZ = ++angleZ > 180 ? 180 : angleZ;
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
	
	window = glfwCreateWindow(width, height, "pyramid", nullptr, nullptr);
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

	glFrustum(-1,1, -1,1, 2,8);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
        // displayPyramid();
		glVertexPointer(3, GL_FLOAT, 0, &vertices);
		glEnableClientState(GL_VERTEX_ARRAY);

		glPushMatrix();
			moveCamera();
			show();
		glPopMatrix();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}