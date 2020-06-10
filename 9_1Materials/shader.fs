#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
// 在片段着色器中创建一个结构体来储存物体的材质属性
// 其实可以将它们储存为独立的uniform值 但是作为一个结构体来储存更灵活高效。
// 首先定义结构体的布局 然后使用刚创建的结构体为类型 声明一个uniform变量
// 这个结构体用来代替先前简单的只设置了一个objectColor
struct Material {
    vec3  ambient; // 环境光照下物体反射的颜色
    vec3  diffuse; // 漫反射光照下物体反射的颜色
    vec3  specular; // 镜面光照下物体反射的颜色
    float shininess; // 影响镜面高光的散射/半径
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 通过新增一个光照结构体 来为不同的光照设置不同的强度

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

// 修改光照的计算来顺应新的材质属性
void main()
{
    // 环境光
    vec3 ambient = light.ambient * material.ambient;
    // 将光源的环境光强度设置小一点 限制环境光颜色
    // 这个物体太亮了，过亮的原因是环境光、漫反射和镜面光这三个颜色对任何一个光源都会全力反射。
    // 但是光源实际上对环境光、漫反射和镜面光分量也具有不同的强度
    // 这里通过类似的系统，这次为每个光照分量都指定一个强度向量
    // 因此上一节通过一个强度值 来改变环境光和镜面光强度的方式解决反射强度的问题
    // 假设lightColor 为vec3(1.0)

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
