//////////////////////////////////////////////////////////////////////////
//The resource manager is mainly used to store the global resources that will be shared in multiple pass
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <GL/glew.h>
#include <set>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <boost/any.hpp>
#include <iostream>
#include "common.h"
#include "Singleton.h"
#include "FRAME_EXPORTS.h"
#include "MVector.h"
#include "Tools.h"
class CCamera;
class CUBO4ProjectionWorld;
class CShader;
class CFirstPass;
class CModel;
class CScreenQuadPass;
class CLoadModelPass;
class CLoadModelCullFrontPass;
class IRenderPass;
class IGameObject;
class IGUI;
class CMainGUI;
class CGLFWWindow;

class CResourceManager : public CSingleton<CResourceManager>
{
	friend class CSingleton<CResourceManager>;
public:
	~CResourceManager();

	GLvoid  init();
	GLint														getOrCreateScreenQuadVAO();
	GLint														getOrCreateCubeVAO();
	GLint														getOrCretaeSphereVAO();
	const std::shared_ptr<CModel>&								getOrCreateModel(const std::string &vModelPath);
	const std::shared_ptr<CMainGUI>&							getOrCreateMainGUI();
	const std::shared_ptr<IRenderPass>&							getRenderPassByName(const std::string &vPassName);			//Time consuming much
	const std::shared_ptr<IGameObject>&							getGameObjectByName(const std::string &vGameObjectName);	//Time consuming much
	const std::shared_ptr<IGUI>&								getSubGUIByName(const std::string &vSubGUIByName);			//Time consuming much
	const ElayGraphics::MVector<std::shared_ptr<IRenderPass>>&  getRenderPassSet() const { return m_RenderPassSet; }
	const ElayGraphics::MVector<std::shared_ptr<IGameObject>>&  getGameObjectSet() const { return m_GameObjectSet; }
	const ElayGraphics::MVector<std::shared_ptr<IGUI>>&			getSubGUISet() const { return m_SubGUISet; }
	const boost::any&											getSharedDataByName(const std::string &vDataName);
	boost::any&													fetchSharedDataByName(const std::string& vDataName);

	std::shared_ptr<CCamera>              fecthOrCreateMainCamera();
	std::shared_ptr<CUBO4ProjectionWorld> fetchOrCreateUBO4ProjectionWorld();
	std::shared_ptr<CGLFWWindow>		  fetchOrCreateGLFWWindow();
	std::shared_ptr<CTools>				  fetchOrCreateTools();

	void updateSharedDataByName(const std::string& vDataName, const boost::any& vData);

	GLvoid registerRenderPass(const std::shared_ptr<IRenderPass> &vRenderPass);
	GLvoid registerGameObject(const std::shared_ptr<IGameObject> &vGameObject);
	GLvoid registerSubGUI(const std::shared_ptr<IGUI> &vSubGUI);
	GLvoid registerSharedData(const std::string& vDataName, boost::any vData);

private:
	CResourceManager();

	std::shared_ptr<CGLFWWindow>			m_pGLFWWindow;
	std::shared_ptr<CUBO4ProjectionWorld>   m_pUBO4ProjectionWorld;
	std::shared_ptr<CCamera>                m_pMainCamera;
	std::shared_ptr<CMainGUI>				m_pMainGUI;
	std::shared_ptr<CTools>				    m_pTools;

	GLint m_ScreenQuadVAO = 0;
	GLint m_CubeVAO = 0;
	GLint m_SphereVAO = 0;
	GLint m_Texture4FisrtPass = 0;
	GLint m_Texture4LoadModelCullFrontPass = 0;
	ElayGraphics::MVector<std::shared_ptr<IRenderPass>> m_RenderPassSet;
	ElayGraphics::MVector<std::shared_ptr<IGameObject>> m_GameObjectSet;
	ElayGraphics::MVector<std::shared_ptr<IGUI>>		m_SubGUISet;
	std::map<std::string, std::shared_ptr<CModel>> m_ModelMap;
	std::map<std::string, boost::any> m_SharedDataMap;
};