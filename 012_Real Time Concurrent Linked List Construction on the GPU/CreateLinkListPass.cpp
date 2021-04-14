#include "CreateLinkListPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "Dragon.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CreateLinkListPass::CreateLinkListPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CreateLinkListPass::~CreateLinkListPass()
{
}

void CreateLinkListPass::initV()
{
	const int MAX_LIST_NODE = ElayGraphics::WINDOW_KEYWORD::getWindowWidth() * ElayGraphics::WINDOW_KEYWORD::getWindowHeight();
	data = new GLuint[MAX_LIST_NODE];
	memset(data, 0xFF, MAX_LIST_NODE * sizeof(GLuint));
	m_HeadInitSSBO = genBuffer(GL_PIXEL_UNPACK_BUFFER, MAX_LIST_NODE * sizeof(GLuint), data, GL_STATIC_DRAW, -1);
	m_AtomicSSBO = genBuffer(GL_ATOMIC_COUNTER_BUFFER, sizeof(int), { 0 }, GL_DYNAMIC_COPY, 0);
	m_NodeSSBO = genBuffer(GL_SHADER_STORAGE_BUFFER, MAX_LIST_NODE * sizeof(ListNode) * 64, nullptr, GL_STATIC_DRAW, 0);

	m_HeadTexture = std::make_shared<ElayGraphics::STexture>();
	m_HeadTexture->Width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	m_HeadTexture->Height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();
	m_HeadTexture->InternalFormat = GL_R32UI;
	m_HeadTexture->ExternalFormat = GL_RED_INTEGER;
	m_HeadTexture->DataType = GL_UNSIGNED_INT;
	genTexture(m_HeadTexture);

	ElayGraphics::Camera::setMainCameraFarPlane(100);
	ElayGraphics::Camera::setMainCameraMoveSpeed(1.0);
	ElayGraphics::Camera::setMainCameraPos({ 0.0484748, -0.932904, -2.94066 });
	ElayGraphics::Camera::setMainCameraFront({ 0.0811415, -0.153331, 0.984838 });
	m_pShader = std::make_shared<CShader>("CreateLinkList_VS.glsl", "CreateLinkList_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pDragon = std::dynamic_pointer_cast<CDragon>(ElayGraphics::ResourceManager::getGameObjectByName("Dragon"));

	m_pShader->activeShader();
	auto NearPlane = ElayGraphics::Camera::getMainCameraNear();
	auto FarPlane = ElayGraphics::Camera::getMainCameraFar();
	m_pShader->setFloatUniformValue("u_NearPlane", static_cast<float>(NearPlane));
	m_pShader->setFloatUniformValue("u_FarPlane", static_cast<float>(FarPlane));
	m_pShader->setTextureUniformValue("u_ListHeadPtrTex", m_HeadTexture);
	m_pSponza->initModel(*m_pShader);
	m_pDragon->initModel(*m_pShader);

	ElayGraphics::ResourceManager::registerSharedData("HeadTexture", m_HeadTexture);
	ElayGraphics::ResourceManager::registerSharedData("HeadInitSSBO", m_HeadInitSSBO);
	ElayGraphics::ResourceManager::registerSharedData("AtomicSSBO", m_AtomicSSBO);
	ElayGraphics::ResourceManager::registerSharedData("NodeSSBO", m_NodeSSBO);

}

void CreateLinkListPass::clear()
{
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_HeadInitSSBO);
	glBindTexture(GL_TEXTURE_2D, m_HeadTexture->TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight(), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
	glBindImageTexture(0, m_HeadTexture->TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_AtomicSSBO);
	const GLuint zero = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);
}
void CreateLinkListPass::updateV()
{
	clear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_isDiffuseColor", false);
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->updateModel(*m_pShader);
	glm::mat4 model = m_pDragon->getModelMatrix();
	for (int i = 0; i < m_Location.size(); i++)
	{
		glm::mat4 temp = glm::rotate(glm::translate(model, m_Location[i]), 1.57f, glm::vec3(0, 1, 0));
		m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(temp));
		m_pShader->setIntUniformValue("u_isDiffuseColor", true);
		m_pShader->setFloatUniformValue("u_DiffuseColor", m_Color[i].x, m_Color[i].y, m_Color[i].z, m_Color[i].w);
		m_pDragon->updateModel(*m_pShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}