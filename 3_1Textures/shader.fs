#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = texture(ourTexture, TexCoord);
    // 使用GLSL内建的texture函数来采样纹理颜色
    // 第一个参数为纹理采样器
    // 第二个参数为相应的纹理坐标
    // texture函数会使用之前设置的纹理参数相对应的颜色值进行采样
    // 这个片段着色器的输出就是纹理的插值纹理坐标上的过滤后的颜色
}
