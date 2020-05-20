# 数字图像处理note

[TOC]

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

- [ ] 保边滤波器 https://blog.csdn.net/eejieyang/article/details/52333112

## 其他的滤波器

- motion filter，对图像进行运动模糊处理，kernel是对对角线元素取平均。t12。

## 边缘检测滤波器

一个点，边缘检测的滤波器sum为0，去噪滤波器sum为1（做了归一化）。

另一个点，滤波其实是对每个像素点进行操作，输出操作（融合周围的信息以达到某种运算目的（梯度，二阶微分...））后的像素点。

还一个点，用**梯度来检测边缘的原理**：梯度其实就是相邻像素之间的变化量，边缘的外边的像素点与里面的像素点存在巨大的变化，所以体现在梯度上，就是梯度值很大（应该说绝对值）；而在内部区域，像素点之间变化很小，对应的梯度就很小。由此，还可以引出一个点，就是梯度方向和边缘方向垂直。

还一个点，内部区域的像素点不论是一阶微分还是二阶微分都是0。这个点对于一阶微分检测边缘没有影响，因为一阶就是找一阶微分的极值点，0肯定不是。对于二阶微分检测是有影响的，因为理论上讲，二阶微分应该是检测二阶微分的过零点，但内部点的二阶微分也是0，但这些点肯定不是边缘。所以二阶微分还是用越大越是边缘的思路，因为边缘点很难做到一定是0。

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

梯度方向：
$$
\text{tan}=\arctan(\frac{G_y}{G_x})
$$
一阶微分其实一个更重要的作用是求梯度图，这在hog特征等特征提取中均有体现。


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

二阶微分的过零点为要检测的边缘

参考：https://blog.csdn.net/u014485485/article/details/78364573

二阶微分具有旋转不变性，但同时也因此失去了边缘的方向信息，所以后面canny边缘检测、hog特征提取，都是用的一阶微分，以得到梯度的方向信息。

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
  
- DoG滤波器。https://blog.csdn.net/u014485485/article/details/78364573 用差分的方法取代LoG的计算，但可以达到LoG的效果。教程里的t74就是变相的DoG，只是直接用的插值尺度变换来代替高斯滤波了。

  推导过程：

  LoG算子对$\sigma$求导，有
  $$
  \frac{\partial\nabla^2Guassian_\sigma}{\partial \sigma}=\frac{x^2+y^2-\sigma^2}{2\pi\sigma^5}exp(-\frac{x^2+y^2}{2\sigma^2})=\sigma \nabla^2Guassian_\sigma
  $$
  从导数定义出发，有
  $$
  \frac{\partial\nabla^2Guassian_\sigma}{\partial \sigma}=\lim_{\triangle \sigma\to0}\frac{Guassian_{\sigma+\triangle\sigma}(x,y)-Guassian_\sigma(x,y)}{(\sigma+\triangle\sigma)-\sigma}\\\
  approx \frac{Guassian_{k\sigma}(x,y)-Guassian_\sigma(x,y)}{(k-1)\sigma}
  $$
  综合上面两式，可以得到
  $$
  \nabla^2Guassian_\sigma=\frac{Guassian_{k\sigma}(x,y)-Guassian_\sigma(x,y)}{(k-1)\sigma^2}
  $$
  忽略掉分母的常系数，可以用
  $$
  Guassian_{k\sigma}(x,y)-Guassian_\sigma(x,y)
  $$
  近似表示LoG算子，这个本身就叫做DoG算子，D是差分的意思

  最终，得到
  $$
  DoG(f)=Guassian_{k\sigma}(x,y)-Guassian_\sigma(x,y)
  $$
  用这个算子与图像做卷积操作，即可。

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

### 透明混合

$$
out = img1*\alpha+img2*(1-\alpha)
$$

### HSV空间

H：色相；S：饱和度；V：明度

### YCbCr空间

Y：亮度；Cb：蓝色色度；Cr：红色色度

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

## 霍夫变换

### 检测直线

参考：https://www.cnblogs.com/AndyJee/p/3805594.html

霍夫变换中直线方程推导：https://blog.csdn.net/qq_25847123/article/details/74612976

直线方程推导过程自己写一遍吧：

<img src="https://img-blog.csdn.net/20170706213251494" alt="img" style="zoom: 80%;" />

