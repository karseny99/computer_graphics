#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

float cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = 5.0f; // Позиция камеры
float cameraAngleX = 0.0f; 
float cameraAngleY = 0.0f; 
const float moveSpeed = 0.1f; // Скорость перемещения камеры

void drawCube() {
    glBegin(GL_QUADS);
    
    // Передняя грань
    glColor3f(1.0, 0.0, 0.0); // Красный
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);

    // Задняя грань
    glColor3f(0.0, 1.0, 0.0); // Зеленый
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);

    // Левая грань
    glColor3f(0.0, 0.0, 1.0); // Синий
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);

    // Правая грань
    glColor3f(1.0, 1.0, 0.0); // Желтый
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);

    // Верхняя грань
    glColor3f(0.5, 0.5, 0.5); // Серый
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);

    // Нижняя грань
    glColor3f(1.0, 0.5, 0.5); // Розовый
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);

    glEnd();
}

void drawPyramid() {
    glBegin(GL_TRIANGLES);

    // Передняя грань
    glColor3f(1.0, 0.0, 0.0); // Красный
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);

    // Правая грань
    glColor3f(0.0, 1.0, 0.0); // Зеленый
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);

    // Задняя грань
    glColor3f(0.0, 0.0, 1.0); // Синий
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);

    // Левая грань
    glColor3f(1.0, 1.0, 0.0); // Желтый
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);

    glEnd();

    glBegin(GL_QUADS);
    // Основание
    glColor3f(0.5, 0.5, 0.5); // Серый
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glEnd();
}

void drawSphere(float radius, int slices, int stacks) {
    GLUquadric* quadric = gluNewQuadric();
    gluSphere(quadric, radius, slices, stacks);
    gluDeleteQuadric(quadric);
}

void handleInput(sf::Window& window) {
    // Перемещение камеры
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cameraPosZ -= moveSpeed; // Вперед
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraPosZ += moveSpeed; // Назад
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cameraPosX -= moveSpeed; // Влево
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraPosX += moveSpeed; // Вправо
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) cameraPosY += moveSpeed; // Вверх
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) cameraPosY -= moveSpeed; // Вниз
}

// Основная функция
int main() {
    // Создание окна
    sf::Window window(sf::VideoMode(800, 600), "3D Objects", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);

    // Настройка OpenGL
    glEnable(GL_DEPTH_TEST); // Включение глубинного теста
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Основной цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Обработка ввода
        handleInput(window);

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glLoadIdentity();
        gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 
                  cameraPosX, cameraPosY, cameraPosZ - 1.0f, 
                  0.0f, 1.0f, 0.0f);

        // Отрисовка объектов
        glPushMatrix();
        glTranslatef(-2.0f, 0.0f, 0.0f); // Сдвиг для куба
        drawCube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f); // Пирамида в центре
        drawPyramid();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.0f, 0.0f, 0.0f); // Сдвиг для сферы
        drawSphere(1.0f, 20, 20); // Сфера радиусом 1.0
        glPopMatrix();

        // Обновление окна
        window.display();
    }

}
