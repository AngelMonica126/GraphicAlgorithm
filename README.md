[![GitHub stars](https://img.shields.io/github/stars/AngelMonica126/GraphicAlgorithm.svg?style=flat-square)](https://github.com/AngelMonica126/GraphicAlgorithm/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/AngelMonica126/GraphicAlgorithm?style=flat-square)](https://github.com/AngelMonica126/GraphicAlgorithm/issues)
[![GitHub forks](https://img.shields.io/github/forks/AngelMonica126/GraphicAlgorithm?style=flat-square)](https://github.com/AngelMonica126/GraphicAlgorithm/network)
[![GitHub last commit](https://img.shields.io/github/last-commit/AngelMonica126/GraphicAlgorithm?style=flat-square)](https://github.com/AngelMonica126/GraphicAlgorithm)


   :octopus: :octopus: :octopus: 本项目会每周复现或者解析一篇经典有趣的图形学论文(#^.^#)，大家有觉得好的图形学论文也可以发邮件给我复现。

# 实时渲染

## 全局光照

[Reflective Shadow Maps(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Reflective-Shadow-Maps)

[Stochastic Light Culling(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Stochastic-Light-Culling)

## 光照加速

[Non-interleaved Deferred Shading of Interleaved Sample Patterns(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Non-interleaved-Deferred-Shading-of-Interleaved-Sample-Patterns)

[Metalights : Improved Interleaved Shading(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Metalights-:-Improved-Interleaved-Shading)

[Tile Shading(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Tile-Shading)

## 透明度

[Interactive Order-Independent Transparency(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Interactive-Order-Independent-Transparency)

## 纹理

[Wang Tiles for Image and Texture Generation(1)(已复现)](https://github.com/AngelMonica126/GraphicAlgorithm/wiki/Wang-Tiles-for-Image-and-Texture-Generation)


# 项目配置

* 首先克隆本项目到本地

* 用管理员的权限运行 Graphic.bat 以自动配置环境变量

* 在项目中选择Frame点击生成以生成动态链接库（这是一个简单的把Opengl进行了简单封装的引擎，和原本的OpenGL没有特别的大的区别就简化了很多比如创建帧缓冲的代码）

* 然后就可以运行不同的项目了，由于我是配置的Release版的环境，如果想要在Debug下进行调试需要把每个项目下的属性->连接器->常规../x64/Release改为../x64/Debug；在把每个项目下属性->连接器->输入里面的glew32.lib 改为glew32d.lib即可
