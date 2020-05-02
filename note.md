# 数字图像处理note

##  C++ opencv的基础的基础操作

- 关于cv::Vec3f和.at的说明 https://www.cnblogs.com/TearCheer/p/12628798.html
- cv::Mat的赋值和切片方法 https://blog.csdn.net/o3279/article/details/81304621
- 创建Mat方法 https://blog.csdn.net/u012058778/article/details/90764430

## C++问题

- 返回值是二维数组的函数该怎么定义？
- 类似python的函数形参列表初始化，只对其中几个赋值怎么办？有关的：https://www.cnblogs.com/jerrybai/p/10693594.html；
- 函数返回数组的方法：https://www.cnblogs.com/Wade-James/p/7965775.html

## 几个去噪滤波器的特点，适用的情况

- https://blog.csdn.net/qinghuaci666/article/details/81737624
- 中值滤波适用于去除椒盐噪声，因为椒盐噪声是异常点，取中值不会取到异常点，即异常点对滤波后的图像没有影响了。而且相比均值滤波，图像不是那么模糊。
- 均值滤波对椒盐噪声没有作用，因为取均值，椒盐噪声异常点对滤波后的图像仍然有很大的影响。
- 高斯滤波器有去噪和模糊图像两个作用。

## 其他的滤波器

- motion filter，对图像进行运动模糊处理，kernel是对对角线元素取平均。t12。

## 边缘检测滤波器

一个点，边缘检测的滤波器sum为0，去噪滤波器sum为1（做了归一化）。

另一个点，滤波其实是对每个像素点进行操作，输出操作（融合周围的信息以达到某种运算目的（梯度，二阶微分...））后的像素点。

- max-min滤波器。t13。原理：图像的细节属于低频信息，图像的边缘属于高频信息。我们使用一定大小的 Max-Min 滤波器作用于图像，当滤波器作用于图像细节时，输出结果往往趋向于0（黑色）；而滤波器作用于图像边缘时，Max-Min 输出结果往往趋向于255（白色）。所以 最大-最小滤波器 能有效地用于检测图像的边缘和轮廓。

- Emboss滤波器。常用于检测图像的边缘和轮廓，能够有效地增强图像的高频信息（边缘和轮廓），并保留图像的低频信息（图像内容）。t18。**类似有点浮雕效果？**
  $$
  K=
  \left[
  \begin{matrix}
  -2&-1&0\\
  -1&1&1\\
  0&1&2
  \end{matrix}
  \right]
  $$

### 基于梯度的

https://blog.csdn.net/xiahn1a/article/details/42141429

#### 一阶

横向和纵向的组合方法：
$$
G = \sqrt{G_x^2+G_y^2} \approx |G_x| +|G_y|
$$


- 差分滤波器。使用的直接相邻的差值为梯度。t14。

  计算梯度的方法：
  $$
  G_y = f(x,y)-f(x,y-1)\\
  G_x = f(x,y)-f(x-1,y)
  $$
  对应的模板：

  纵向：
  $$
  K=\left[
  \begin{matrix}
  0&-1&0\\
  0&1&0\\
  0&0&0
  \end{matrix}
  \right]
  $$
  横向：
  $$
  K=\left[
  \begin{matrix}
  0&0&0\\
  -1&1&0\\
  0&0&0
  \end{matrix}
  \right]
  $$

- sobel滤波器。使用左右两个像素点差值作为梯度。t15。

  Sobel算子根据像素点上下、左右邻点灰度加权差，在边缘处达到极值这一现象检测边缘。对噪声具有平滑作用，提供较为精确的边缘方向信息，边缘定位精度不够高。当对精度要求不是很高时，是一种较为常用的边缘检测方法。

  计算梯度的方法：
  $$
  G_x = \frac{f(x+1,y)-f(x-1,y)}{2}\\
  G_x = \frac{f(x+1,y-1)-f(x-1,y-1)}{2}\\
  G_x = \frac{f(x+1,y+1)-f(x-1,y+1)}{2}
  $$
  将上面三个式子相加，且对第一个加2倍的权重（这个最重要，剩下两个是考虑到更远一点的周围）。
  $$
  G_x = \frac{1}{8}(f(x+1,y-1)+2f(x+1,y)+f(x+1,y+1)-f(x-1,y-1)-2f(x-1,y)-f(x-1,y+1))
  $$
  y方向的推导同理。

  对应的模板：（这里和他给的教程是不一样的）

  纵向：
  $$
  K=\left[
  \begin{matrix}
  -1&-2&-1\\
  0&0&0\\
  1&2&1
  \end{matrix}
  \right]
  $$
  横向：
  $$
  K=\left[
  \begin{matrix}
  -1&0&1\\
  -2&0&2\\
  -1&0&1
  \end{matrix}
  \right]
  $$

- Prewitt滤波器。原理和sobel类似，只是对第一个Gx式子没有进行2倍加权。

  模板：（这里给的教程横向的有问题，这里写的是正确的）

  纵向：
  $$
  K=\left[
  \begin{matrix}
  -1&-1&-1\\
  0&0&0\\
  1&1&1
  \end{matrix}
  \right]
  $$
  横向：
  $$
  K=\left[
  \begin{matrix}
  -1&0&1\\
  -1&0&1\\
  -1&0&1
  \end{matrix}
  \right]
  $$

#### 二阶

- Laplacian滤波器。最基本的二阶。原理其实是求每个像素点的散度。t17。

  推导过程：
  $$
  \nabla^2f = \frac{\partial^2f}{\partial x^2} + \frac{\partial^2f}{\partial y^2}\\
  = G_{xx}+G_{yy}
  $$
  即求每个像素点的$$\nabla^2f$$值。
  $$
  G_x = f(x+1,y)-f(x,y)\\
  G_{xx} = G_x(x,y)-G_x(x-1,y)\\
  = [f(x+1,y)-f(x,y)]-[f(x,y)-f(x-1,y)]\\
  =f(x+1,y) + f(x-1,y) - 2f(x,y)
  $$
  同理，
  $$
  G_{yy} = f(x,y+1)+f(x,y-1)-2f(x,y)
  $$
  所以，
  $$
  \nabla^2f = f(x+1,y)+f(x-1,y)+f(x,y+1)+f(x,y-1)-4f(x,y)
  $$
  对应的模板：
  $$
  K=
  \left[
  \begin{matrix}
  0&1&0\\
  1&-4&1\\
  0&1&0
  \end{matrix}
  \right]
  $$

- LoG滤波器。先进行高斯滤波降噪，再用Laplacian滤波器进行边缘检测。目的是防止二阶微分使噪声异常点变得更明显。https://blog.csdn.net/u014485485/article/details/78364573

  推导过程：

  先用高斯滤波器作用，再用Laplacian滤波器作用 等价于 先对高斯滤波器求二阶微分，再用这个结果作用于图像。即
  $$
  高斯滤波：Guassian_\sigma = \frac{1}{2\pi\sigma}exp(-\frac{x^2+y^2}{2\sigma^2})\\
  LoG(f(x,y)) = \nabla^2(Guassian_\sigma*f) = (\nabla^2Guassian_\sigma)*f\\
  所以 LoG算子即为\nabla^2Guassian_\sigma\\
  对第一个式子求散度，得到\\
  \nabla^2Guassian_\sigma=\frac{x^2+y^2-\sigma^2}{2\pi\sigma^6}exp(-\frac{x^2+y^2}{2\sigma^2})
  $$
  

## 直方图系列

- C++ opencv绘制直方图库函数方法：https://blog.csdn.net/qq_24282081/article/details/73135799