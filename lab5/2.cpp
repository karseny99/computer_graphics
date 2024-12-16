#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

void CheckShaderCompileErrors(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shader << "\n" 
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}


class Game {
public:
    Game(int width, int height, const char* title);
    void Run();

private:
    void Init();
    void GenerateSphere(float radius, int latitudeBands, int longitudeBands);
    void InitializeSphere();
    void InitializePlane();
    void Render();
    void ProcessInput(GLFWwindow* window);
    GLuint CreateShaderProgram();

    GLFWwindow* window;
    GLuint sphereVAO, sphereVBO, sphereEBO;
    GLuint planeVAO, planeVBO, planeEBO;
    GLuint shaderProgram;

    glm::mat4 modelSphere;
    glm::mat4 modelPlane;

    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
    float yaw, pitch;
    float sensitivity;

    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

    std::vector<float> planeVertices = {
            -1.0f, 0.0f, -1.0f, 0.4f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 
             1.0f, 0.0f, -1.0f, 0.4f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
             1.0f, 0.0f,  1.0f, 0.4f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 0.0f,  1.0f, 0.4f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f 
    };

    std::vector<unsigned int> planeIndices = {
        0, 1, 2,
        0, 2, 3
    };
};

Game::Game(int width, int height, const char* title) 
    : cameraPosition(0.0f, 0.0f, 5.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f),
      cameraSpeed(2.5f), yaw(-90.0f), pitch(0.0f), sensitivity(0.04f) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_DEPTH_TEST);
}

void Game::Init() {
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST); 

    GenerateSphere(0.5f, 20, 20);
    InitializeSphere();
    InitializePlane();

    modelSphere = glm::mat4(1.0f);
    modelPlane = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));

    shaderProgram = CreateShaderProgram();
}


void Game::GenerateSphere(float radius, int latitudeBands, int longitudeBands) {
    for (int lat = 0; lat <= latitudeBands; lat++) {
        float theta = lat * M_PI / latitudeBands; // Угол по вертикали
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        for (int lon = 0; lon <= longitudeBands; lon++) {
            float phi = lon * 2 * M_PI / longitudeBands; // Угол по горизонтали
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            // Вычисление координат
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            // Добавление координат
            sphereVertices.push_back(radius * x); // x
            sphereVertices.push_back(radius * y); // y
            sphereVertices.push_back(radius * z); // z

            // Добавление нормалей
            sphereVertices.push_back(x); // Нормали
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);

            // Добавление цвета (например, белый)
            sphereVertices.push_back(0.5f); // R
            sphereVertices.push_back(.6f); // G
            sphereVertices.push_back(.3f); // B
        }
    }

    // Генерация индексов для отрисовки треугольников
    for (int lat = 0; lat < latitudeBands; lat++) {
        for (int lon = 0; lon < longitudeBands; lon++) {
            int first = (lat * (longitudeBands + 1)) + lon; // Индекс первой вершины
            int second = first + longitudeBands + 1; // Индекс второй вершины

            // Добавление индексов для первого треугольника
            sphereIndices.push_back(first);
            sphereIndices.push_back(second);
            sphereIndices.push_back(first + 1);

            // Добавление индексов для второго треугольника
            sphereIndices.push_back(second);
            sphereIndices.push_back(second + 1);
            sphereIndices.push_back(first + 1);
        }
    }
}

void Game::InitializeSphere() {
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);

    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::InitializePlane() {
    GLuint planeVBO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);

    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(float), planeVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &planeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeIndices.size() * sizeof(unsigned int), planeIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::Render() {
    // Очистка буферов цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем как цветовой, так и глубинный буфер
    glUseProgram(shaderProgram);

    // Установка матриц
    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Установка uniform-переменных
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Установка параметров освещения
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 1.0f, 1.0f, 1.0f); // Позиция источника света
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f); // Цвет света
    glUniform1f(glGetUniformLocation(shaderProgram, "lightSize"), 1.0f); // Размер света
    glUniform3f(glGetUniformLocation(shaderProgram, "sphereCenter"), 0.0f, 0.0f, 0.0f); // Центр сферы
    glUniform1f(glGetUniformLocation(shaderProgram, "sphereRadius"), 0.5f); // Радиус сферы
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowSamples"), 16); // Количество образцов для теней
    glUniform1i(glGetUniformLocation(shaderProgram, "is_plane"), true); // Установите true, если это плоскость

    // Отрисовка плоскости
    glBindVertexArray(planeVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelPlane));
    glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);
    // Отрисовка сферы
    glBindVertexArray(sphereVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelSphere));
    glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);


    // Сброс привязки VAO
    glBindVertexArray(0);

    // Обновление окна
    glfwSwapBuffers(window);
}


void Game::ProcessInput(GLFWwindow* window) {
    float deltaTime = 0.1f;
    float cameraSpeed = this->cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPosition.y += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPosition.y -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint Game::CreateShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        layout(location = 2) in vec3 aNormal;

        out vec3 ourColor;
        out vec3 FragPos;
        out vec3 Normal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            ourColor = aColor;
        }
    )";
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompileErrors(vertexShader);

    // Создание фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 ourColor;
        in vec3 FragPos;
        in vec3 Normal;

        out vec4 FragColor;

        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 lightColor;
        uniform float lightSize;
        uniform vec3 sphereCenter;
        uniform float sphereRadius;
        uniform int shadowSamples;
        uniform bool is_plane;

        struct Ray {
            vec3 origin;
            vec3 direction;
        };

        float rand() {
            return fract(sin(dot(gl_FragCoord.xy, vec2(12.9898, 78.233))) * 43758.5453);
        }

        bool RayIntersectsSphere(Ray ray, vec3 sphereCenter, float sphereRadius) {
            vec3 oc = ray.origin - sphereCenter;
            float a = dot(ray.direction, ray.direction);
            float b = 2.0 * dot(oc, ray.direction);
            float c = dot(oc, oc) - sphereRadius * sphereRadius;
            float discriminant = b * b - 4 * a * c;
            return (discriminant > 0);
        }

        vec3 randPointOnLight(vec3 lightPos, float lightSize) {
            float halfSize = lightSize / 2.0;
            float randomX = (rand() - 0.5) * lightSize;
            float randomY = (rand() - 0.5) * lightSize;
            return lightPos + vec3(randomX, randomY, 0.0);
        }

        float SoftShadow(vec3 fragPos, vec3 lightPos, float lightSize, int samples) {
            float shadow = 0.0;
            for (int i = 0; i < samples; i++) {
                vec3 randomPointOnLight = randPointOnLight(lightPos, lightSize);
                Ray shadowRay;
                shadowRay.origin = fragPos;
                shadowRay.direction = normalize(randomPointOnLight - fragPos);

                if (RayIntersectsSphere(shadowRay, sphereCenter, sphereRadius)) {
                    shadow += 0.5;
                }
            }
            return shadow / float(samples);
        }

        void main() {
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            // Расчет мягкой тени
            float shadow = SoftShadow(FragPos, lightPos, lightSize, shadowSamples);
            vec3 result = diffuse * ourColor;

            // Если это плоскость, учитываем тени
            if (is_plane) {
                result *= (1.0 - shadow); // Уменьшаем яркость в зависимости от тени
            }

            FragColor = vec4(result, 1.0);
        }
    )";
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompileErrors(fragmentShader);

    // Создание шейдерной программы
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Удаление шейдеров, так как они уже связаны с программой
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Game::Run() {
    Init();
    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);
        Render();
        glfwPollEvents();
    }
    glfwTerminate();
}

int main() {
    Game game(800, 600, "OpenGL Game");
    game.Run();
    return 0;
}
