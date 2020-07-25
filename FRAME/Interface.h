#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <boost/any.hpp>
#include "FRAME_EXPORTS.h"

class IGameObject;
class IRenderPass;
class IGUI;
class CModel;
class CMainGUI;
class CCamera;

namespace ElayGraphics
{
	namespace App
	{
		FRAME_DLLEXPORTS void   initApp();
		FRAME_DLLEXPORTS void   updateApp();
		FRAME_DLLEXPORTS int    getFramesPerSecond();
		FRAME_DLLEXPORTS double getDeltaTime();
		FRAME_DLLEXPORTS double getCurrentTime();
		FRAME_DLLEXPORTS double getFrameRateInMilliSecond();
	}

	namespace WINDOW_KEYWORD
	{
		FRAME_DLLEXPORTS int  getWindowWidth();
		FRAME_DLLEXPORTS int  getWindowHeight();
		FRAME_DLLEXPORTS void setWindowSize(int vWidth, int vHeight, bool vIsViewportSizeChangedWithWindow = true);
		FRAME_DLLEXPORTS void setViewportSize(int vWidth, int vHeight);
		FRAME_DLLEXPORTS void setSampleNum(int vSampleNum);
		FRAME_DLLEXPORTS void setIsCursorDisable(bool vIsCursorDisable);
		FRAME_DLLEXPORTS void setWindowTile(const std::string& vWindowTitle);
	}

	namespace COMPONENT_CONFIG
	{
		FRAME_DLLEXPORTS void setIsEnableGUI(bool vIsEnableGUI);
	}

	namespace Camera
	{
		FRAME_DLLEXPORTS double		getMainCameraFov();
		FRAME_DLLEXPORTS double		getMainCameraNear();
		FRAME_DLLEXPORTS double		getMainCameraFar();
		FRAME_DLLEXPORTS glm::dvec3 getMainCameraLookAtPos();
		FRAME_DLLEXPORTS glm::dvec3 getMainCameraDir();
		FRAME_DLLEXPORTS const glm::dvec3& getMainCameraPos();
		FRAME_DLLEXPORTS const glm::dvec3& getMainCameraUp();
		FRAME_DLLEXPORTS glm::mat4  getMainCameraProjectionMatrix();
		FRAME_DLLEXPORTS glm::mat4  getMainCameraViewMatrix();
		FRAME_DLLEXPORTS glm::mat4  getMainCameraPrevViewMatrix();
		FRAME_DLLEXPORTS void setMainCameraPos(glm::dvec3 vCameraPos);
		FRAME_DLLEXPORTS void setMainCameraFront(glm::dvec3 vCameraFront);
		FRAME_DLLEXPORTS void setMainCameraFarPlane(double vFarPlane);
		FRAME_DLLEXPORTS void setMainCameraMoveSpeed(double vMoveSpeed);
		FRAME_DLLEXPORTS void setMainCameraFov(double vFov);
		FRAME_DLLEXPORTS void setIsEnableCursor(bool vIsEnableCursor);
	}

	namespace ResourceManager
	{
		FRAME_DLLEXPORTS int								 getOrCreateScreenQuadVAO();
		FRAME_DLLEXPORTS int								 getOrCreateCubeVAO();
		FRAME_DLLEXPORTS const std::shared_ptr<CMainGUI>&	 getOrCreateMainGUI();
		FRAME_DLLEXPORTS const std::shared_ptr<IGameObject>& getGameObjectByName(const std::string &vGameObjectName);
		FRAME_DLLEXPORTS const std::shared_ptr<CModel>&		 getOrCreateModel(const std::string &vModelPath);
		FRAME_DLLEXPORTS const boost::any&					 getSharedDataByName(const std::string &vDataName);

		FRAME_DLLEXPORTS void registerSharedData(const std::string& vDataName, boost::any vData);
		FRAME_DLLEXPORTS void registerRenderPass(const std::shared_ptr<IRenderPass> &vRenderPass);
		FRAME_DLLEXPORTS void registerGameObject(const std::shared_ptr<IGameObject> &vGameObject);
		FRAME_DLLEXPORTS void registerSubGUI(const std::shared_ptr<IGUI> &vSubGUI);

		FRAME_DLLEXPORTS void updateSharedDataByName(const std::string& vDataName, const boost::any& vData);

		template <typename TDataType>
		TDataType getSharedDataByName(const std::string &vDataName)		//no return const &, because any_cast will allocate one temporary variable
		{
			return boost::any_cast<TDataType>(getSharedDataByName(vDataName));
		}

		template <typename TDataType>
		TDataType getSharedDataByName(const std::string &vDataName, const boost::any& vDataValueWhenLastGet, bool &vioDataChanged)
		{
			auto Data = boost::any_cast<TDataType>(getSharedDataByName(vDataName));
			auto LastData = boost::any_cast<TDataType>(vDataValueWhenLastGet);
			vioDataChanged = !(LastData == Data);
			return Data;
		}
	}

	namespace InputManager
	{
		FRAME_DLLEXPORTS int getKeyStatus(int vKey);
	}
	namespace Tools
	{
		FRAME_DLLEXPORTS float getRandom01();
		FRAME_DLLEXPORTS int   getRandomIntRange(int min, int max);
	}
}