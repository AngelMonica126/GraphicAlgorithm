
## 间接光照
这篇文章可以说是全局光照中一篇超级经典的文章，大家不要被他的名字所骗了Reflective Shadow Maps直译过来是反射阴影贴图，可他不是用来解决阴影问题的，而是用来解决间接光照问题的文章。光照可以分为两类：一类为直接光照，这类光照是直接由光源或者发光物发出的光照；另一类为直接光照之后反射的光照称为间接光照。直接光照的计算十分简单，因为计算的范围和次数（没有反射）有限。而对于间接光照而言计算就十分复杂，因为间接光照可能不止反射了一次，一束光可能需要经历了很多次反射才能到达着色点。而间接光照又十分重要，可以看下面两张图第一张是只有直接光照，第二张是直接光照加上间接光照的效果，大家可以看到地板上面有间接光照的会受到周围环境的影响从而更加的逼真。因此间接光照的模拟是图形学中一个经久不衰的话题。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612190307549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzNTM2OTgx,size_16,color_FFFFFF,t_70 =500x300)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612190307792.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzNTM2OTgx,size_16,color_FFFFFF,t_70 =500x300)

## 虚拟点光源
RSM的核心思想是把直接光源照亮的区域又作为发光物来进行计算。当然RSM只计算一次反射，也就是说先计算直接光照对场景的影响，然后将直接光照照亮的区域再用于计算对场景的影响，一共计算两次光照。
* 第一步是先单独计算直接光照对环境的影响。
* 第二步就是要获得直接光照亮的区域，那么该如何获得这块区域呢？其实有个很简单的办法，从光源的视角进行一次渲染就可以得到从光源视角看的区域。
* 第三步就是要用第二步计算的发光区域用于计算间接光照，不过无法直接用这个区域计算间接光照，因此RSM的核心就是把这个发光区域的每一个像素都看成一个光源叫做虚拟点光源（Virtual point light：VPL），然后用这所有的虚拟点光源来计算间接光照。
* 第四步再将间接光照和直接光照融合起来（直接相加即可）。
## 具体细节
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612200210861.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzNTM2OTgx,size_16,color_FFFFFF,t_70#pic_center)
着色点x收到的虚拟点光源Xp的辐射强度如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612200114319.png)
着色点收到的所有的虚拟点光源的辐射强度公式如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612200502801.png)
## 改进
这个算法非常直接简单，不过简单的代价就是效率低，大家可以想象如果发光区域很大的话比如长宽为1024*1024，那么虚拟点光源的数量就为1024*1024个，如果要老老实实的计算所有虚拟点光源的话效率太过低下。因此文章提出了一个改进的办法，我们不把所有的VPL都用于间接光照的计算，而是有一个侧重的选取。那如何侧重呢，作者假设屏幕空间里面里这个着色点越近的那么对光源贡献就更大。因此我们多选取一点离着色点近的VPL，并且为了弥补越往外采样数越少可能会带来的问题，作者引入了权重，越近了权重越小，越远的权重越大。如下图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612202420691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzNTM2OTgx,size_16,color_FFFFFF,t_70#pic_center)
可以看到随着采样距离变大，采样密度在逐渐减小。并且为了弥补越往外采样数越少可能会带来的问题，作者让样本的权重随着采样变大而逐渐增大。采样公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200612203033855.png#pic_center)
(s,t)是着色点的屏幕空间的坐标，我们可以通过生成两个随机数ξ1，ξ2代入上面的公式就可以得到一个采样坐标，然后就可以去第二步生成的发光区域的纹理来采样生成一个VPL，然后再根据上面的光照计算公式来计算这个VPL对着色点的光照公式，然后再乘以权重ξ1*ξ2即可。

代码链接 https://github.com/AngelMonica126/GraphicAlgorithm/tree/master/TestCase_001_A
