#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // program ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        // 1. 从文件路径获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证ifstream对象可以正常抛出异常
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // 读取文件
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // ===============================
        // =========2. 编译着色器
        // ===============================
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // 顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // 打印编译错误
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // 片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // 着色器程序
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // 打印连接错误
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // 删除着色器，因为他们已经被链接
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // 激活程序
    void use()
    {
        glUseProgram(ID);
    }
    // uniform 工具函数
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

};
#endif
// 在头文件顶部使用了几个预处理指令
// 告知编译器在没有被包含过SHADER头文件时才包含和编译这个头文件
// 着色器类储存了着色器程序的ID
// 它的构造器需要顶点和片段着色器的源代码的文件路径
// 这样可以把源码的文本文件储存在硬盘上了
// 加入工具函数：
// use用于激活着色器程序
// 所有的set函数能够查询一个uniform的位置并且设置它的值
//
// 现在已经写完了一个完整的着色器类
// 使用这个着色器类时 只要创建一个着色器对象
// 在源代码中 Shader XXXX("path/to/shaders/shader.vs", "path/to/shaders/shader.fs");
// 将顶点着色器和片段着色器的源代码储存为shader.vs 和shader.fs
