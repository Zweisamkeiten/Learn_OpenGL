#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量\n"
    "void main()\n"
    "{\n"
    "   FragColor = outColor;\n"
    "}\n";
