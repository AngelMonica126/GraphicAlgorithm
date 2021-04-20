#include "Interface.h"
#include "CornellBox.h"
#include "ScreenQuadPass.h"
#include "GBufferPass.h"
#include "SSAOPass.h"
#include "SSAOBlurPass.h"
#include "SSDOPass.h"
#include "SSDOBlurPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CornellBox>("CornellBox", 1));

	
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGBufferPass>("GBufferPass", 1));
	//SSAO
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSSAOPass>("SSAOPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSSAOBlurPass>("SSAOBlurPass", 3));

	//SSDO Indirect Light
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSSDOPass>("SSDOPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSSDOBlurPass>("SSDOBlurPass", 5));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 6));



	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}