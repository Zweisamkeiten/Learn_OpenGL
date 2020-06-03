#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;

int main()
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // 使用GLM内建的向量类定义一个叫做vec的向量

    glm::mat4 trans = glm::mat4(1.0f);
    // 定义一个mat4类型的trans
    // 默认为一个4 * 4的单位矩阵
    // 下一步是创建一个变换矩阵
    // 把单位矩阵和位移向量传递给 glm::translate函数来完成
    // 用给定的矩阵乘以位移矩阵就能获得最后需要的矩阵
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
}
