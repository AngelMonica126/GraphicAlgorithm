#include <memory>
#include "DrawTextureArrayByFBO.h"
#include "LightSourcePass.h"
#include "GroundPass.h"
#include "LightSource.h"
#include "Ground.h"
#include "Interface.h"
#include "MyGUI.h"

int main()
{
	{
		ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
		ElayGraphics::WINDOW_KEYWORD::setWindowSize(1080, 768);
		/*ElayGraphics::Camera::setMainCameraFarPlane(2000.0);
		ElayGraphics::Camera::setMainCameraMoveSpeed(50.0);*/

		ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSource>("LightSource", 1));
		ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CGround>("Ground", 2));

		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDrawTextureArrayByFBO>("DrawTextureArrayByFBO", 1));
		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGroundPass>("GroundPass", 2));
		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightSourcePass>("LightSourcePass", 3));

		ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CMyGUI>("MyGUI", 1));

		ElayGraphics::App::initApp();
		ElayGraphics::App::updateApp();
	}

	return 0;
}