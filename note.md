# 数字图像处理note

##  C++ opencv的基础的基础操作

- 关于cv::Vec3f和.at的说明 https://www.cnblogs.com/TearCheer/p/12628798.html
- cv::Mat的赋值和切片方法 https://blog.csdn.net/o3279/article/details/81304621
- 创建Mat方法 https://blog.csdn.net/u012058778/article/details/90764430
- 矩阵运算https://blog.csdn.net/iracer/article/details/51296631

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

- 直方图归一化

  图像的直方图可能只集中与某一块，若集中于0附近会导致图像整体偏暗，集中于255则偏亮。解决方法就是进行归一化，对图像像素值取值范围进行转换映射，由[c,d]转换为[a,b]，其中c为当前像素值最小值，d为最大值。也可以说是扩大直方图的**动态范围**。使图像更清楚。也叫做**灰度变换**。（这里可以参考官方教程）。

  本质就是对像素值范围进行线性映射，使像素值范围扩大，不集中于某一块。

  转换公式：（这里官方教程给的判断条件是错误的，这里是正确的）
  $$
  x_{out}=\begin{cases}a& (\text{if}\quad x_{in}<a)\\\frac{b-a}{d-c}\ (x_{in}-c)+a&(\text{else if}\quad a\leq x_{in}<b)\\b&(\text{else})\end{cases}
  $$

- 直方图均衡化

  参考：https://blog.csdn.net/schwein_van/article/details/84336633

  也是使直方图平坦化，提高图像细节信息和**对比度**。不同于归一化需要计算原始图像数据，均衡化直接对直方图进行操作即可。

  直方图均衡化公式推导：https://blog.csdn.net/schwein_van/article/details/84336633

  涉及到概率论求概率密度函数，主要证明均衡化变换后呈均匀分布。

- 直方图匹配（规定化）

  根据给定的目标图像的直方图，让输入图像的直方图尽可能去接近目标图像的直方图。t23_more。

  参考：https://blog.csdn.net/mmmmmk_/article/details/82927411

  具体做法：

  1. 输入图像和目标图像各自进行直方图均衡化，得到均衡化的直方图
  2. 对两个均衡化直方图得到累积直方图
  3. 计算输入图像累积直方图每个灰度级对应的差距最小的目标图像累积直方图灰度级，进行对应的变换过去

  一个重要的细节：变换不能开始就对应重复的值，除非到最后，具体参照t23_more的实现。

  - [ ] 直方图匹配实现上还是有问题的，需要再解决。

## 暂时不知道怎么归类的

### 伽马校正

https://blog.csdn.net/wukery/article/details/77567933

主要是解决传输过程中产生的非线性问题。参考官方教程。（这里应该涉及到了gamma变换，可以结合hsv一起整理一下）

## 插值

这里官方教程讲的不错，可以直接参考。

### 最近邻插值

t25。

### 双线性插值

t26。

### 双三次插值

- 一个注意点：因为用到了邻域16个像素点，所以需要判断用到的像素点是不是在图像范围内。（最近邻和双线性没有这个问题）

