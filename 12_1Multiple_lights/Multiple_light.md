# 多光源

结合冯式着色 材质 光照贴图 以及不同种类的投光物来创建一个包含六个光源的场景。

模拟一个类似太阳的定向光 四个分散在场景中的点光源 以及一个手电筒

为了在场景中使用多个光源 因此希望将光照计算封装到GLSL函数中

这样做的原因是，每一种光源都需要一种不同的计算方法

而一旦想对多个光源进行光照计算时， 源代码会变得很复杂 如果只在main函数中进行所有的计算 代码会难以理解

GLSL中的函数与C函数类似 有一个函数名 一个返回值类型 如果函数不是在main函数之前声明的，还必须在代码顶部声明一个原型。

为每个光照类型都创建一个不同的函数。
定向光、点光源和聚光