#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
 uniform mat4 projection;
// GLSL也有mat4类型
// 将变换矩阵传入着色器

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    // 将其与位置向量相乘
    // 乘法要从右往左读
    ourColor = aColor;
    TexCoord = aTexCoord;
}
