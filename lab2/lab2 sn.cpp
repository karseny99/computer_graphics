#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <GL/glu.h> 
#include <cmath>

float angleX = 0.0f;
float angleY = 0.0f;
float fov = 45.0f;

void drawPyramid() {
    glBegin(GL_TRIANGLES);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(-1.0f, 0.0f, -1.0f); 
    glVertex3f(1.0f, 0.0f, -1.0f); 

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(1.0f, 0.0f, -1.0f); 
    glVertex3f(1.0f, 0.0f, 1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(1.0f, 0.0f, 1.0f); 
    glVertex3f(-1.0f, 0.0f, 1.0f); 

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f); 

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glEnd();
}

int main() {
    sf::Window window(sf::VideoMode(800, 600), "3D Pyramid", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    
    window.setActive(true);
    glEnable(GL_DEPTH_TEST);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) angleY -= 5.01f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angleY += 5.01f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) angleX -= 5.01f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) angleX += 5.01f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) fov -= 1.01;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) fov += 1.01;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        gluPerspective(fov, (float)800 / (float)600, 1.0, 100.0);
        
        gluLookAt(5 * cos(angleX * M_PI / 180) * sin(angleY * M_PI / 180), 10*sin(angleX * M_PI / 180), 
                  5 * cos(angleX * M_PI / 180) * cos(angleY * M_PI / 180), 
                  0, 0, 0,
                  0, 1, 0);

        drawPyramid();

        window.display();
    }

    return 0;
}