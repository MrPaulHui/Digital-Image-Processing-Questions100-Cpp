# 数字图像处理note

##  C++ opencv的基础的基础操作

- 关于cv::Vec3f和.at的说明 https://www.cnblogs.com/TearCheer/p/12628798.html
- cv::Mat的赋值和切片方法 https://blog.csdn.net/o3279/article/details/81304621
- 创建Mat方法 https://blog.csdn.net/u012058778/article/details/90764430

## C++问题

- 返回值是二维数组的函数该怎么定义？
- 类似python的函数形参列表初始化，只对其中几个赋值怎么办？有关的：https://www.cnblogs.com/jerrybai/p/10693594.html；

## 几个去噪滤波器的特点，适用的情况

- https://blog.csdn.net/qinghuaci666/article/details/81737624
- 中值滤波适用于去除椒盐噪声，因为椒盐噪声是异常点，取中值不会取到异常点，即异常点对滤波后的图像没有影响了。而且相比均值滤波，图像不是那么模糊。
- 均值滤波对椒盐噪声没有作用，因为取均值，椒盐噪声异常点对滤波后的图像仍然有很大的影响。

## 其他的滤波器

- motion filter，对图像进行运动模糊处理，kernel是对对角线元素取平均。t12。

## 边缘检测滤波器

- max-min滤波器。t13。原理：图像的细节属于低频信息，图像的边缘属于高频信息。我们使用一定大小的 Max-Min 滤波器作用于图像，当滤波器作用于图像细节时，输出结果往往趋向于0（黑色）；而滤波器作用于图像边缘时，Max-Min 输出结果往往趋向于255（白色）。所以 最大-最小滤波器 能有效地用于检测图像的边缘和轮廓。