#include "Shader.h"
#include "Common.h"
#include <crtdbg.h>
#include <fstream>
#include <sstream>
#include <iostream>

CShader::CShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName,
	const std::string& vTessellationControlShaderFileName, const std::string& vTessellationEvaluationShaderFileName)
{
	__loadShader(vVertexShaderFileName, vFragmentShaderFileName, vGeometryShaderFileName, vTessellationControlShaderFileName, vTessellationEvaluationShaderFileName);
}

CShader::CShader(const std::string& vComputeShaderFileName)
{
	_ASSERT(!vComputeShaderFileName.empty());
	if (m_ShaderProgram)
		return;
	m_ShaderProgram = glCreateProgram();
	GLint ComputeShader = __loadShader(vComputeShaderFileName, GL_COMPUTE_SHADER);
	__linkProgram();
	__deleteShader(ComputeShader);
}

CShader::~CShader()
{
	if (m_ShaderProgram)
		glDeleteProgram(m_ShaderProgram);
}

//************************************************************************************
//Function:
GLvoid CShader::__loadShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName,
	const std::string& vTessellationControlShaderFileName, const std::string& vTessellationEvaluationShaderFileName)
{
	_ASSERT(!vVertexShaderFileName.empty() && !vFragmentShaderFileName.empty());
	if (m_ShaderProgram)
		return;
	m_ShaderProgram = glCreateProgram();

	GLint VertexShader   = __loadShader(vVertexShaderFileName, GL_VERTEX_SHADER);
	GLint FragmentShader = __loadShader(vFragmentShaderFileName, GL_FRAGMENT_SHADER);

	GLint GeometryShader = 0, TessellationControlShader = 0, TessellationEvaluationShader = 0;
	if (!vGeometryShaderFileName.empty())
		GeometryShader = __loadShader(vGeometryShaderFileName, GL_GEOMETRY_SHADER);
	if (!vTessellationControlShaderFileName.empty())
		TessellationControlShader = __loadShader(vTessellationControlShaderFileName, GL_TESS_CONTROL_SHADER);
	if (!vTessellationEvaluationShaderFileName.empty())
		TessellationEvaluationShader = __loadShader(vTessellationEvaluationShaderFileName, GL_TESS_EVALUATION_SHADER);

	__linkProgram();

	__deleteShader(VertexShader);
	__deleteShader(FragmentShader);
	__deleteShader(GeometryShader);
	__deleteShader(TessellationControlShader);
	__deleteShader(TessellationEvaluationShader);
}

//************************************************************************************
//Function:
GLint CShader::__loadShader(const std::string& vShaderFileName, GLint vShaderType) const
{
	_ASSERT(!vShaderFileName.empty() && vShaderType);
	GLint Shader = glCreateShader(vShaderType);
	_ASSERT(Shader);
	const std::string ShaderSource = __loadShaderSourceFromFile(vShaderFileName);
	const GLchar *pShaderSource = ShaderSource.c_str();
	_ASSERT(pShaderSource);
	glShaderSource(Shader, 1, &pShaderSource, nullptr);
	_ASSERT(__compileShader(Shader));
	_ASSERT(m_ShaderProgram);
	glAttachShader(m_ShaderProgram, Shader);
	return Shader;
}

//************************************************************************************
//Function:
std::string CShader::__loadShaderSourceFromFile(const std::string& vShaderFileName) const
{
	try
	{
		std::ifstream Fin(vShaderFileName);
		_ASSERT(Fin);
		std::stringstream FinStream;
		FinStream << Fin.rdbuf();
		return FinStream.str();
	}
	catch(std::exception e)
	{
		std::cerr << "Error::Shader:: Read Shader File Failure: " << vShaderFileName << std::endl;
		return std::string();
	}
}

//************************************************************************************
//Function:
GLboolean CShader::__compileShader(GLint vShader) const
{
	_ASSERT(vShader);
	GLint Success = 0;
	const GLint MaxInfoLogLength = 512;
	GLchar InfoLog[MaxInfoLogLength] = {};
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(vShader, MaxInfoLogLength, nullptr, InfoLog);
		std::cerr << "Error::Shader:: Shader Compile Failure: " << InfoLog << std::endl;
		return GL_FALSE;
	}
	return GL_TRUE;
}

