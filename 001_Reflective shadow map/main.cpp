#include "Interface.h"
#include "HalfCornelBox.h"
#include "HalfCornellBoxGBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "ShadingWithRSMPass.h"

int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CHalfCornellBox>("HalfCornellBox", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CHalfCornellBoxGBufferPass>("HalfCornellBoxGBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRSMBufferPass>("RSMBufferPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingWithRSMPass>("ShadingWithRSMPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 4));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}