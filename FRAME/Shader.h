//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include "FRAME_EXPORTS.h"
#include "Common.h"

class FRAME_DLLEXPORTS CShader
{
public:
	CShader() = default;
	CShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName = "",
		const std::string& vTessellationControlShaderFileName = "", const std::string& vTessellationEvaluationShaderFileName = "");
	CShader(const std::string& vComputeShaderFileName);
	~CShader();

	GLint  getUniformId(const std::string& vUniformName) const;
	GLint  getCommonUniformId(const std::string& vUniformName) const;

	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0) const;
	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1) const;
	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2) const;
	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2, GLint v3) const;
	GLvoid setIntUniformValue(int vUniformId, GLint v0) const;
	GLvoid setIntUniformValue(int vUniformId, GLint v0, GLint v1) const;
	GLvoid setIntUniformValue(int vUniformId, GLint v0, GLint v1, GLint v2) const;
	GLvoid setIntUniformValue(int vUniformId, GLint v0, GLint v1, GLint v2, GLint v3) const;
	
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0) const;
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1) const;
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2) const;
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
	GLvoid setFloatUniformValue(int vUniformId, GLfloat v0) const;
	GLvoid setFloatUniformValue(int vUniformId, GLfloat v0, GLfloat v1) const;
	GLvoid setFloatUniformValue(int vUniformId, GLfloat v0, GLfloat v1, GLfloat v2) const;
	GLvoid setFloatUniformValue(int vUniformId, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;

	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0) const;
	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1) const;
	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2) const;
	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const;
	GLvoid setDoubleUniformValue(int vUniformId, GLdouble v0) const;
	GLvoid setDoubleUniformValue(int vUniformId, GLdouble v0, GLdouble v1) const;
	GLvoid setDoubleUniformValue(int vUniformId, GLdouble v0, GLdouble v1, GLdouble v2) const;
	GLvoid setDoubleUniformValue(int vUniformId, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const;

	GLvoid setTextureUniformValue(const std::string& vTextureUniformName, const std::shared_ptr<ElayGraphics::STexture>& vioTextureConfig);
	GLvoid setImageUniformValue(const std::shared_ptr<ElayGraphics::STexture>& vTextureConfig);
	GLvoid setMat4UniformValue(const std::string& vUniformName, const GLfloat vMatrix[16]) const;
	GLvoid setMat4UniformValue(int vUniformId, const GLfloat vMatrix[16]) const;

	GLvoid setCommonUniformAndId(const std::string& vUniformName, int vValue = -1);

	//GLvoid changeTextureUniformValue(const std::string& vTextureUniformName, GLint vTextureID, GLint vTextureType = GL_TEXTURE_2D);	//changeTextureUniformValue and setTextureUniformValue merged
	//GLvoid changeImageUniformValue(const ElayGraphics::STexture& vTextureCongfig);

	GLvoid activeShader() const;

	void InquireLocalGroupSize(std::vector<GLint>& voLocalGroupSize) const;

	GLint getShaderProgram() const;

private:
	GLint m_ShaderProgram = 0;
	GLint m_LastBindingIndex = 7;
	std::map<std::string, std::tuple<int, std::shared_ptr<ElayGraphics::STexture>>> m_TextureUniformNameAndBindUnitAndTC;
	std::set<std::shared_ptr<ElayGraphics::STexture>> m_BindingImageTextureSet;
	//std::vector<std::tuple<int, int, int>> m_TextureAndBindingIndexAndTextureTypeSet;
	std::shared_ptr<std::map<std::string, int>> m_pCommonUniformAndId;	//这个数据是为模型类准备的

	GLint		__loadShader(const std::string& vShaderFileName, GLint vShaderType) const;
	GLvoid		__loadShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName = "",
								const std::string& vTessellationControlShaderFileName = "", const std::string& vTessellationEvaluationShaderFileName = "");
	GLvoid		__deleteShader(GLint vShader) const;
	GLboolean	__compileShader(GLint vShader) const;
	GLboolean	__linkProgram() const;
	std::string __loadShaderSourceFromFile(const std::string& vShaderFileName) const;

	GLvoid		__activeAllTextureUniform() const;
};