#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    //vec3 specular;
    sampler2D specular; // 采样镜面光贴图
    float shininess;
};

struct Light {
    vec3 position; // 点光源
    //vec3 direction; // 定向光

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 实现衰减
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec2 TexCoords;

void main()
{
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(-light.direction);
    // 光照计算需求一个从片段至光源的方向
    // 但是我们输入的定向光为一个从光源出发的全局方向 因此需要取反改变方向
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

    // 根据公式计算点光源衰减值
    // // 需要公式中距离光源的距离 计算向量的长度
    // 通过获取片段和光源之间的向量差 并获取结果向量的长度作为距离项
    // 可通过GLSL内建的length函数来完成
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    vec3 result = (ambient + diffuse + specular)  * attenuation;
    FragColor = vec4(result, 1.0);

}
