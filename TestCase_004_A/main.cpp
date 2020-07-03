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
#include "LowResolutionPass.h"
#include "ShadingLowResolutionPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(true);
	
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSponzaGBufferPass>("SponzaGBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLowResolutionPass>("LowResolutionPass", 2, ElayGraphics::ERenderPassType::RenderPassType_Once));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CEdgeDetectionPass>("EdgeDetectionPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSplitGBufferPass>("SplitGBufferPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingLowResolutionPass>("ShadingLowResolutionPass", 5, ElayGraphics::ERenderPassType::RenderPassType_Once));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingOnSplitedGBufferPass>("ShadingOnSplitedGBufferPass", 7));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGatherBufferPass>("GatherBufferPass",8));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("GaussianBlurPass", 9));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 10));

	ElayGraphics::Camera::setMainCameraFront(glm::vec3(0, -0.575005, -0.818128));
	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}