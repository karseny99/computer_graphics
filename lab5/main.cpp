#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

GLuint sphereVAO, sphereVBO, sphereEBO;
GLuint planeVAO, planeVBO, planeEBO;
GLuint shaderProgram;

glm::mat4 modelSphere;
glm::mat4 modelPlane;

glm::vec3 cameraPosition = {0.0f, 0.0f, 3.0f};
glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f};
glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};

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

void get_sphere(float radius, int latitudeBands, int longitudeBands) {
    for (int lat = 0; lat <= latitudeBands; lat++) {
        float theta = lat * M_PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        for (int lon = 0; lon <= longitudeBands; lon++) {
            float phi = lon * 2 * M_PI / longitudeBands; 
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            sphereVertices.push_back(radius * x); 
            sphereVertices.push_back(radius * y); 
            sphereVertices.push_back(radius * z); 

            sphereVertices.push_back(x); 
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);

            sphereVertices.push_back(0.5f); 
            sphereVertices.push_back(.6f);
            sphereVertices.push_back(.3f); 
        }
    }

    for (int lat = 0; lat < latitudeBands; lat++) {
        for (int lon = 0; lon < longitudeBands; lon++) {
            int first = (lat * (longitudeBands + 1)) + lon; 
            int second = first + longitudeBands + 1;

            sphereIndices.push_back(first);
            sphereIndices.push_back(second);
            sphereIndices.push_back(first + 1);

            sphereIndices.push_back(second);
            sphereIndices.push_back(second + 1);
            sphereIndices.push_back(first + 1);
        }
    }
}

void sphere_init() {
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

void plane_init() {
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

void render(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "lightSize"), 1.0f); 
    glUniform3f(glGetUniformLocation(shaderProgram, "sphereCenter"), 0.0f, 0.0f, 0.0f); 
    glUniform1f(glGetUniformLocation(shaderProgram, "sphereRadius"), 0.5f); 
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowSamples"), 16); 

    glBindVertexArray(sphereVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelSphere));
    glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(planeVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelPlane));
    glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
}


GLuint create_shader() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        layout(location = 2) in vec3 aNormal;

        out vec3 ourColor;
        out vec3 fragPosition;
        out vec3 Normal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            fragPosition = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            ourColor = aColor;
        }
    )";
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 ourColor;
        in vec3 fragPosition;
        in vec3 Normal;

        out vec4 FragColor;

        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 lightColor;
        uniform float lightSize;
        uniform vec3 sphereCenter;
        uniform float sphereRadius;
        uniform int shadowSamples;

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
            float d = b * b - 4 * a * c;
            return (d > 0);
        }

        vec3 randPointOnLight(vec3 lightPos, float lightSize) {
            float halfSize = lightSize / 2.0;
            float randomX = (rand() - 0.5) * lightSize;
            float randomY = (rand() - 0.5) * lightSize;
            return lightPos + vec3(randomX, randomY, 0.0);
        }

        float CalcSoftShadow(vec3 fragPosition, vec3 lightPos, float lightSize, int samples) {
            float shadow = 0.0;
            for (int i = 0; i < samples; i++) {
                vec3 randomPointOnLight = randPointOnLight(lightPos, lightSize);
                Ray ray;
                ray.origin = fragPosition;
                ray.direction = normalize(randomPointOnLight - fragPosition);

                if (RayIntersectsSphere(ray, sphereCenter, sphereRadius)) {
                    shadow += 0.5;
                }
            }
            return shadow / float(samples);
        }

        void main() {
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - fragPosition);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            float shadow = CalcSoftShadow(fragPosition, lightPos, lightSize, shadowSamples);
            vec3 result = diffuse * ourColor;
            
            result *= (1.0 - shadow);

            FragColor = vec4(result, 1.0);
        }
    )";
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_DEPTH_TEST);

    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST); 

    get_sphere(0.5f, 20, 20);
    sphere_init();
    plane_init();

    modelSphere = glm::mat4(1.0f);
    modelPlane = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));

    shaderProgram = create_shader();

    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
