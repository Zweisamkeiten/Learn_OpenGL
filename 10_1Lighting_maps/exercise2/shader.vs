// 需要一个顶点着色器来绘制箱子
// 不需要纹理坐标
// 为每个顶点添加了一个法向量。 所以需要更新顶点着色器
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


// 需要片段的位置
// 需要在世界空间中进行所有的光照计算
// 因此需要一个在世界空间中顶点位置
// 可以通过把所有顶点位置属性乘以模型矩阵来将其变换到世界空间坐标
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
}
