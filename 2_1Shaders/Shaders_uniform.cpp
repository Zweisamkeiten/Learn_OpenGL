#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
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
    // 我们在片段着色器中声明了一个uniform vec4的ourcolor
    // 并把片段着色器的输出颜色设置为uniform值的内容
    // 因为uniform是全局变量，可以在任何着色器中定义它们
    // 全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的
    // 而无需通过顶点着色器作为中介
    // 顶点着色器不需要这个uniform
    // 所以我们不用在顶点着色器中定义它
    // 如果我们声明了一个uniform却没在GLSL代码中使用
    // 编译器会静默移除这个变量
    // 导致最后编译出的版本中不包含他
    // 这会导致非常麻烦的错误
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n";
    // 现在uniform还是空的
    // 还没有添加任何数据
    // 首先要找到着色器中uniform属性的索引或者位置值
    // 当我们得到索引/位置值后
    // 就可以更新它的值
    // 这次我们不去给像素传递单独的一个颜色
    // 而是让他随时间改变颜色
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello_triangle_index", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILAtION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glad_glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAIED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = { // 索引从0开始！
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    unsigned int VBO, VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 激活着色器
        glUseProgram(shaderProgram);
        float timeValue = glfwGetTime();
        // 通过glfwwGetTime()获取运行的秒数
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // 使用sin函数让颜色在0.0到1.0之间改变
        // 最后将结果储存到greenValue中
        // 使用glGetUniformLocation查询uniform 中ourColr的位置值
        // 这个查询函数需要被提供着色器程序和uniform名字
        // 如果 glGetUniformLocation 返回-1表示没有找到这个位置值
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        // 通过 glUniform4f函数设置uniform值
        // 查询uniform地址不要求之前使用过着色器程序
        // 但是更新一个uniform之前 必须 先使用程序 通过调用 glUseProgram
        // 它是在当前激活的着色器程序中设置uniform
        // OpenGL 核心为C库不支持类型重载
        // 函数参数类型不同哦你时就要为其定义新的函数
        // glUniform有一个特定的后缀
        // 标识设定的uniform类型
        // f 需要一个float值
        // i int
        // ui unsigned int
        // 3f 3个float
        // fv 需要一个float向量/数组
        //
        //
        // 我们可以使用uniform来渲染了
        // 如果打算让颜色慢慢变化
        // 需要在游戏循环的每一次迭代中更新这个uniform
        // 因为迭代所以他会逐帧改变更新uniform,否则三角形就不会改变颜色
        // 通过计算greenValue来使每个渲染更新这个uniform

        // 绘制三角形
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 交换缓冲并查询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
