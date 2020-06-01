#define STB_IMAGE_IMPLEMENTATION
// 通过定义 STB_IMAGE_IMPLEMENTATION 预处理器会修改头文件
// 让其只包含相关的函数定义源码
// 等于是将这个头文件变为一个cpp文件
// 现只需在程序中包含 stb_image.h 并编译就可以了
#include "../stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../shader_s.h"

using namespace std;
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Textures", NULL, NULL);
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

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("shader.vs", "shader.fs");

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    // 添加了一个额外的顶点属性 所以需要告诉 OpenGL新的顶点格式

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };



    // 为了能够把纹理映射到三角形上
    // 我们需要指定三角形的每个顶点对应纹理的哪个部分
    // 这样每个顶点就会关联着一个纹理坐标
    // 用来表明该从纹理图像的哪个部分采样
    // 之后在图形的其他片段上进行片段插值
    //
    // 纹理坐标在x和y轴上
    // 范围为0到1之间
    // 使用纹理坐标获取纹理颜色叫做采样
    // 纹理坐标起始于(0,0) 图片的左下角
    // 终始于(1, 1) 图片的右上角
    //float texCoords[]{
    //    0.0f, 0.0f, // 左下角
    //    1.0f, 0.0f, // 右下角
    //    0.5f, 1.0f  // 上中
    //};

    // 纹理环绕方式
    // 纹理坐标的范围通常是从(0,0)到(1,1)
    // 如果将纹理坐标设置在范围之外
    // OpenGL默认的行为是重复这个纹理图像
    // GL_REPEAT 对纹理的默认行为 重复纹理图像
    // GL_MIRRORED_REPEAT 也是重复 但每次重复图片是镜像放置的
    // GL_CLAMP_TO_EDGE 纹理坐标会被约束为0到1之间 超出部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果
    // GL_CLAMP_TO_BORDER 超出的坐标为用户指定的边缘颜色
    //
    // 上述每个选项可使用 glTexParamete* 函数对单独的一个坐标轴设置
    // s t 如果使用3D纹理还有一个r
    // 与 x, y, z等价
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 第一个参数指定了纹理目标
    // 使用2D纹理所以纹理目标为 GL_TEXTURE_2D
    // 第二个参数指定设置的选项与应用的纹理轴
    // 配置WRAP选项，指定s 和 t轴
    // 最后一个参数传递一个Wrapping环绕方式
    // 这里选择GL_MIRRORED_REPEAT
    // 如选择 GL_CLAMP_TO_BORDER 还需指定一个边缘的颜色
    // 这需要使用glTexParameter函数的fv后缀格式
    // 用GL_TEXTURE_BORDER_COLOR作为它的选项
    // 并且传递一个float数组作为边缘的颜色值
    // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE__BORDER_COLOR, borderColor);
    //
    //
    // 纹理坐标不依赖于分辨率
    // 它可以为任意浮点值
    // 所以OpenGL需要知道怎样将纹理像素 Texture Pixel映射到纹理坐标
    // 当有一个很大的物体但是纹理的分辨率很低的时候
    // 这个过程就显得尤为重要
    //
    // OpenGL也有对于纹理过滤的选项
    // 纹理过滤有很多个选项 这里讨论两种
    // GL_NEAREST和GL_LINEAR
    //
    // 纹理坐标是给模型顶点设置的数组
    // OpenGL以这个顶点的纹理坐标数据去查找纹理图像上的像素
    // 然后进行采样提取纹理像素的颜色
    //
    // GL_NEAREST 邻近过滤 Nearest Neighbor Filtering
    // 默认纹理过滤方式
    // 当设置为GL_NEAREST时 OpenGL会选择中心点最接近纹理坐标的那个像素 选择它作为样本颜色
    //
    // GL_LINEAR 线性过滤 linear Filtering
    // 基于纹理坐标附近的纹理像素
    // 计算出一个插值
    // 近似出这些纹理像素之间的颜色
    // 一个纹理像素的中心位置距离纹理坐标越近
    // 这个纹理像素的颜色对最终的样本颜色的贡献越大
    //
    // 在对低像素的图像进行处理时
    // GL_NEAREST 会产生颗粒状的图案
    // 能够清晰看到组成纹理的像素
    // GL_LINEAR能够产生更平滑的图像
    // 很难看出单个纹理像素 因而可以产生更真实的输出
    //
    // 进行放大Magnify和缩小Minify操作的时候可以设置纹理过滤选项
    // 可以在纹理被缩小时使用邻近过滤，被放大时使用线性过滤
    // 需要使用glTexParameter* 函数来为放大和缩小指定过滤方式
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //
    //
    // 多级渐远纹理
    // 不同距离的物体如果拥有同样高的分辨率 会造成不真实和内存浪费的问题
    // 远处物体可能只产生很少的片段，OpenGL很难从高分辨率纹理中为这些片段获取正确的颜色值
    //
    // OpenGL使用多级渐远纹理来解决这个问题 Mipmap
    // 实际上是一系列的纹理图像
    // 后一个纹理图像是前一个的二分之一
    // 理念:距观察者的距离超过一定的阈值 OpenGL会使用不同的多级渐远纹理 即最适合物体距离的那个
    // 由于距离远 即使解析度不高也不会被用户注意
    // 同时也提高了了性能
    //
    // OpenGL 中 glGenerateMipmaps 函数在创建完一个纹理之后通过调用它就可以创建一系列多级渐远纹理
    //
    // 在渲染中切换多级渐远纹理级别是， OpenGL会在两个不同级别的多级渐远纹理层之间产生不真实的生硬边界
    // 切换多级纹理级别时也可以在两个不同多级渐远纹理级别之间使用NEAREST 和 LINEAR过滤
    // 为指定不同多级渐远纹理级别之间的过滤方式
    // 可使用以下来代替原有的过滤规则
    // GL_NEAREST_MIPMAP_NEAREST 使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
    // GL_LINEAR_MIPMAP_NEAREST 使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
    // GL_NEAREST_MIPMAP_LINEAR 在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
    // GL_LINEAR_MIPMAP_LINEAR 在两个邻近的多级渐远纹理之间使用
    // 使用 glTexParameteri 将过滤方式设置为上述一种
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 一个常见的错误是，将放大过滤的选项设置为多级渐远纹理过滤选项之一。这样没有任何效果，因为多级渐远纹理主要是使用在纹理被缩小的情况下的：纹理放大不会使用多级渐远纹理，为放大过滤设置多级渐远纹理的选项会产生一个GL_INVALID_ENUM错误代码。
    //
    //

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 添加了一个额外的顶点属性 所以需要告诉 OpenGL新的顶点格式
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // layout location 为2
    // 由两个数组成 所以为2
    // 浮点数
    // 不希望被标准化
    // 步长为8 = 3 + 3 + 2
    // 偏移量为 6 = 3 + 3
    // 接下来需要调整顶点着色器使其能够接受顶点坐标为一个顶点属性
    // 并将坐标传给片段着色器

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    //
    // ================= 生成纹理
    unsigned int texture;
    glGenTextures(1, &texture);
    // glGenTextures 函数首先需要输入生成的纹理数量
    // 然后把它们储存在第二个参数的 unsigned int 数组中
    // 然后需要绑定它
    // 让之后任何的纹理指令都可以配置当前绑定的纹理
    //

    glBindTexture(GL_TEXTURE_2D, texture);
    // 这样纹理就已经绑定了，可以使用前面载入的图片数据
    //
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // stb_image.h
        // 使用stbi_load函数加载图片
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
        // 这个函数首先接受一个图像文件的位置作为输入
        // 接下来需要三个int作为后三个参数
        // stb_image.h 将会用图像的宽度高度，和颜色通道的个数来填充这三个变量，之后在生成纹理时会用到图像的宽度和高度
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // 第一个参数指定纹理目标 target 设置为 GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理 (任何绑定到 GL_TEXTURE_1D和 GL_TEXTURE_3D的纹理不会受到影响)
    // 第二个参数为纹理制定多级渐远纹理的级别 如果希望单独手动设置多个多级渐远纹理的级别的话，填写0,即基本级别
    // 第三个参数告诉OpenGL希望将纹理储存为何种格式 这里的图像只有RGB所以将 纹理储存为RGB值
    // 第四和第五个参数设置最终的纹理的宽度和高度 之前加载图像时已经储存了它们
    // 第六个参数因历史遗留问题总是被设置为0
    // 第七第八个参数定义了源图的格式和数据类型 使用RGB值加载这个图像，并且将他们储存为char(byte)数组
    // 最后一个参数是真正的图像数据
        glGenerateMipmap(GL_TEXTURE_2D);
    // 当调用 glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像
    // 然而目前只有基本级别的(Base-level)的纹理图像被加载了
    // 如果要使用多级渐远纹理
    // 必须手动设置不同的图像(不断递增第二个参数)
    // 或者直接在生成纹理之后调用 glGenerateMipmap
    // 这会为当前绑定的纹理自动生成所有需要的多级渐远纹理
    //
    // 生成了纹理和相应的多级渐远纹理之后 释放图像的内存
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    while(!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        ourShader.use();
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
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

