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
	ElayGraphics::Camera::setMainCameraPos({ -0.413941, 1.49538, 5.88081 });
	ElayGraphics::Camera::setMainCameraFront({ 0.055595, -0.22376, -0.973057 });
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1080, 768);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSource>("LightSource", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CGround>("Ground", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDrawTextureArrayByFBO>("DrawTextureArrayByFBO", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGroundPass>("GroundPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightSourcePass>("LightSourcePass", 3));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CMyGUI>("MyGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}