k为斜率，$\theta$为垂线与x轴夹角，r为原点到直线的距离
$$
\frac{r\sin\theta-y}{r\cos\theta-x}=k=-\frac{1}{\tan\theta}=-\frac{\cos\theta}{\sin\theta}
$$

$$
-\sin\theta(r\sin\theta-y) = \cos\theta(r\cos\theta-x)\\
-r\sin^2\theta+y\sin\theta=r\cos^2\theta-x\cos\theta
$$

$$
r=x\cos\theta+y\sin\theta
$$

霍夫变换具体过程步骤参考官方教程，和自己代码的实现。

### 检测圆

参考：https://www.iteye.com/blog/lps-683-2256012

圆方程推导：

一般的圆方程：
$$
(x-a)^2+(y-b)^2=r^2
$$
引入$\theta$参数
$$
x = r\cos\theta+a\\
y = r\sin\theta+b
$$
最终形式
$$
a=x-r\cos\theta\\
b=y-r\sin\theta
$$

检测圆的复杂度太高。。。还没有代码实现

## 模式匹配

大概是传统cv的**特定**目标检测

具体做法官方教程讲的很好，可参考之。其中NCC的公式有问题，修改为：
$$
S=\frac{\sum\limits_{x=0}^w\ \sum\limits_{y=0}^h\ I(i+x,j+y)\  T(x,y)}{\sqrt{\sum\limits_{x=0}^w\ \sum\limits_{y=0}^h\ I(i+x,j+y)^2}\  \sqrt{\sum\limits_{x=0}^w\ \sum\limits_{y=0}^h\ T(x,y)^2}}
$$
即分子没有绝对值

关于zero-mean NCC，$m_i$表示整个图像的平均值。

几个算法都是衡量两个图像区域之间的相关性，用不同的衡量标准，类似于不同的loss。

NCC对变化敏感的原因：用相关性衡量，相关性是对应像素点相乘再相加，像素点的轻微变化经过乘法之后会放大很多，而对于SSD，SAD来说，减法对于微小的变化并不敏感。

## 形态学运算

参考：https://www.cnblogs.com/angle6-liu/p/10704970.html

https://www.cnblogs.com/little-monkey/p/7236188.html

所有形态学运算都是针对二值化图像，即黑白图。

### 膨胀和腐蚀

#### 膨胀

insight：让白色的膨胀出去

原理：对于一个黑色的点，只要四周有白色的，这一点也变成白色

具体实现：卷积操作，模板见官方教程。（其实这样卷积效率很低，直接循环判断就可以了）。

#### 腐蚀

insight：让白色的腐蚀进来，或者叫收缩

原理：对于一个白色的点，只要四周有黑色的，这一点也变成黑色。所以可以发现，所谓腐蚀，就是对黑色部分的膨胀。

#### 二者关系

膨胀和腐蚀都是针对白色部分而言，对于黑色部分，二者颠倒过来就可以了。

膨胀是对白色膨胀，对黑色腐蚀。

腐蚀是对黑色膨胀，对白色腐蚀。

### 开运算 闭运算

参考：https://www.cnblogs.com/-wenli/p/11735633.html

开运算：

先腐蚀N次，再膨胀N次。

作用：去除孤立的小点、毛刺和小桥。

闭运算：

先膨胀N次，再腐蚀N次。

作用：填平小湖（即小孔），弥合小裂缝。

开运算和闭运算最终起作用的都是第一步，即开运算的腐蚀和闭运算的膨胀，再进行一步相反处理的作用是保证总的位置和形状不变。

开闭运算实际上是用了腐蚀膨胀不可逆的特点，腐蚀或膨胀后的图像再变换时只和当前的图像内容有关，比如白色孤立点在腐蚀后被去掉了，再做膨胀也变不回来的。

### 形态学梯度

图像经过膨胀操作的输出与经过腐蚀操作的输出，二者做差，可抽取出物体的边缘。

原理大概是膨胀与腐蚀对原图像变换最大的部分就是边缘（膨胀或腐蚀边缘）。

### 顶帽 黑帽

顶帽：二值化图像和开运算图像的差，分离比临近区域亮的斑点

黑帽：二值化图像和闭运算图像的差，分离比邻近区域暗的斑点

## 连通域

关于Look Up Table（LUT）https://www.cnblogs.com/zsychanpin/p/7399627.html

