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
    vec3 position; // 聚光的位置向量
    vec3 direction; // 聚光的方向向量
    float cutOff; // 切光角

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 实现衰减
    //float constant;
    //float linear;
    //float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec2 TexCoords;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    vec3 ambient = vec3(1.0);
    vec3 diffuse = vec3(1.0);
    vec3 specular = vec3(1.0);

    // 计算光源到片段与光线方向夹角 与 切光角比较 决定是否在聚光内部
    float theta = dot(lightDir, normalize(-light.direction));

    if(theta > light.cutOff)
    {
        // 执行光照计算
        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

        // vec3 lightDir = normalize(-light.direction);
        // 光照计算需求一个从片段至光源的方向
        // 但是我们输入的定向光为一个从光源出发的全局方向 因此需要取反改变方向
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  


        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);

    }
    else // 否则使用环境光，让场景在聚光外不至于完全黑暗
        //FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb), 1.0);
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
}
