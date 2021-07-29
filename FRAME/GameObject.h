//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <string>
#include <GLM/glm.hpp>
#include <memory>
#include <vector>
#include "FRAME_EXPORTS.h"

class CModel;
class CShader;
class CAABB;

class FRAME_DLLEXPORTS IGameObject
{
public:
	IGameObject();
	IGameObject(const std::string &vGameObjectName, int vExecutionOrder);
	virtual ~IGameObject();

	virtual void initV() = 0;
	virtual void updateV() = 0;

	void setPosition(const glm::vec3& vPosition);
	void setRotationX(float vRotationAngle);
	void setRotationY(float vRotationAngle);
	void setRotationZ(float vRotationAngle);
	void setScale(const glm::vec3& vScale);
	void setExecutionOrder(int vExecutionOrder) { m_ExecutionOrder = vExecutionOrder; }
	void setGameObjectName(const std::string &vName) { m_Name = vName; };
	void setVAO(int vVAO) { m_VAO = vVAO; }
	void setModel(const std::shared_ptr<CModel> &vModel) { m_pModel = vModel; }

	int getVAO() const;
	int getExecutationOrder() const { return m_ExecutionOrder; }
	float			   getRotationX() const;
	float			   getRotationY() const;
	float			   getRotationZ() const;
	const glm::vec3&   getPosition() const;
	const glm::vec3&   getScale() const;
	const glm::mat4&   getModelMatrix() const;
	const std::string& getGameObjectName() const { return m_Name; }
	const std::shared_ptr<CModel>& getModel() { return m_pModel; }


	void translate(const glm::vec3& vPositionOffset);
	void rotateX(float vRotationOffset);
	void rotateY(float vRotationOffset);
	void rotateZ(float vRotationOffset);
	void scale(const glm::vec3& vScaleOffset);

	void initModel(CShader& vioShader) const;
	void updateModel(const CShader& vShader) const;

	std::shared_ptr<CAABB> getAABB() const;
	std::vector<glm::vec3> getTriangle();
	bool operator<(const IGameObject& vOtherPass) const;

private:
	int m_VAO = -1;
	int m_ExecutionOrder = -1;
	std::string m_Name;
	glm::vec3   m_Position;
	glm::vec3	m_RotationAngle;	//ÈÆxyzÖáµÄÐý×ª½Ç¶È
	glm::vec3	m_Scale = glm::vec3(1, 1, 1);
	glm::mat4	m_ModelMatrix;
	glm::mat4   m_TranslationMatrix;
	glm::mat4   m_RotationMatrix;
	glm::mat4   m_ScaleMatrix;
	std::shared_ptr<CModel> m_pModel;

	void __updateModelMatrix();
	void __setRotation(float vRotationAngleOffset, const glm::vec3& vRotateAxis);
};