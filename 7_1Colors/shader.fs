#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0);
}
// 这个片段着色器从uniform变量中接受物体的颜色和光源的颜色
// 将光源的颜色与物体反射的颜色相乘
