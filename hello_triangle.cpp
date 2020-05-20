#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
void processInput(GLFWwindow *window);

    // 创建一个顶点着色器源码
    // 使用in关键字在顶点着色器中声明所有的输入顶点属性
    // 现在我们只关心位置数据，所以只需要一个顶点属性
    // GLSL有一个向量数据类型 包含1-4个float分量
    // 包含的数量可以从它的后缀数字看出来
    // 由于每个定点都有一个3D坐标
    // 我们就创建一个vec3输入变量aPos
    // 同样通过layout(location = 0) 设定了输入变量的位置值
    // 为了设置顶点着色器的输出，需要将位置数据赋值给预定义的gl_Position变量
    // 它在幕后是vec4类型的
    // 在main函数的最后，gl_Position的值将会成为该顶点着色器的输出
    // 我们的输入是三分量的向量，所以需要将它转换为四分量
    // 可以将vec3的数据作为vec4的构造器的参数，同时将w分量设置为1.0f
    // 一个最简单的顶点着色器就完成了，因为没有对输入数据进行处理就可以传到着色器的输出
    // 真实应用场景里的输入数据通常不是标准化设备坐标
    // 因此必须首先将它转换至OpenGL的可视区域内
    // 向量
    // GLSL中一个向量最多有四个分量
    // 每个分量代表空间中的一个坐标
    // 可以通过vec.x, vec.y, vec.z, vec.w来获取
    // vec.w不是用作表达空间中的位置的
    // 用在所谓透视除法上
    //
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}\0";
    // 片段着色器
    // 渲染三角形，片段着色器是我们需要使用的最后一个着色器
    // 片段着色器的用处就是计算像素最后的颜色输出
    // 在计算机图形中颜色被表示为有4个元素的数组:红色、绿色、蓝色和alpha透明度分量 缩写为RGBA
    // 当在OpenGL或GLSL中定义一个颜色的时候
    // 我们把颜色的每个强度分量设置在0.0到1.0之间
    // 片段着色器只需要一个输出变量
    // 这个变量是一个4分量向量
    // 表示的是最终的输出颜色
    // 我们需要自己将其计算出来
    // 可以使用
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
    // 两个着色器现在都已经

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello_triangle", NULL, NULL);
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
    //--------------------------------------------------
    // 编译着色器
    // 在程序的开头已经编写了一个顶点着色器源码
    // 为了能够让OpenGL使用它，必须在运行时动态编译它的源码

    // 首先创建一个着色器对象，需要注意是要用ID来引用的
    // 然后用glCreateShader创建这个着色器对象
    // 把需要创建的着色器类型以参数的形式提供给glCreateShader,由于正在创建一个顶点着色器，传递的参数为GL_VERTEX_SHADER
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //下一步把着色器源码附加到着色器对象上，并编译它
    // glShaderSource函数把要编译的着色器对象作为第一个参数
    // 第二个参数指定了传递的源码字符串数量，此处只有一个
    // 第三个参数是定点着色器真正的源码
    // 第四个参数我们先设置为NULL
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 我们需要检测调用glCompileShader后编译是否成功
    // 如果未成功需要返回报错
    // 可以通过以下代码来实现
    // 首先定义一个整型变量来表示成功与否的状态码
    // 并定义一个储存错误信息的容器
    // 用 glGetShaderiv来检查是否编译成功
    // 如果编译失败，可以用 glGetShaderInfoLog来获取错误信息并打印它
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //--------------------------------------------------
    // 编译片段着色器 使用GL_FRAGMENT_SHADER常量作为着色器类型
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glad_glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //--------------------------------------------------
    // 着色器程序
    // 着色器程序对象是多个着色器合并之后并最终链接完成的版本
    // 如果要使用刚才编译的着色器，我们必须把它们链接为一个着色器程序对象
    // 然后在渲染对象的时候激活这个着色器程序
    // 已激活的着色器程序的着色器将在我们发送渲染调用的时候被使用
    // 当链接着色器至一个程序的时候
    // 他会把每个着色器的输出链接到下个着色器的输入
    // 当输入和输出不匹配的时候
    // 我们会得到一个连接错误
    //
    // 创建程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用
    // 现在我们需要把之前编译的着色器附加到程序对象上
    // 然后使用glLinkProgram链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 代码应该很清楚
    // 我们将两个着色器都附加到了程序上
    // 然后使用 glLinkProgram 链接
    //
    // 就像着色器的编译一样，也需要检测链接着色器程序是否失败
    // 并获取相应日志
    // 但所需要的函数与上面不同
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // 最终得到的结果为一个程序对象，可以调用 glUseProgram函数
    // 用刚刚创建的程序对象作为它的参数，用以激活这个程序对象
    //glUseProgram(shaderProgram);
    // glUseProgram 函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象
    // 所以应该添加进渲染循环中
    // 在把着色器对象链接到程序对象之后，需要删除着色器对象
    // 因为已经不再需要
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 现在已经将输入顶点数据发送给了GPU
    // 并指示了GPU如何在顶点和片段着色器中处理它
    // 最后一步需要让OpenGL知道如何解释内存中的顶点数据
    // 以及它该如何将顶点数据链接到顶点着色器的属性上
    // 这需要我们告诉它

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------
    //
    //--------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    // 渲染一个三角形，制定三个顶点
    // 以标准化设备坐标的形式，OpenGL的可见区域定义数组
    // opengl工作于3D空间，渲染2D三角形时将顶点的z坐标设置为0.0
    // 每一点深度相同从而看上去是2D的
    // 深度可以理解为z坐标，代表了一个像素在空间中和我们的距离
    // 如果离我们远就可能被别的像素遮挡
    // opengl就会丢弃这些像素，以节省资源
    //
    //
    //--------------------------------------------------
    // 一旦定点坐标在顶点着色器中被处理过，
    // 他们就成为了标准化设备坐标
    // 标准化设备坐标是一个x,y,z值在-1.0到1.0的一小段空间
    // 任何落在范围外的坐标都会被丢弃或者剪裁而不显示在屏幕上
    // 所有变换过的坐标都在这个坐标空间中
    // 标准化设备坐标接着变换为屏幕空间坐标
    // 这是通过glViewport函数提供的数据，进行视口变化完成的？？？？？
    // 所得到的屏幕空间坐标又会被变换为片段输入到片段着色器中
    // 顶点着色器处理阶段
    // 在GPU上创建内存用于储存顶点数据
    // 配置OpenGL如何解释这些内存
    // 并指定其如何发送给显卡
    //
    // 我们通过顶点缓冲对象VBO管理这些内存
    // 顶点缓冲对象会在GPU内存即显存中储存大量顶点
    // 使用缓冲对象的好处为可以一次性发送大批数据到显卡上
    // 而不是一个顶点发送一次
    // 从CPU发送数据到显卡相对较慢
    // 所以只要可能就都要尝试一次性发送尽可能多的数据
    // 当数据发送至显卡内存中，顶点着色器几乎能立即访问定点 这是个非常快的过程
            // 链接顶点属性
            // 顶点着色器允许我们指定任何以顶点属性为形式的输入
            // 这使得具有很强灵活性的同时
            // 它还意味着我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性
            // 因此，我们必须在渲染之前指定OpenGL该如何解释顶点数据
            // 我们的一个位置的顶点数据被解析为
            // 一个位置数据被储存为32位(4字节)浮点值
            // 每个位置包含三个这样的值
            // 三个值之间没有空隙，在数组中紧密排列
            // 数据中的第一个值在缓冲开始的位置
            // 有了这些信息之后，可以使用 glVertexAttribPointer
            // 函数来告诉OpenGL该如何解析顶点数据(逐个应用到每个顶点属性上):
            // bookmark "a"

    // 每个顶点属性从一个VBO管理的内存中获得它的数据
    // 而具体是从哪个VBO(程序中可以有多个VBO)获取
    // 则是通过调用 glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的
    // 由于在调用 glVertexAttribPointer之前绑定的是先前定义的VBO对象
    // 顶点属性0现在会链接到它的定点数据
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    // glBindVertexArray 绑定VAO来使用它
    glBindVertexArray(VAO);
    // VAO顶点数组对象
    // 可以将为每个物体所配置的所有顶点属性状态配置储存在一个对象之中，并可以通过绑定这个对象来恢复状态
    // 这样的好处是，当配置顶点属性指针时，我们只需要将那些调用执行一次
    // 之后在绘制物体的时候绑定相应的VAO即可
    // 这使得在不同的顶点数据和属性配置之间切换变得非常简单
    // 只要绑定不同的VAO就行
    // !!!!!! OpenGL的核心模式要求我们使用VAO，所以它知道该如何处理我们的顶点输入
    // 如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西
    // 一个顶点数组对象会储存以下内容
    // glENableVertexAttribArray glDisableVertexAttribArray调用
    // 通过 glVertexAttribPointer设置的顶点属性配置
    // 通过 glVertexAttribPointer调用和顶点属性关联的顶点缓冲对象
    glGenBuffers(1, &VBO);
    // 使用glgenbuffer函数和缓冲ID生成一个顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindBuffer将新创建的顶点缓冲对象绑定到GL_ARRAY_BUFFER目标上
    // 从此起，对GL_ARRAY_BUFFER目标上的缓冲调用
    // 都会用来配置当前绑定的缓冲VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 可以调用glBufferData把定义的顶点数据复制到缓冲的内存中
    // glBufferData是一个专门用来把用户定义的数据复制到当前绑定的缓冲函数
    // 第一个参数为目标缓冲的类型 因为当前顶点缓冲当前绑定在GL_ARRAY_BUFFER目标
    // 第二个参数指定传输数据的大小，以字节为单位 用sizeof计算顶点数据大小
    // 第三个参数是希望发送的实际数据
    // 第四个参数指定了希望显卡如何管理给定的数据
    // 有三种形式
    // 1. GL_STATIC_DRAW:数据不会或几乎不会被改变
    // 2. GL_DYNAMIC_DRAW:数据会被改变很多
    // 3. GL_STREAM_DRAW:数据每次绘制时都会改变
    // 三角形的位置数据不会改变，每次渲染调用时都保持原样
    // 因此使用类型设为GL_STATIC_DRAW
    // 后两者能够确保显卡把数据放在能够高速写入的内存部分
    // 现已经将顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理
    //
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 第一个值为指定我们要配置的顶点属性 , layout(loacation = 0)
    // 定义了positiond顶点属性位置值。
    // 它可以把顶点属性的位置值设为0
    // 因为我们希望把数据传递到这一个顶点属性中，所以我们传入0
    // 第二个参数指定顶点属性的大小。顶点属性为一个vec3
    // 它由三个值组成，所以大小为3
    // 第三个参数指定数据的类型，GL_FLOAT（GLSL中vec*都是由浮点数值组成)
    // 第四个参数定义我们是否希望数据被标准化
    // 如设为GL_TRUE，所有数据都会被映射到0到-1之间，我们设置为GL_FALSE
    // 第五个参数为步长
    // 它告诉我们在连续的顶点属性组之间的间隔
    // 由于下个组数据位置在3个float之后
    // 我们将步长设为3 * sizeof(float)
    // 最后一个参数void*，我们在进行强制类型转换
    // 它表示位置数据在缓冲中起始位置的偏移量
    // 由于位置数据在数组的开头，所以这里为0
    glEnableVertexAttribArray(0);
    // 用顶点属性位置值作为参数，启用顶点属性
    // 顶点属性默认是禁用的

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // 当我们将顶点缓冲对象绑定到顶点数组对象之后，就可以解绑VBO了
    // 当我们生成和配置完所有的VAO之后，然后储存他们以供后面使用
    // 当我们打算绘制物体的时候就拿出相应的VAO
    // 绑定他，绘制完物体之后再解绑VAO
    //--------------------------------------------------
    //
    // 创建一个顶点和片段着色器来处理数据
    //-------------------------------------------------- // 顶点着色器
    // 是几个可编程着色器之一
    // 如需渲染,OpenGL需要至少设置一个顶点和一个片段着色器
    //--------------------------------------------------
    // 配置着色器来绘制三角形
    // 使用着色器语言GLSL(OpenGL Shading Language)编写顶点着色器，并进行编译
    //
    // 编写顶点着色器

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元类型
        // 由于我们希望绘制三角形，所以传入 GL_TRIANGLES
        // 第二个参数指定了顶点数组的起始索引，所以这里为0
        // 最后一个参数指定我们打算绘制多少个顶点

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
