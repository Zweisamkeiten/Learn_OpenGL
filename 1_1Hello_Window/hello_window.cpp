#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
void processInput(GLFWwindow *window);

int main()
{
    glfwInit(); // Init GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // we can configure GLFW using glfwWindowHint。
    // first argument of glfwWindowHint 
    // 我们可以从很多以GLFW_开头的枚举值中选择
    // 第二个参数接受一个整型 用以设置这个选项的值
    // 前两句将主版本号和次版本号都设置为3，明确opengl3.3版本
    // 第三句明确GLFW使用的为核心模式(Core-profile)
    // 使用核心模式意味着只能使用OpenGL的一个子集，不需要向后兼容特性
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // macos系统需要加以上代码才能使配置生效
    //

    //--------------------------------------------
    // 创建一个窗口对象

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    // glfwCreateWindow 需要窗口宽高作为前两个参数，第三个参数为窗口的标题
    // 最后两个参数暂时忽略
    // 该函数会返回一个GLFWwindow对象
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建完窗口就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //--------------------------------------------

    // GLAD是用来管理OpenGL函数指针的，调用任何OpenGL函数之前需要初始化GLAD
    //--------------------------------------------
    // 初始化GLAD

    // 给GLAD传入用来加载系统相关的OpenGL函数指针地址的函数glfwGetProcAddress,根据编译的系统定义了正确的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //--------------------------------------------
    // 定义视口
    // 开始渲染之前，需要告诉OpenGL渲染窗口的尺寸大小，即视口
    // 如此opengl才知道如何根据窗口大小来显示数据和坐标
    // 通过调用glViewport来设置窗口的 维度

    glViewport(0, 0, 800, 600);
    // 前两个参数控制窗口左下角的位置
    // 第三四个参数控制渲染窗口的宽度和高度
    // 实际上可以将视口的维度设置为比GLFW窗口维度小，
    // 这样所有的OpenGL渲染将会在一个更小的窗口中显示。
    // 因此我们可以将一些其他元素显示在OpenGL视口之外
    //
    // 当用户改变窗口大小的时候，视口也应该被调整。
    // 因此需要对窗口注册一个回调函数
    // 会在每次窗口大小被调用的时候被调用
    // 函数原型

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    // 声明之后需要使用GLFW对其进行注册
    // 告诉GLFW希望在每次调整窗口大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //--------------------------------------------

    //--------------------------------------------
    // 准备引擎
    // 添加一个while循环作为渲染循环render loop
    // 能够让GLFW在退出之前保持运行

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        // 每次渲染循环时调用processInput检测按键

        //--------------------------------------------
        // 将所有的渲染操作都放到循环渲染中
        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 每次新的渲染迭代开始时，我们总希望清屏，否则我们仍然能看到上一次迭代的渲染结果
        // 通过调用glClear函数来清空屏幕的颜色缓冲
        // 它接受一个缓冲位来指定要清空的缓存
        // 可能的缓冲位有GL_COLOR_BUFFER_BIT,GL_DEPTH_BUFFER_BIT,GL_STENCTL_BUFFER_BIT
        // 现在我只关心颜色值，所以只清空颜色缓冲
        // 调用glClearColor来清空屏幕所用的颜色
        // glClear清空颜色缓冲，之后颜色缓冲会被填充为glClearColor里所设置的颜色
        //--------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfwWindowShouldClose 在每次循环开始前检测GLFW是否被要求退出，如果为是，函数返回true然后退出
    // glfwSwapBuffers用来交换窗口的两个颜色缓冲，也叫做双缓冲。如果不使用双缓冲，就会出现闪屏现象。
    // 因为绘制一般不是一下子绘制完毕的，而是从左到右、从上到下地绘制。
    // 使用双缓冲避免闪屏
    // 前缓冲是最终的图像，程序会在后缓冲上绘制
    // 后缓冲绘制完毕后交换两个缓冲，解决闪屏问题
    // glfwPollEvents函数检测是否有发生例如键盘输入、鼠标移动,更新窗口状态，并调用对应的回调函数
    // 因为GLFW不会自动更新状态，必须调用glfwPollEvents才能更新
    // glfwPollEvents()用来检查是否有事件被触发，例如点击关闭按钮、点击鼠标、按下键盘，等等。如果有，GLFW将会对这些事件进行处理。更严谨地说（以键盘、鼠标键为例），GLFW自己会记录每个键、鼠标键的状态（按下/没有按下），但当某个按键松开或被按下时，GLFW不会自动更新状态，必须调用glfwPollEvents()才能更新。调用glfwPollEvents()时会检查状态是否有变化（如按下的是否松开，没有按下的是否被按下），如果有就会更新该状态。如果设置了回调函数（这个将在以后讲），还会调用相应的回调函数。如果不调用这个函数，不仅无法检测输入（后文会需要这样），我们在点击窗口右上角的X时，GLFW也不会知道需要关闭窗口。所以必须在每一轮游戏循环中调用这个函数。

    //--------------------------------------------


    //--------------------------------------------
    // 最后一件事正确释放和删除之前分配的所有资源
    // 调用glfwTerminate函数来完成
    // glfwTerminate将会释放所有的GLFW资源
    glfwTerminate();
    return 0;
}

// 窗口大小回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // 检测用户是否按下Escape键
    // 如果没有按下glfwGetKey会返回GLFW_RELEASE
    // 如果按下则返回GLFW_PRESS
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // 通过glfwSetWindowSholdClose将windowshouldclose的属性值设为true来关闭GLFW
    // 下一次while循环检测时就无法通过，程序关闭
}
