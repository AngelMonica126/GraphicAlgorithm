#include "LightSource.h"
#include "Interface.h"

CLightSource::CLightSource(const std::string &vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

//************************************************************************************
//Function:
void CLightSource::initV()
{
	setVAO(ElayGraphics::ResourceManager::getOrCreateScreenQuadVAO());
	translate(glm::vec3(0, 1 , 0));
	//scale(glm::vec3(1, sqrt(2) / 2.0, 1));
	//setRotationX(45);
	m_PolygonalLightVertexOriginalPosSet =
	{
		glm::vec3(1, 1, 0),
		glm::vec3(1, -1, 0),
		glm::vec3(-1, -1, 0),
		glm::vec3(-1, 1, 0)
	};
	m_PolygonalLightVertexPosSet.resize(m_PolygonalLightVertexOriginalPosSet.size());
	for (int i = 0; i < m_PolygonalLightVertexPosSet.size(); ++i)
	{
		m_PolygonalLightVertexPosSet[i] = glm::vec3(getModelMatrix() * glm::vec4(m_PolygonalLightVertexOriginalPosSet[i], 1.0));
	}
	//translate(glm::vec3(0, 0.5-0.25, -0.5));
	//setRotationX(0);
	//scale(glm::vec3(1, 1, 1));
}

//************************************************************************************
//Function:
void CLightSource::updateV()
{
	//glm::vec3 RotationFromGUI = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Rotation");
	//if (RotationFromGUI != m_Rotation)
	//{
	//	m_Rotation = RotationFromGUI;
	//	setRotationX(m_Rotation.x);
	//	setRotationY(m_Rotation.y);
	//	setRotationZ(m_Rotation.z);
	//	for (int i = 0; i < m_PolygonalLightVertexPosSet.size(); ++i)
	//	{
	//		m_PolygonalLightVertexPosSet[i] = glm::vec3(getModelMatrix() * glm::vec4(m_PolygonalLightVertexOriginalPosSet[i], 1.0));
	//	}
	//}
}