官方教程这里有个错误，距离计算公式正确的：
$$
\begin{align*}
d_{x_1} = |\frac{x'}{a} - (x-1)|\quad 
d_{x_2} = |\frac{x'}{a}- x| \quad 
d_{x_3} = |\frac{x'}{a}- (x+1)|\quad 
d_{x_4} = |\frac{x'}{a} - (x+2)|\\
d_{y_1} = |\frac{y'}{a} - (y-1)|\quad 
d_{y_2} = |\frac{y'}{a} - y| \quad 
d_{y_3} = |\frac{y'}{a} - (y+1)| \quad 
d_{y_4} = |\frac{y'}{a} - (y+2)|
\end{align*}
$$

## 注意点

- 图像处理操作完记得是否需要进行clip。

## 仿射变换

仿射变换最基本的就是$y=ax+b$，对于二维的就是
$$
x' = ax+by+t_x\\
y' = cx+dy+t_y
$$
参考对二维向量做这种形式的变换，向量会发生平移，缩放，旋转等变换，同理对二维图像也是一样。

写成矩阵形式：
$$
\left[\begin{matrix}x'\\y'\\1\end{matrix}\right]=
\left[
\begin{matrix}
a&b&tx\\
c&d&ty\\
0&0&1
\end{matrix}
\right]
\left[\begin{matrix}x\\y\\1\end{matrix}\right]
$$
其中，矩阵
$$
A=\left[
\begin{matrix}
a&b&tx\\
c&d&ty\\
0&0&1
\end{matrix}
\right]
$$
A为仿射变换矩阵，其中参数a,b,c,d为控制缩放，旋转，倾斜变换的参数，tx，ty为控制平移的参数。

A的逆矩阵为
$$
A^{-1}=\left[
\begin{matrix}
\frac{d}{ad-bc}&\frac{-b}{ad-bc}&\frac{bt_y-dt_x}{ad-bc}\\
\frac{-c}{ad-bc}&\frac{a}{ad-bc}&\frac{ct_x-at_y}{ad-bc}\\
0&0&1
\end{matrix}
\right]
=\frac{1}{ad-bc}\left[
\begin{matrix}
d&-b&bt_y-dt_x\\
-c&a&ct_x-at_y\\
0&0&ad-bc
\end{matrix}
\right]
$$
实现中采用逆矩阵，即遍历要输出的图像像素，通过逆矩阵反变换对应到原图的像素点（对应到小数值的进行插值）。如果直接遍历输入的原图，正变换到输出图，输出图有元素会对应不到。

要输出图像的尺寸也需要注意，对于缩放问题，根据缩放幅度确定变换后的图像尺寸，对于旋转问题，看下面的情况确定。

### 平移

最简单的一种，a,b,c,d构成单位矩阵，用tx，ty控制两个方向的平移幅度即可。

对应的矩阵：
$$
\left[
\begin{matrix}
1&0&tx\\
0&1&ty\\
0&0&1
\end{matrix}
\right]
$$

### 缩放

根据最基本的二维仿射变换式子可知，a的值控制x方向的缩放幅度，d的值控制y方向的缩放幅度
$$
\left[
\begin{matrix}
a&0&tx\\
0&d&ty\\
0&0&1
\end{matrix}
\right]
$$
不需要平移把tx和ty置0即可。

缩放分为uniform scaling和non-uniform scaling

前者两个方向缩放比例一致，即a=d，后者则随意。

若缩放比例为负，则是反射的效果（参考下面旋转问题中的坐标系转换）。

### 旋转

很有东西的，花了一天时间推导的哦。。。

首先二维向量的旋转矩阵为
$$
\left[
\begin{matrix}
cos(\alpha)&-sin(\alpha)\\
sin(\alpha)&cos(\alpha)
\end{matrix}
\right]
$$
同样也适用于二维图像，不过要注意二者坐标系的区别，图像逆时针旋转对应的旋转角为负值。（正常二维坐标系，逆时针是向y轴正轴方向转，在图像坐标系也是这样，不过图像坐标系y轴正轴是向下的，和正常的相反）

#### 以(0,0)为中心旋转

最简单的一种，只要注意上面说的坐标系区别，旋转角正负即可。

对应矩阵
$$
\left[
\begin{matrix}
cos(\alpha)&-sin(\alpha)&0\\
sin(\alpha)&cos(\alpha)&0\\
0&0&1
\end{matrix}
\right]
$$
这种旋转会导致图像很大部分显示不出来。

#### 以图像的中心为中心旋转

下面的w，h为图像的宽度和高度

思路：

1. 先进行坐标系转换，把图像中心转换为坐标原点，并且将图像坐标系转换为正常的二维坐标系。如下图

   ![img](https://pic002.cnblogs.com/images/2010/221871/2010122615243629.png)

   这一步转换对应的矩阵为：
   $$
   A_1=\left[
   \begin{matrix}
   1&0&-\frac{w}{2}\\
   0&-1&\frac{h}{2}\\
   0&0&1
   \end{matrix}
   \right]
   $$

2. 进行旋转变换，注意这里和二维向量的旋转一模一样，即正的角度代表逆时针旋转，因为已经转换了坐标系

   这一步转换对应的矩阵为：
   $$
   A_2=\left[
   \begin{matrix}
   cos(\alpha)&-sin(\alpha)&0\\
   sin(\alpha)&cos(\alpha)&0\\
   0&0&1
   \end{matrix}
   \right]
   $$

3. 将坐标系转换回去，转换为原来的图像坐标系，原点也转换为原来的。是第一步的逆过程

   对应的矩阵是第一步矩阵的逆矩阵
   $$
   A_3=A_1^{-1}=\left[
   \begin{matrix}
   1&0&\frac{w}{2}\\
   0&-1&\frac{h}{2}\\
   0&0&1
   \end{matrix}
   \right]
   $$

将以上三步矩阵组合起来，得到最终的变换矩阵。要注意组合顺序，先进行的在右边，因为是右乘坐标点。
$$
A=A_3 A_2 A_1=\left[
\begin{matrix}
1&0&\frac{w}{2}\\
0&-1&\frac{h}{2}\\
0&0&1
\end{matrix}
\right]
\left[
\begin{matrix}
cos(\alpha)&-sin(\alpha)&0\\
sin(\alpha)&cos(\alpha)&0\\
0&0&1
\end{matrix}
\right]
\left[
\begin{matrix}
1&0&-\frac{w}{2}\\
0&-1&\frac{h}{2}\\
0&0&1
\end{matrix}
\right]
$$
运算后得到
$$
A=\left[
\begin{matrix}
cos(\alpha)&sin(\alpha)&(-wcos\alpha-hsin\alpha+w)/2\\
-sin(\alpha)&cos(\alpha)&(wsin\alpha-hcos\alpha+h)/2\\
0&0&1
\end{matrix}
\right]
$$
为最终的变换矩阵。

这样旋转会使图像中心部分出现在最终输出图像的中部，但边缘四个角的图像部分会显示不出来。

#### 旋转后显示全部图像

思路：

1. 首先输出图像的尺寸要确保可以显示旋转后的图像。如下图

   ![img](https://pic002.cnblogs.com/images/2010/221871/2010122616090174.png)

   那么可以根据旋转角，确定输出图像（说成画布可能更好理解）的尺寸。高度h‘，宽度w’。
   $$
   h' = hcos\alpha + wsin\alpha\\
   w' = wcos\alpha + hsin\alpha
   $$

2. 以图像中心为中心旋转得到图像，将这个图像的中心挪到新尺寸画布的中心，即再做一次平移变换。

   这个平移变换矩阵为：
   $$
   \left[
   \begin{matrix}
   1&0&(w'-w)/2\\
   0&1&(h'-h)/2\\
   0&0&1
   \end{matrix}
   \right]
   $$
   再将其和中心旋转对应的矩阵组合
   $$
   \left[
   \begin{matrix}
   1&0&(w'-w)/2\\
   0&1&(h'-h)/2\\
   0&0&1
   \end{matrix}
   \right]
   \left[
   \begin{matrix}
   cos(\alpha)&sin(\alpha)&(-wcos\alpha-hsin\alpha+w)/2\\
   -sin(\alpha)&cos(\alpha)&(wsin\alpha-hcos\alpha+h)/2\\
   0&0&1
   \end{matrix}
   \right]
   $$
   最终得到变换矩阵
   $$
   \left[
   \begin{matrix}
   cos(\alpha)&sin(\alpha)&(-wcos\alpha-hsin\alpha+w')/2\\
   -sin(\alpha)&cos(\alpha)&(wsin\alpha-hcos\alpha+h')/2\\
   0&0&1
   \end{matrix}
   \right]
   $$
   和上一种中心旋转得到的A矩阵，有另一种数值关系：
   $$
   A[0,2] += (w'-w)/2\\
   A[1,2] += (h'-h)/2
   $$
   就得到了最终显示全部的旋转变换矩阵。

### 倾斜/切变

原理：

以x方向倾斜$\alpha$为例，此时y方向保持不变（大概就是矩形拉伸成平行四边形）。
$$
y' = y
$$
x方向上，变换后的x‘与y值有关，y越大，倾斜的越大，中间的比例系数就是$tan\alpha$
$$
x' = x+ytan\alpha
$$
如下图

<img src="/home/paul/下载/188271077.jpg" style="zoom:25%;" />

写成变换矩阵：
$$
shear_x=\left[
\begin{matrix}
1&tan\alpha&0\\
0&1&0\\
0&0&1
\end{matrix}
\right]
$$
同理，对于y方向倾斜$\theta$，有
$$
x' = x\\
y' = y+xtan\theta\\
shear_y=\left[
\begin{matrix}
1&0&0\\
tan\theta&1&0\\
0&0&1
\end{matrix}
\right]
$$
对于两个方向都倾斜，直接组合即可（可以看做先在一个方向倾斜，再倾斜另一个方向。顺序不敏感，下面式子先算y方向的，再算x方向的，反之亦可，结果相同）。
$$
shear_{xy} = shear_xshear_y=\left[
\begin{matrix}
1&tan\alpha&0\\
0&1&0\\
0&0&1
\end{matrix}
\right]
\left[\begin{matrix}1&0&0\\tan\theta&1&0\\0&0&1\end{matrix}\right]
=\left[\begin{matrix}1&tan\alpha&0\\tan\theta&1&0\\0&0&1\end{matrix}\right]
$$
对于输出画布的尺寸，应在进行倾斜的方向加上最大可能的倾斜幅度（对于x方向，就是$htan\alpha$）。

### 综合的仿射变换——数字图像处理课的映射四边形问题和人脸关键点对齐问题

这个问题更准确说是给定src图像上几个坐标点和dst图像上对应的坐标点，求src到dst的仿射变换矩阵。本质问题就是通过给定的对应坐标点条件，求解仿射矩阵，即解多元线性方程（超定方程）。

#### 仿射矩阵自由度

涉及到求解方程，肯定要考虑未知数个数，仿射矩阵未知数个数就是自由度。

1. 最一般的仿射矩阵

$$
\left[
\begin{matrix}
a&b&tx\\
c&d&ty\\
0&0&1
\end{matrix}
\right]
$$

6个未知数，自由度为6

2. 旋转+平移仿射（旋转和平移的组合，称作刚体变换或欧式变换）

$$
\left[
\begin{matrix}
cos(\alpha)&-sin(\alpha)&tx\\
sin(\alpha)&cos(\alpha)&ty\\
0&0&1
\end{matrix}
\right]
$$

有3个未知数，$\alpha,t_x,t_y$，自由度为3

3. 旋转+平移+uniform scaling仿射（这个组合称作相似变换）

$$
\left[
\begin{matrix}
Scos(\alpha)&-Ssin(\alpha)&tx\\
Ssin(\alpha)&Scos(\alpha)&ty\\
0&0&1
\end{matrix}
\right]
$$

​       推导：
$$
\left[\begin{matrix}Scos(\alpha)&-Ssin(\alpha)&tx\\Ssin(\alpha)&Scos(\alpha)&ty\\0&0&1\end{matrix}\right]
=\left[
\begin{matrix}
cos(\alpha)&-sin(\alpha)&tx\\
sin(\alpha)&cos(\alpha)&ty\\
0&0&1
\end{matrix}
\right]
\left[
\begin{matrix}
S&0&0\\
0&S&0\\
0&0&1
\end{matrix}
\right]
$$
有4个未知数，$S,\alpha,t_x,t_y$，自由度为4

可以参照这上面讲的https://www.cnblogs.com/shine-lee/p/10950963.html，其中对于空间和基的角度理解仿射变换矩阵这一点很不错。

#### 求解仿射矩阵

一对对应坐标点可以列出两个方程，所以根据要使用的仿射矩阵自由度，决定需要的对应点对数。对应点对数至少是自由度的一半（一对对应点列两个方程）。

##### 人脸对齐

考虑到人脸对齐后的效果要求，不能在变换过程中产生形变，所以不能使用切变/倾斜变换和non-uniform scaling，所以变换就是平移+旋转+uniform scaling，即上面说的第三种情况，相似变换，自由度为4。

一般人脸检测检测出5个关键点，为$(x_1,y_1),(x_2,y_2),...,(x_5,y_5)$，对应的对齐后的位置为$(m_1,n_1),(m_2,n_2),...,(m_5,n_5)$

映射关系即方程组为
$$
\left[
\begin{matrix}
m_1&m_2&m_3&m_4&m_5\\
n_1&n_2&n_3&n_4&n_5\\
1&1&1&1&1
\end{matrix}
\right]=
\left[
\begin{matrix}
Scos(\alpha)&-Ssin(\alpha)&tx\\
Ssin(\alpha)&Scos(\alpha)&ty\\
0&0&1
\end{matrix}
\right]
\left[
\begin{matrix}
x_1&x_2&x_3&x_4&x_5\\
y_1&y_2&y_3&y_4&y_5\\
1&1&1&1&1
\end{matrix}
\right]
$$
等价的，可以写成
$$
\left[
\begin{matrix}
m_1\\
n_1\\
m_2\\
n_2\\
m_3\\
n_3\\
m_4\\
n_4\\
m_5\\
n_5
\end{matrix}
\right]=
\left[
\begin{matrix}
x_1&-y_1&1&0\\
y_1&x_1&0&1\\
x_2&-y_2&1&0\\
y_2&x_2&0&1\\
x_3&-y_3&1&0\\
y_3&x_3&0&1\\
x_4&-y_4&1&0\\
y_4&x_4&0&1\\
x_5&-y_5&1&0\\
y_5&x_5&0&1\\
\end{matrix}
\right]
\left[
\begin{matrix}
Scos\alpha\\
Ssin\alpha\\
t_x\\
t_y\\
\end{matrix}
\right]
$$
上式可记做
$$
Y=XA
$$
由最小二乘法（广义逆？，参考https://blog.csdn.net/wlzard/article/details/77840275，好像就是吴恩达讲的线性回归的什么解法。。。），可推出
$$
A=(X^TX)^{-1}X^TY
$$

##### 数字图像处理课的映射四边形问题

和上面同理，只是没有形变限制，所以使用最一般的自由度为6的仿射矩阵，求解之，即可。

## Canny边缘检测

官方教程讲的不错，不过有几个错误，下面是复制过来并且修改了的。

**整体步骤：**

1. 使用高斯滤波；
2. 在$x$方向和$y$方向上使用Sobel滤波器，在此之上求出边缘的强度和边缘的梯度；
3. 对梯度幅值进行非极大值抑制（Non-maximum suppression）来使边缘变得更细；
4. 使用滞后阈值来对阈值进行处理。

**步骤1和2的细化步骤：**

1. 将图像进行灰度化处理；

2. 将图像进行高斯滤波（$5\times5$，$s=1.4$）；

3. 在$x$方向和$y$方向上使用Sobel滤波器，在此之上求出边缘梯度$f_x$和$f_y$。边缘梯度可以按照下式求得：
   $$
   \text{edge}=\sqrt{{f_x}^2+{f_y}^2}\\
   \text{tan}=\arctan(\frac{f_y}{f_x})
   $$

4. 梯度方向量化：(这里根据代码对教程进行了较大修改)

   4.1 先将得到弧度转换为角度值angle（角度值是和x正方向夹角，从tan的计算可以看出来）

   4.2 将角度值范围控制在-22.5~157.5（控制方法见代码）

   4.3 按下面公式进行量化

   （note：这个梯度角度方向和sobel模板的定义有关系，如果是按本文档写的矩阵，那么方向和图像坐标系是一致的，如果按官方教程给的，那么方向和普通二维坐标系是一致的，其实本质问题就是梯度图以上面还是下面为y轴的正方向）
   $$
   \text{angle}=\begin{cases}
   0 & (\text{if}\quad angle \leq 22.5)\\
   45 & (\text{else if}\quad angle \leq 67.5)\\
   90  &(\text{else if}\quad angle \leq 112.5)\\
   135  &(\text{otherwise})
   \end{cases}
   $$



**步骤3：**

见https://blog.csdn.net/u010551600/article/details/80507271，其中提出了不量化方向，用插值的方法

对求出的边缘梯度进行非极大值抑制，来对边缘进行细化。

非极大值抑制是对除去非极大值以外的值的操作的总称（这个术语在其它的任务中也经常出现）。

在这里，我们比较我们我们所关注的地方边缘的法线方向（即梯度方向，这里可以从为什么梯度可以引出边缘考虑——垂直于边缘的方向梯度会产生明显变化）邻接的三个像素点的梯度幅值，如果该点的梯度值不比其它两个像素大，那么这个地方的值设置为0。

也就是说，我们在注意梯度幅值$\text{edge}(x,y)$的时候，可以根据下式由梯度方向$\text{angle}(x,y)$来变换$\text{edge}(x,y)$：（按照本文档的sobel模板推出）

- $\text{angle}(x,y)  = 0$

  如果在$\text{edge}(x,y)$、$\text{edge}(x-1,y)$、$\text{edge}(x+1,y)$中$\text{edge}(x,y)$不是最大的，那么$\text{edge}(x,y)=0$；

- $\text{angle}(x,y)  = 45$

  如果在$\text{edge}(x,y)$、$\text{edge}(x-1,y-1)$、$\text{edge}(x+1,y+1)$ 中$\text{edge}(x,y)$不是最大的，那么$\text{edge}(x,y)=0$；

- $\text{angle}(x,y)  = 90$

  如果在$\text{edge}(x,y)$、$\text{edge}(x,y-1)$、$\text{edge}(x,y+1)$中$\text{edge}(x,y)$不是最大的，那么$\text{edge}(x,y)=0$；

- $\text{angle}(x,y)  = 135$

  如果在$\text{edge}(x,y)$、$\text{edge}(x-1,y+1)$、$\text{edge}(x+1,y-1)$中$\text{edge}(x,y)$不是最大的，那么$\text{edge}(x,y)=0$；

**步骤4：**

在这里我们将通过设置高阈值（HT：high threshold）和低阈值（LT：low threshold）来将梯度幅值二值化。滞后阈值。

1. 如果梯度幅值$edge(x,y)$大于高阈值的话，令$edge(x,y)=255$；
2. 如果梯度幅值$edge(x,y)$小于低阈值的话，令$edge(x,y)=0$；
3. 如果梯度幅值$edge(x,y)$介于高阈值和低阈值之间并且周围8邻域内有比高阈值高的像素点存在，令$edge(x,y)=255$；

在这里，我们使高阈值为100，低阈值为20。顺便说一句，阈值的大小需要边看结果边调整。

上面的算法就是Canny边缘检测算法了。