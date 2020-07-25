#include "Interface.h"
#include "Sponza.h"
#include "LightSource.h"
#include "SponzaGBufferPass.h"
#include "ScreenQuadPass.h"
#include "SplitGBufferPass.h"
#include "ShadingOnSplitedGBufferPass.h"
#include "GatherBufferPass.h"
#include "EgdeDetectionPass.h"
#include "GaussianBlurPass.h"
#include "CustomGUI.h"

int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSources>("LightSources", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSponzaGBufferPass>("SponzaGBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CEdgeDetectionPass>("EdgeDetectionPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSplitGBufferPass>("SplitGBufferPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingOnSplitedGBufferPass>("ShadingOnSplitedGBufferPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGatherBufferPass>("GatherBufferPass", 5));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("GaussianBlurPass", 6));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 7));

#ifdef _DEBUG
	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));
#endif

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}