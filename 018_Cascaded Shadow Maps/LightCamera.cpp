#include "LightCamera.h"
#include "Interface.h"
#include <glm/gtc/matrix_transform.hpp>

CLightCamera::CLightCamera(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CLightCamera::~CLightCamera()
{
}
void CLightCamera::initFrustum()
{
	m_FrustumNF.resize(m_SplitNum);
	m_Frustum.resize(m_SplitNum);
}


void CLightCamera::setFrustum()
{
	glm::vec3 Center = ElayGraphics::Camera::getMainCameraPos();
	glm::vec3 Direct = ElayGraphics::Camera::getMainCameraDir();
	glm::vec3 Up(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::normalize(glm::cross(Direct, Up));

	float FOVHalf = ElayGraphics::Camera::getMainCameraFov();
	float Ratio = ElayGraphics::WINDOW_KEYWORD::getWindowHeight() / ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	float Near = ElayGraphics::Camera::getMainCameraNear();
	float Far = ElayGraphics::Camera::getMainCameraFar();

	float Lamuda = Far / Near;
	m_FrustumNF[0].Near = Near;
	for (int i = 1; i < m_SplitNum; i++) {
		float Si = i / (float)m_SplitNum;
		float t_Near = m_Weight * (Near * powf(Lamuda, Si)) + (1 - m_Weight) * (Near + (Far - Near) * Si);
		float t_Far = t_Near;
		m_FrustumNF[i].Near = Near;
		m_FrustumNF[i - 1].Far = t_Far;
	}
	m_FrustumNF[m_SplitNum - 1].Far = Far;

	for (int i = 0; i < m_SplitNum; i++) {
		FrustumPos& t_frustum = m_FrustumNF[i];

		glm::vec3 fc = Center + Direct * t_frustum.Far;
		glm::vec3 nc = Center + Direct * t_frustum.Near;

		Right = glm::normalize(Right);
		Up = glm::normalize(glm::cross(Right, Direct));

		float NearHeight = tan(FOVHalf) * t_frustum.Near;
		float NearWidth = NearHeight * Ratio;
		float FarHeight = tan(FOVHalf) * t_frustum.Far;
		float FarWidth = FarHeight * Ratio;

		t_frustum.Pos.push_back(nc - Up * NearHeight - Right * NearWidth);
		t_frustum.Pos.push_back(nc + Up * NearHeight - Right * NearWidth);
		t_frustum.Pos.push_back(nc + Up * NearHeight + Right * NearWidth);
		t_frustum.Pos.push_back(nc - Up * NearHeight + Right * NearWidth);

		t_frustum.Pos.push_back(fc - Up * FarHeight - Right * FarWidth);
		t_frustum.Pos.push_back(fc + Up * FarHeight - Right * FarWidth);
		t_frustum.Pos.push_back(fc + Up * FarHeight + Right * FarWidth);
		t_frustum.Pos.push_back(fc - Up * FarHeight + Right * FarWidth);


		glm::vec3 tmax(-1000.0f, -1000.0f, 0.0f);
		glm::vec3 tmin(1000.0f, 1000.0f, 0.0f);

		glm::vec4 t_transf = m_LightViewMatrix * glm::vec4(t_frustum.Pos[0], 1.0f);

		tmin.z = t_transf.z;
		tmax.z = t_transf.z;
		for (int j = 1; j < 8; j++) {
			t_transf = m_LightViewMatrix * glm::vec4(t_frustum.Pos[j], 1.0f);
			if (t_transf.z > tmax.z) { tmax.z = t_transf.z; }
			if (t_transf.z < tmin.z) { tmin.z = t_transf.z; }
		}
		tmax.z += 50; 

		glm::mat4 t_ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -tmax.z, -tmin.z);
		glm::mat4 t_shad_mvp = t_ortho * m_LightViewMatrix;

		for (int j = 0; j < 8; j++) {
			t_transf = t_shad_mvp * glm::vec4(t_frustum.Pos[j], 1.0f);

			//t_transf.x /= t_transf.w;
			//t_transf.y /= t_transf.w;

			if (t_transf.x > tmax.x) { tmax.x = t_transf.x; }
			if (t_transf.x < tmin.x) { tmin.x = t_transf.x; }
			if (t_transf.y > tmax.y) { tmax.y = t_transf.y; }
			if (t_transf.y < tmin.y) { tmin.y = t_transf.y; }
		}

		glm::vec2 tscale(2.0f / (tmax.x - tmin.x), 2.0f / (tmax.y - tmin.y));
		glm::vec2 toffset(-0.5f * (tmax.x + tmin.x) * tscale.x, -0.5f * (tmax.y + tmin.y) * tscale.y);

		glm::mat4 t_shad_crop;
		t_shad_crop[0][0] = tscale.x;
		t_shad_crop[1][1] = tscale.y;
		t_shad_crop[0][3] = toffset.x;
		t_shad_crop[1][3] = toffset.y;
		//t_shad_crop = glm::transpose(t_shad_crop);
		m_Frustum[i] = t_shad_crop * t_ortho;
	}
}
void CLightCamera::initV()
{
	initFrustum();
	ElayGraphics::Camera::setMainCameraFarPlane(1000);
	ElayGraphics::Camera::setMainCameraPos({ 24.4867, 33.3286, 90.7312 });
	ElayGraphics::Camera::setMainCameraFront({ -0.472339, -0.335287, -0.815155 });
	//ElayGraphics::Camera::setMainCameraPos({ -0.0730926, -1.18774, 2.28317 });
	//ElayGraphics::Camera::setMainCameraFront({ 0.0633111, 0.0078539, -0.997963 });
	ElayGraphics::Camera::setMainCameraMoveSpeed(0.1);
	m_LightPos = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightPos");
	m_LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	if (abs(m_LightDir) == m_LightUpVector)
		m_LightDir.z += 0.01f;
	m_LightViewMatrix = glm::lookAt(glm::vec3(0), m_LightDir, m_LightUpVector);
	ElayGraphics::ResourceManager::registerSharedData("LightViewMatrix", m_LightViewMatrix);
	ElayGraphics::ResourceManager::registerSharedData("Exposure", m_Exposure);
	ElayGraphics::ResourceManager::registerSharedData("SplitNum", m_SplitNum);
	setFrustum();
}

void CLightCamera::updateV()
{

}