#version 330 core
in vec3 FragPos;
// 添加所有经过变换到世界空间中的顶点属性位置的输入坐标
// 接下来在片段着色器中添加光照计算
// 首先计算光源与片段位置之间的方向向量
// 光的方向向量是光源位置向量与片段位置向量之间的向量差
// 确保所有相关向量最后都转换为单位向量 所以将法线和最终的方向向量进行标准化
in vec3 Normal;
in vec3 LightPos;
// 在片段着色器中定义相应的输入变量
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
// 使用摄像机对象的位置坐标作为观察者的世界空间坐标
// 将相应的摄像机坐标传给片段着色器

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    // 对norm与lightDir向量进行点乘 计算光源对当前片段实际的漫反射影响 结果值 是法向量与光照方向向量的点乘
    // 结果值再乘以光的颜色 得到漫反射分量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // 如果向量夹角大于90 点乘结果为负
    // 夹角大于90 应该即处于黑暗中的部分
    // 因此使用max函数返回两个参数间较大的参数 保证漫反射分量不会变为负数
    // 负数颜色的光照是没有定义的
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // vec3 result = ambient * objectColor;
    // 将环境光分量与漫反射分量相加 结果乘以物体颜色 获得片段最后的输出颜色
    float specularStrength = 0.5;
    // 定义一个镜面强度变量 给镜面高光一个中等亮度颜色
    // 如果设置为1.0f会得到一个非常亮的镜面光分量
    // 计算视线方向向量 和对应沿着法线轴的反射向量
    vec3 viewDir = normalize(- FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    // 对lightDir向量取反 lightDir是片段指向光源，我们需要光源指向片段的向量，用reflect函数沿片段面法向量对称得到反射光线向量
    //
    // 计算镜面分量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    // 计算视线方向与反射方向的点乘，确保其不为赋值 然后取其32次幂
    // 32 为高光的反光度 物体的反光度越高，，反射光的能力越强，散射得越少，高光点就越小。
    vec3 specular = specularStrength * spec * lightColor;
    // 将镜面反射分量加入环境光和漫反射分量中
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
// 这个片段着色器从uniform变量中接受物体的颜色和光源的颜色
// 将光源的颜色与物体反射的颜色相乘