//************************************************************************************
//Function:
GLboolean CShader::__linkProgram() const
{
	_ASSERT(m_ShaderProgram);
	GLint Success = 0;
	const GLint MaxInfoLogLength = 512;
	GLchar InfoLog[MaxInfoLogLength] = {};
	glLinkProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_ShaderProgram, MaxInfoLogLength, nullptr, InfoLog);
		std::cerr << "Error::Shader:: Shader Program Link Failure: " << InfoLog << std::endl;
		return GL_FALSE;
	}
	return GL_TRUE;
}

//************************************************************************************
//Function:
GLvoid CShader::__deleteShader(GLint vShader) const
{
	if (vShader)
		glDeleteShader(vShader);
}

//************************************************************************************
//Function:
GLint CShader::getUniformId(const std::string& vUniformName) const
{
	return glGetUniformLocation(m_ShaderProgram, vUniformName.c_str());
}

//************************************************************************************
//Function:
GLint CShader::getCommonUniformId(const std::string& vUniformName) const
{
	if (m_pCommonUniformAndId)
	{
		_ASSERT(m_pCommonUniformAndId->find(vUniformName) != m_pCommonUniformAndId->end());
		return (*m_pCommonUniformAndId)[vUniformName];
	}
	else
		return -1;
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0) const 
{
	glUniform1i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1) const
{
	glUniform2i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2) const
{
	glUniform3i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2, GLint v3) const
{
	glUniform4i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(int vUniformId, GLint v0) const
{
	glUniform1i(vUniformId, v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(int vUniformId, GLint v0, GLint v1) const
{
	glUniform2i(vUniformId, v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(int vUniformId, GLint v0, GLint v1, GLint v2) const
{
	glUniform3i(vUniformId, v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(int vUniformId, GLint v0, GLint v1, GLint v2, GLint v3) const
{
	glUniform4i(vUniformId, v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0) const
{
	glUniform1f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1) const
{
	glUniform2f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2) const
{
	glUniform3f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
	glUniform4f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(int vUniformId, GLfloat v0) const
{
	glUniform1f(vUniformId, v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(int vUniformId, GLfloat v0, GLfloat v1) const
{
	glUniform2f(vUniformId, v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(int vUniformId, GLfloat v0, GLfloat v1, GLfloat v2) const
{
	glUniform3f(vUniformId, v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(int vUniformId, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
	glUniform4f(vUniformId, v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0) const
{
	glUniform1d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1) const
{
	glUniform2d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2) const
{
	glUniform3d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const
{
	glUniform4d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(int vUniformId, GLdouble v0) const
{
	glUniform1d(vUniformId, v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(int vUniformId, GLdouble v0, GLdouble v1) const
{
	glUniform2d(vUniformId, v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(int vUniformId, GLdouble v0, GLdouble v1, GLdouble v2) const
{
	glUniform3d(vUniformId, v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(int vUniformId, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const
{
	glUniform4d(vUniformId, v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setMat4UniformValue(const std::string& vUniformName, const GLfloat vMatrix[16]) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, GL_FALSE, vMatrix);
}

//************************************************************************************
//Function:
GLvoid CShader::setMat4UniformValue(int vUniformId, const GLfloat vMatrix[16]) const
{
	glUniformMatrix4fv(vUniformId, 1, GL_FALSE, vMatrix);
}

//************************************************************************************
//Function:
GLvoid CShader::setTextureUniformValue(const std::string& vTextureUniformName, const std::shared_ptr<ElayGraphics::STexture>& vioTextureConfig)
{
	if (m_TextureUniformNameAndBindUnitAndTC.find(vTextureUniformName) != m_TextureUniformNameAndBindUnitAndTC.end())
	{                
		//_WARNING(m_TextureNameAndTextureIDAndBindingIndexAndTextureTypeSet.find(vTextureUniformName) == m_TextureNameAndTextureIDAndBindingIndexAndTextureTypeSet.end(), "Texture Uniform not be binded.");
		auto &Item = m_TextureUniformNameAndBindUnitAndTC[vTextureUniformName];
		std::get<1>(Item) = vioTextureConfig;
		glActiveTexture(GL_TEXTURE0 + std::get<0>(Item));
		glBindTexture(static_cast<int>(vioTextureConfig->TextureType), vioTextureConfig->TextureID);
	}
	else
	{
		int BindingIndex = ++m_LastBindingIndex;
		glActiveTexture(GL_TEXTURE0 + BindingIndex);
		glUniform1i(glGetUniformLocation(m_ShaderProgram, vTextureUniformName.c_str()), BindingIndex);
		glBindTexture(static_cast<int>(vioTextureConfig->TextureType), vioTextureConfig->TextureID);
		m_TextureUniformNameAndBindUnitAndTC[vTextureUniformName] = std::make_tuple(BindingIndex, vioTextureConfig);
	}
}

//************************************************************************************
//Function:
GLvoid CShader::setImageUniformValue(const std::shared_ptr<ElayGraphics::STexture>& vTextureConfig)
{
	if (vTextureConfig->ImageBindUnit != -1)
	{
		if (m_BindingImageTextureSet.find(vTextureConfig) == m_BindingImageTextureSet.end())
		{
			glBindImageTexture(vTextureConfig->ImageBindUnit, vTextureConfig->TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, vTextureConfig->InternalFormat);
			m_BindingImageTextureSet.insert(vTextureConfig);
		}
		else
		{
			glBindImageTexture(vTextureConfig->ImageBindUnit, vTextureConfig->TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, vTextureConfig->InternalFormat);
		}
	}
}

//************************************************************************************
//Function:
GLvoid CShader::setCommonUniformAndId(const std::string& vUniformName, int vValue)
{
	if (!m_pCommonUniformAndId)
		m_pCommonUniformAndId = std::make_shared<std::map<std::string, int>>();
	int Id = getUniformId(vUniformName);
	(*m_pCommonUniformAndId)[vUniformName] = Id;
	if (Id > 0 && vValue > -1)
		setIntUniformValue(Id, vValue);
}

//************************************************************************************
//Function:
//GLvoid CShader::changeTextureUniformValue(const std::string& vTextureUniformName, GLint vTextureID, GLint vTextureType/* = GL_TEXTURE_2D*/)
//{
//	_WARNING(m_TextureNameAndTextureIDAndBindingIndexAndTextureTypeSet.find(vTextureUniformName) == m_TextureNameAndTextureIDAndBindingIndexAndTextureTypeSet.end(), "Texture Uniform not be binded.");
//	auto &Item = m_TextureNameAndTextureIDAndBindingIndexAndTextureTypeSet[vTextureUniformName];
//	std::get<0>(Item) = vTextureID;
//	std::get<2>(Item) = vTextureType;
//	glActiveTexture(GL_TEXTURE0 + std::get<1>(Item));
//	glBindTexture(vTextureType, vTextureID);
//}

//************************************************************************************
//Function:
GLvoid CShader::activeShader() const
{
	glUseProgram(m_ShaderProgram);
	__activeAllTextureUniform();
}

//************************************************************************************
//Function:
GLvoid CShader::__activeAllTextureUniform() const
{
	/*for (const auto& Item : m_TextureAndBindingIndexAndTextureTypeSet)
	{
		glActiveTexture(GL_TEXTURE0 + std::get<1>(Item));
		glBindTexture(std::get<2>(Item), std::get<0>(Item));
	}*/
	for (const auto& Item : m_TextureUniformNameAndBindUnitAndTC)
	{
		glActiveTexture(GL_TEXTURE0 + std::get<0>(Item.second));
		const auto& TextureConfig = std::get<1>(Item.second);
		glBindTexture(static_cast<int>(TextureConfig->TextureType), TextureConfig->TextureID);
	}

	for (const auto& Item : m_BindingImageTextureSet)
	{
		glBindImageTexture(Item->ImageBindUnit, Item->TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, Item->InternalFormat);
	}
}

//************************************************************************************
//Function:
void CShader::InquireLocalGroupSize(std::vector<GLint>& voLocalGroupSize) const
{
	voLocalGroupSize.resize(3);
	glGetProgramiv(m_ShaderProgram, GL_COMPUTE_WORK_GROUP_SIZE, voLocalGroupSize.data());
}