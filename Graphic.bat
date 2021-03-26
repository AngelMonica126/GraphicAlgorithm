@echo off
SETX /M ASSIMP "%~dp0GraphicSDK\Assimp\Assimp" 
SETX /M BOOST "%~dp0GraphicSDK\boost_1_69\boost_1_69" 
SETX /M GLI "%~dp0GraphicSDK\gli\gli" 
SETX /M GLM "%~dp0GraphicSDK\glm\glm" 
SETX /M OPENGL "%~dp0GraphicSDK\OpenGL\OpenGL" 
SETX /M SOIL "%~dp0GraphicSDK\SOIL\SOIL" 
SETX /M STB_IMAGE "%~dp0GraphicSDK\stb_image\stb_image" 
SETX /M OPENCV "%~dp0GraphicSDK\opencv\build" 
SETX /M CIMG "%~dp0GraphicSDK\CImg" 
pause