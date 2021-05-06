#pragma once
#include <GL/glew.h>
#include <vector>
#include <GLM/glm.hpp>
#include <memory>
#include <array>
#include "FRAME_EXPORTS.h"
#include "common.h"

#define SAFE_DELETE(p) if(p){ delete p; p = nullptr; }

FRAME_DLLEXPORTS GLint  createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, const GLint vIndices[] = nullptr, GLint vIndicesSize = 0, int *voVBO = nullptr);
FRAME_DLLEXPORTS GLint  createVAO(int vBufferIndex, std::initializer_list<GLint> vAttribsLength, GLenum vDataType = GL_FLOAT);
FRAME_DLLEXPORTS GLint  genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage, GLint vBindingIndex = -1);
FRAME_DLLEXPORTS GLvoid getSSBOBuffer(GLint BufferID, GLsizeiptr vSize, GLvoid **vData);
FRAME_DLLEXPORTS GLvoid updateSSBOBuffer(GLint BufferID, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage);
FRAME_DLLEXPORTS GLvoid genTexture(std::shared_ptr<ElayGraphics::STexture> vioTexture/* = ElayGraphics::STexture()*/);
FRAME_DLLEXPORTS GLvoid genTextureByBuffer(std::shared_ptr<ElayGraphics::STexture> vioTexture, GLint vBuffer);
FRAME_DLLEXPORTS GLvoid genGenerateMipmap(std::shared_ptr<ElayGraphics::STexture> vioTexture);
FRAME_DLLEXPORTS GLvoid ClearTexture(std::shared_ptr<ElayGraphics::STexture> vioTexture, GLuint TextureType);
FRAME_DLLEXPORTS GLvoid loadTextureFromFile(const std::string& vFilePath, std::shared_ptr<ElayGraphics::STexture> voTexture2D);
FRAME_DLLEXPORTS GLvoid loadCubeTextureFromFile(const std::vector<std::string>& vFilePath, std::shared_ptr<ElayGraphics::STexture> voTexture2D);
FRAME_DLLEXPORTS GLint  createVAO4ScreenQuad();
FRAME_DLLEXPORTS GLint  createVAO4Cube();
FRAME_DLLEXPORTS GLint  createVAO4Sphere();
FRAME_DLLEXPORTS void   drawQuad();
FRAME_DLLEXPORTS void   drawCube();
FRAME_DLLEXPORTS void   drawSphere();
FRAME_DLLEXPORTS GLint  genFBO(const std::initializer_list<std::shared_ptr<ElayGraphics::STexture>>& vioTextureAttachments);
FRAME_DLLEXPORTS void   transferData2Buffer(GLenum vTarget, GLint vTargetID, std::vector<GLintptr> vOffsets, std::vector<GLsizeiptr> vSizes, std::vector<const GLvoid*> vDatas);
FRAME_DLLEXPORTS int    captureScreen2Img(const std::string& vFileName, int vQuality = 100.0);
FRAME_DLLEXPORTS void   hueToRGB(float vHue, glm::vec4& voRGB);
FRAME_DLLEXPORTS void   blitFrameBufferTexture(std::shared_ptr<ElayGraphics::STexture> vioSrcTexture, std::shared_ptr<ElayGraphics::STexture> vioDestTexture, GLenum vFilter = GL_NEAREST, std::array<bool, 4> vChannels = { true, true, true, true});

bool floatEqual(float vFloatNum1, float vFloatNum2);