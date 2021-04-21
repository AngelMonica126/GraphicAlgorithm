#include "Interface.h"
#include "CornellBox.h"
#include "ScreenQuadPass.h"
#include "GBufferPass.h"
#include "HBAOPass.h"
#include "HBAOBlurPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1280, 720);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CornellBox>("CornellBox", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGBufferPass>("GBufferPass", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CHBAOPass>("SSAOPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CHBAOBlurPass>("SSAOBlurPass", 3));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 6));



	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}