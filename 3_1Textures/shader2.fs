#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// 需要编辑片段着色器来接受另一个采样器

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    // 最终的输出颜色现在是两个纹理的结合
    // GLSL内建的mix函数需要接受两个值
    // 并对它们根据第三个参数进行线性插值
    // 如果第三个值为0.0 他会返回地一个输入
    // 如果是1.0会返回第二个输入值
    // 0.2会返回80% 的第一个输入颜色 和20%的第二个颜色
    // 即返回两个纹理的混合色
}
