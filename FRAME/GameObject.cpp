#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "Utils.h"
#include "Common.h"

IGameObject::IGameObject()
{
}

IGameObject::IGameObject(const std::string &vGameObjectName, int vExecutionOrder) : m_Name(vGameObjectName), m_ExecutionOrder(vExecutionOrder)
{
}

IGameObject::~IGameObject()
{
}

//************************************************************************************
//Function:
bool IGameObject::operator<(const IGameObject& vOtherPass) const
{
	return m_ExecutionOrder < vOtherPass.getExecutationOrder();
}

//************************************************************************************
//Function:
const glm::vec3& IGameObject::getPosition() const
{
	return m_Position;
}

//************************************************************************************
//Function:
float IGameObject::getRotationX() const
{
	return m_RotationAngle[0];
}

//************************************************************************************
//Function:
float IGameObject::getRotationY() const
{
	return m_RotationAngle[1];
}

//************************************************************************************
//Function:
float IGameObject::getRotationZ() const
{
	return m_RotationAngle[2];
}

//************************************************************************************
//Function:
const glm::vec3& IGameObject::getScale() const 
{
	return m_Scale;
}

//************************************************************************************
//Function:
void IGameObject::setPosition(const glm::vec3& vPosition)
{
	//m_ModelMatrix = glm::translate(m_ModelMatrix, vPosition - m_Position);
	//m_Position = vPosition;

	/*m_Position = vPosition;
	__updateModelMatrix();*/
	if (vPosition != m_Position)
	{
		m_TranslationMatrix = glm::mat4();
		m_TranslationMatrix = glm::translate(m_TranslationMatrix, vPosition);
		__updateModelMatrix();
		m_Position = vPosition;
	}
}

//************************************************************************************
//Function:
void IGameObject::__setRotation(float vRotationAngleOffset, const glm::vec3& vRotateAxis)
{
	if (!floatEqual(vRotationAngleOffset, 0.0))
	{
		glm::mat4 TempRotationMatrix;
		TempRotationMatrix = glm::rotate(TempRotationMatrix, glm::radians(vRotationAngleOffset), vRotateAxis);
		m_RotationMatrix = TempRotationMatrix * m_RotationMatrix;
		__updateModelMatrix();
		//m_RotationAngle[0] = vRotationAngle;
	}
}

//************************************************************************************
//Function:
void IGameObject::setRotationX(float vRotationAngle)
{
	////if (vRotationAngle[0])
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[0] - m_RotationAngle[0]), glm::vec3(1, 0, 0));
	////if (vRotationAngle[1])
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[1] - m_RotationAngle[1]), glm::vec3(0, 1, 0));
	////if (vRotationAngle[2])
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[2] - m_RotationAngle[2]), glm::vec3(0, 0, 1));
	//m_RotationAngle = vRotationAngle;

	/*m_RotationAngle = vRotationAngle;
	__updateModelMatrix();*/

	__setRotation(vRotationAngle - m_RotationAngle[0], glm::vec3(1, 0, 0));
	m_RotationAngle[0] = vRotationAngle;
}

//************************************************************************************
//Function:
void IGameObject::setRotationY(float vRotationAngle)
{
	__setRotation(vRotationAngle - m_RotationAngle[1], glm::vec3(0, 1, 0));
	m_RotationAngle[1] = vRotationAngle;
}

//************************************************************************************
//Function:
void IGameObject::setRotationZ(float vRotationAngle)
{
	__setRotation(vRotationAngle - m_RotationAngle[2], glm::vec3(0, 0, 1));
	m_RotationAngle[2] = vRotationAngle;
}

//************************************************************************************
//Function:
void IGameObject::setScale(const glm::vec3& vScale)
{
	/*m_ModelMatrix = glm::scale(m_ModelMatrix, vScale / m_Scale);
	m_Scale = vScale;*/
	/*m_Scale = vScale;
	__updateModelMatrix();*/

	if (vScale != m_Scale)
	{
		m_ScaleMatrix = glm::mat4();
		m_ScaleMatrix = glm::scale(m_ScaleMatrix, vScale);
		__updateModelMatrix();
		m_Scale = vScale;
	}
}

//************************************************************************************
//Function:
void IGameObject::translate(const glm::vec3& vPositionOffset)
{
	/*m_ModelMatrix = glm::translate(m_ModelMatrix, vPositionOffset);
	m_Position += vPositionOffset;*/

	/*m_Position += vPositionOffset;
	__updateModelMatrix();*/

	if (vPositionOffset != glm::vec3(0))
	{
		m_TranslationMatrix = glm::translate(m_TranslationMatrix, vPositionOffset);
		__updateModelMatrix();
		m_Position += vPositionOffset;
	}
}

//************************************************************************************
//Function:
void IGameObject::rotateX(float vRotationOffset)
{
	/*if (vRotationOffset[0])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[0]), glm::vec3(1, 0, 0));
	if (vRotationOffset[1])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[1]), glm::vec3(0, 1, 0));
	if (vRotationOffset[2])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[2]), glm::vec3(0, 0, 1));
	m_RotationAngle += vRotationOffset;*/

	__setRotation(vRotationOffset, glm::vec3(1, 0, 0));
	m_RotationAngle[0] += vRotationOffset;
}

//************************************************************************************
//Function:
void IGameObject::rotateY(float vRotationOffset)
{
	__setRotation(vRotationOffset, glm::vec3(0, 1, 0));
	m_RotationAngle[1] += vRotationOffset;
}

//************************************************************************************
//Function:
void IGameObject::rotateZ(float vRotationOffset)
{
	__setRotation(vRotationOffset, glm::vec3(0, 0, 1));
	m_RotationAngle[2] += vRotationOffset;
}

//************************************************************************************
//Function:
void IGameObject::scale(const glm::vec3& vScaleOffset)
{
	/*m_ModelMatrix = glm::scale(m_ModelMatrix, vScaleOffset);
	m_Scale *= vScaleOffset;*/

	/*m_Scale *= vScaleOffset;
	__updateModelMatrix();*/

	if (vScaleOffset != glm::vec3(0))
	{
		m_ScaleMatrix = glm::scale(m_ScaleMatrix, vScaleOffset);
		__updateModelMatrix();
		m_Scale *= vScaleOffset;
	}
}

//************************************************************************************
//Function:
void IGameObject::__updateModelMatrix()
{
	m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
}

//************************************************************************************
//Function:
const glm::mat4& IGameObject::getModelMatrix() const
{
	return m_ModelMatrix;
}

//************************************************************************************
//Function:
int IGameObject::getVAO() const 
{
	return m_VAO;
}

//************************************************************************************
//Function:
void IGameObject::initModel(CShader& vioShader) const
{
	m_pModel->init(vioShader);
}

//************************************************************************************
//Function:
void IGameObject::updateModel(const CShader& vShader) const
{
	m_pModel->update(vShader);
}

//************************************************************************************
//Function:
std::shared_ptr<CAABB> IGameObject::getAABB() const
{
	return m_pModel->getOrCreateBounding();
}