直接用dfs他不香吗？官方教程给的算法bug太多了。

就是典型的求岛屿连通个数问题啊，不要整那么麻烦的，具体见代码。

## 细化处理

### 连接数

参考：http://ftp.gongkong.com/UploadFile/datum/2008-10/2008101416013500001.pdf

作用是判断二值图像像素点的位置属性

官方教程给的算法不错，可参考，补充几点：

- S=0，还可以表示是孤立点
- 4连接数计算公式中，$x_i$表示这一点的像素值，为0或1（二值图像，0对应0，255对应1，8连接数的进行取反）
- 只针对像素值为1(255）的像素计算

### 细化算法

（ps. 其实腐蚀也算是一种细化算法啊）

最基础版本的修正：第一个条件修改为有一个或以上的4邻域值为0，或者说四邻域值不全为1

#### Hilditch算法

1. 从左上角开始进行sliding window；
2. $x_0(x,y)=0$的话、不进行处理。$x_0(x,y)=1$的话，下面五个条件都满足的时候令$x_0=-1$（原地修改）：
   1. 当前像素的4-近邻中有一个或以上为0；（如果都为1，去掉中心点，就会产生空洞）
   2. $x_0$的8-连接数为1；（计算连接数时，不能用原地修改的图像，应该用之前拷贝的一份，即图像中不能有-1出现）
   3. $x_1到x_8$的绝对值之和大于2；
   4. 8-近邻像素的取值有一个以上为$1$；
   5. 对所有$x_n$($n\in [1,8]$)以下任一项成立：（这个条件的作用是去掉一些点后，对周围其他点的连接数等条件不产生影响）
      - 不是$-1$；
      - 当$x_n$为$0$时，$x_0$的$8-$连接数为$1$。(仍然使用之前拷贝的一份图像，并再次拷贝一份，这一份的对应的$x_n$点设为0，再计算相应的连接数)
3. 将每个像素的$-1$更改为$0$；
4. 重复进行光栅扫描，直到某一次光栅扫描中步骤3的变化数变为$0$。

## HOG

参考：

https://www.jianshu.com/p/395f0582c5f7 （重点看这个）

https://www.cnblogs.com/alexme/p/11361563.html

### 总结整理一下算法步骤

1. **求图像梯度幅值和方向**

   1. 灰度化图像（并对图像进行gamma校正）

   2. 求水平方向梯度和数值方向梯度，使用模板为
      $$
      grad_x=
      \left[
      \begin{matrix}
      0&0&0\\
      -1&0&1\\
      0&0&0
      \end{matrix}
      \right]\\
      grad_y=
      \left[
      \begin{matrix}
      0&-1&0\\
      0&0&0\\
      0&1&0
      \end{matrix}
      \right]
      $$

   3. 求出梯度幅值：$grad_m=\sqrt{grad_x^2+grad_y^2}$

      求出梯度方向：$grad_d=arctan(grad_y/grad_x)$

   4. 对梯度方向进行量化，一般以20为一级，为0-8，共9级

2. **计算每个cell的方向梯度直方图**

   1. 确定一个cell的尺寸N×N，一般为8*8，进一步可确定一个图像有多少个cell

   2. 在每个cell内，计算同一个梯度方向数量级的梯度幅度累加和，构成9个bin的直方图。

      其实是每个cell得到了一个9维向量

3. **直方图归一化**

   1. 确定一个block的尺寸，即一个block包括多少个cell

   2. 在每个block内，对每个cell进行归一化，归一化方法为：

      1. 先计算这个block的norm
         $$
         norm = \sqrt{\sum_i^C\sum_j^C\sum_c^9cell_{i,j}^2(c)+\epsilon}
         $$
         每个block有C×C个cell

         对区域和方向数量级同时做归一化

      2. 对每个cell，除norm

      3. 将每个cell归一化后的直方图（9维向量）进行concat，得到9×C×C维向量，即这个block对应的向量

   3. 滑动窗口依次生成block（类似于卷积操作），对每个block重复上面的操作，每个都得到一个向量

   4. 滑动完毕后，将所有block的向量做concat，即得到这个图像对应的向量，也就是hog特征描述子

## 色彩追踪

基本原理：先做hsv变换，得到h颜色空间，根据要追踪颜色的h范围，确定其位置即可

官方教程关于这一块讲的不错，可以参考之。

## 高斯金字塔

参考：https://www.cnblogs.com/wynlfd/p/9704770.html

### 显著图 saliency map

传统方法：通过高斯金字塔的两两差分再叠加可以得到

## 角点检测

角点概念：轮廓（边缘）的角点，也就是几何意义的“角”。角点周围的梯度幅值和方向都有大幅度的变化。

### Harris角点检测

参考：

https://www.cnblogs.com/zyly/p/9508131.html

https://blog.csdn.net/xuxiatian/article/details/52053511

#### 推导过程

##### 1. 基本出发点

使用一个固定窗口在图像上进行**任意方向**上的滑动，比较滑动前与滑动后窗口中的像素灰度变化程度，如果任意方向上的滑动，都有着较大灰度变化，那么我们可以认为该窗口中存在角点。

用公式表示为：
$$
E(u,v)=\sum_{x,y\in W}w(x,y)[f(x+u,y+v)-f(x,y)]^2
$$
其中，W表示窗口大小，u,v表示窗口滑动量，f为图像灰度，

$w(x,y)$为窗口函数，如果平等看待窗口中每一个像素的话，可以设为1；如果突出检测中心像素是不是角点，可以设为二维高斯分布。一般设w为二维高斯分布。

##### 2. 对$E(u,v)$进行化简以求解

这里说的化简其实更应该说是进一步精确，因为如果直接计算E式的话，只能对滑动的方向进行量化，比如量化为0,45,90,135这四个方向，不能体现任意方向这一点要求。

化简或者说进一步精确的方法是采用微分，对$f(x+u,y+v)$进行一阶泰勒展开，可以得到（注意这里的变量是u,v，对u,v进行展开）
$$
f(x+u,y+v) \approx f(x,y)+uf_x(x,y)+vf_y(x,y)
$$
其中，令$I_x=f_x(x,y)$表示横向梯度，$I_y=f_y(x,y)$表示纵向梯度

带入E式，有
$$
E(u,v) \approx \sum_{x,y \in W}w(x,y)[f(x,y)+uf_x(x,y)+vf_y(x,y)-f(x,y)]^2\\
= \sum_{x,y \in W}w(x,y)[uf_x(x,y)+vf_y(x,y)]^2\\
= \sum_{x,y \in W}w(x,y)(u^2I_x^2+2uvI_xI_y+v^2I_y^2)\\
=\left[
\begin{matrix}
u&v
\end{matrix}
\right]
(\sum_{x,y \in W}w(x,y)
\left[
\begin{matrix}
I_x^2&I_xI_y\\
I_xI_y&I_y^2
\end{matrix}
\right])
\left[\begin{matrix}u\\v\end{matrix}\right]
$$
记
$$
M=\sum_{x,y \in W}w(x,y)
\left[
\begin{matrix}
I_x^2&I_xI_y\\
I_xI_y&I_y^2
\end{matrix}
\right]
\\
=\left[
\begin{matrix}
\sum w(x,y)I_x^2&\sum w(x,y)I_xI_y\\
\sum w(x,y)I_xI_y&\sum w(x,y)I_y^2
\end{matrix}
\right]
$$
注：M矩阵中的每个元素都是一个具体的值，每个像素点都有一个对应的M矩阵，以左上角元素为例，先是这个像素点的梯度值运算（$I_xI_x$），再与窗口函数（高斯滤波）做以其为中心的卷积，得到最终的一个值。

到这地方还是很顺利的，但接下来很多博客的解释就让人云里雾里，因为很多这里直接跳到了Ix，Iy的坐标系上，脱离了出发点本质。花了一天时间，终于有了我自己的理解。

##### 3. 用椭圆曲面把$E(u,v)$的大小转换为对M矩阵的分析

现在把E式写成：
$$
E(u,v) =\left[
\begin{matrix}
u&v
\end{matrix}
\right]
M
\left[\begin{matrix}u\\v\end{matrix}\right]
$$
E是一个二项函数，也就是椭圆函数，E这个二维函数在三维坐标系曲面的投影就是椭圆，椭圆的形状（长短轴）取决于M矩阵，更准确的说，取决于M的特征值。（注：这里的坐标系维度分别是u,v,E）

设M的特征值为$\lambda_1,\lambda_2$，对应的椭圆长轴为a，短轴为b，有
$$
a=\frac{1}{\sqrt{\lambda_1}}\\
b=\frac{1}{\sqrt{\lambda_2}}
$$
长短轴的方向则是对应的特征向量的方向。

根据基本出发点，

1. 对于内部平坦区域，u,v的变化对E值只有很小的影响，即使很大的滑动，E值也不怎么变化。要想达到这一点，E椭圆曲面的**长轴和短轴都应该特别大**，这是因为长轴和短轴都大，导致曲面会很宽，u,v的变化只能导致非常小幅度的E值上升（想象一下函数曲面就可以了，很像一个口径特别大的碗，往里面倒水，水的高度涨的很慢，当然这只是不恰当的比喻），更准确说，E关于u和v的偏导数都很小。长轴和短轴都大，对应的就是**M的特征值$\lambda_1,\lambda_2$都小**。
2. 对于角点，则完全和内部平坦区域相反，u,v稍微的变化，就会对E值产生剧烈的影响，那么E椭圆曲面的**长轴和短轴就都应该特别小**，对应的**M的特征值$\lambda_1,\lambda_2$都大**。
3. 对于边缘，u或v其中一个的变化对E值有较大影响，另外一个则影响很小。那么**E椭圆曲面是一个轴大，一个轴小**，对应的**M的特征值也是一小一大**。

以上三点，是这个推导的核心了。

总结一下，

内部：两个特征值都小

边缘：两个特征值一大一小

角点：两个特征值都大

##### 4. 统一公式来表达特征值大小关系

现在问题已经转化为求每个像素点的M矩阵的特征值，根据两个特征值的大小关系来确定该像素点是哪种区域。

但是这样还有问题：1. 计算特征值很费时间；2. 怎么量化大小关系？

解决方法是用一个式子来统一表示，不用计算特征值，同时实现了量化
$$
R=det(M)-k(trace(M))^2
$$
其中，$det(M)$为M的行列式，trace为M的迹，这二者和特征值之间的关系：
$$
det(M) = \lambda_1\lambda_2 \\
trace(M) = \lambda_1+\lambda_2
$$
k为经验参数，远小于1。

即
$$
R=\lambda_1\lambda_2-k(\lambda_1+\lambda_2)^2
$$
在内部或角点情况下，$\lambda_1$与$\lambda_2$都很小或者都很大，二者可以近似认为相等（很接近吧），那么
$$
R=(1-4k)\lambda_1^2
$$
可以发现R>0，如果R很大（是不是很大在实际实现上由设定的经验阈值决定），则表示两个特征值都很大；R很小（但大于0），则表示两个特征值都很小。

在边缘情况下，可以近似认为其中一个很大，另一个近似为0，那么
$$
R=-k\lambda_1^2
$$
可以发现R<0，则R<0就可以推出这个区域为边缘。

总结一下，

R>thresh：为角点

R<thresh：为内部（R>0）或边缘（R<0）

#### Harris角点检测优缺点

优点：

1. 旋转不变性（椭圆方程决定，椭圆旋转后长短轴不变，特征值不变）

缺点：

1. 不具有尺度不变形，一个弯曲的边缘缩小后，很容易就变成角点了

## DFT 傅里叶变换

实际实现的时候，对变换公式有一定修改

正变换：
$$
G(k,l)=\frac{1}{\sqrt{H\ W}}\ \sum\limits_{y=0}^{H-1}\ \sum\limits_{x=0}^{W-1}\ I(x,y)\ e^{-2\  \pi\  j\ (\frac{k\  x}{W}+\frac{l\  y}{H})}
$$
反变换：
$$
I(x,y)=\frac{1}{\sqrt{H\  W}}\ \sum\limits_{l=0}^{H-1}\ \sum\limits_{k=0}^{W-1}\ G(l,k)\ e^{2\  \pi\  j\ (\frac{k\  x}{W}+\frac{l\  y}{H})}
$$
这里得到的$I$是复数，取幅度abs作为其最终的结果，即向量的模值。

一种用矩阵相乘取代循环的方法：https://www.jianshu.com/p/98f493de01db

变换后的频域图，高频集中在图中心，低频在四角。且关于中心点呈中心对称。

## DCT 离散余弦变换

参考：https://www.cnblogs.com/wyuzl/p/7880124.html

### 正变换

#### 一般形式

$$
0\leq u,\ v\leq N\\
F(u,v)=C(u)\  C(v)\ \sum\limits_{y=0}^{N-1}\ \sum\limits_{x=0}^{N-1}\ I(x,y)\  \cos(\frac{(x+0.5)\  u\  \pi}{N}) \cos(\frac{(y+0.5)\  v\  \pi}{N})\\
C(u)=
\begin{cases}
\sqrt\frac{1}{N}& (\text{if}\ u=0)\\
\sqrt\frac{2}{N}&(\text{else})
\end{cases}
$$

其中N为图像的尺寸，这里只考虑height和width相等的情况。

#### 矩阵形式

$$
F=A\ I\ B
$$

其中，
$$
A =\left[
\begin{matrix}
c(0)\cos\frac{(0+0.5)\pi*0}{N}&c(0)\cos\frac{(1+0.5)\pi*0}{N}&...&c(0)\cos\frac{(N-1+0.5)\pi*0}{N}\\
c(1)\cos\frac{(0+0.5)\pi*1}{N}&c(1)\cos\frac{(1+0.5)\pi*1}{N}&...&c(1)\cos\frac{(N-1+0.5)\pi*1}{N}\\
...\\
...\\
c(N-1)\cos\frac{(0+0.5)\pi*(N-1)}{N}&c(N-1)\cos\frac{(1+0.5)\pi*(N-1)}{N}&...&c(N-1)\cos\frac{(N-1+0.5)\pi*(N-1)}{N}
\end{matrix}
\right]
$$
元素规律为：
$$
A(u,x)=c(u)\cos\frac{(x+0.5)\pi u}{N}
$$

$$
B =\left[
\begin{matrix}
c(0)\cos\frac{(0+0.5)\pi*0}{N}&c(1)\cos\frac{(0+0.5)\pi*1}{N}&...&c(N-1)\cos\frac{(0+0.5)\pi*(N-1)}{N}\\
c(0)\cos\frac{(1+0.5)\pi*1}{N}&c(1)\cos\frac{(1+0.5)\pi*1}{N}&...&c(N-1)\cos\frac{(1+0.5)\pi*(N-1)}{N}\\
...\\
...\\
c(0)\cos\frac{(N-1+0.5)\pi*0}{N}&c(1)\cos\frac{(N-1+0.5)\pi*1}{N}&...&c(N-1)\cos\frac{(N-1+0.5)\pi*(N-1)}{N}
\end{matrix}
\right]
$$

元素规律为：
$$
B(y,v) = c(v)\cos\frac{(y+0.5)\pi v}{N}
$$
可以发现，$B=A^T$

所以，最终矩阵形式为：
$$
F=A\ I\ A^T
$$


### 反变换

#### 一般形式

$$
1\leq K\leq N\\
f(x,y)=\sum\limits_{u=0}^{K-1}\sum\limits_{v=0}^{K-1}\ C(u)\ C(v)\ F(u,v)\ \cos(\frac{(x+0.5)\  u\  \pi}{N})\ \cos(\frac{(y+0.5)\  v\  \pi}{N})\\
C(u)=
\begin{cases}
\sqrt\frac{1}{N}& (\text{if}\ u=0)\\
\sqrt\frac{2}{N}&(\text{else})
\end{cases}
$$

其中，K为决定反变换后得到图像分辨率的参数，K=N反变换得到原图，K=1或2得到图像比原图分辨率低很多（最低），合理选择K值，可以减小图像大小，即压缩。

关于K值的原理解释：在变换之后，系数较大的集中在左上角，而右下角的几乎都是0，其中左上角的是低频分量，右下角的是高频分量，低频系数体现的是图像中目标的轮廓和灰度分布特性，高频系数体现的是目标形状的细节信息。DCT变换之后，能量主要集中在低频分量处，这也是DCT变换去相关性的一个体现。

DCT和取K值的IDCT的组合变换后，可以对图像（数据）达到压缩效果。

#### 矩阵形式

一个重要的点，A矩阵为正交矩阵，$A^T=A^{-1}$

根据正变换的矩阵形式，
$$
I=A^{-1}\ F\ (A^T)^{-1}=A^T\ F\ A
$$
这里是K=N的情况。

若K<N，则F取前K行和前K列；A取前K行，列不变。

## JPEG压缩

可以参考官方教程给的算法

有损压缩

核心是通过DCT变换，量化DCT变换的系数，以减小存储量。