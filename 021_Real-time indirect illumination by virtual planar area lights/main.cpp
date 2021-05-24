#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "ShadingWithRSMAndTilePass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "GaussianBlurPass.h"
#include "LightSourcePass.h"
#include "NormalComputePass.h"
#include "CalculatePlanePass.h"
#include "FitContinuityPlanePass.h"
#include "CalculateWeightPass.h"
#include "LowRSMBufferPass.h"
#include "CalculateEigenPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setSampleNum(4);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightCamera>("LightCamera", 2));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CDynamicObject>("DynamicObject", 3));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRSMBufferPass>("RSMBufferPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLowRSMBufferPass>("LowRSMBufferPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CNormalComputePass>("NormalComputePass", 5));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CCalculateEigenPass>("CalculateEigenPass", 6));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CCalculatePlane>("CalculatePlanePass", 7));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CFitContinuityPlanePass>("FitContinuityPlanePass", 8));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CCalculateWeight>("CCalculateWeight", 9));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("GaussianBlurPass", 10));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDirectLightPass>("DirectLightPass", 12));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingWithRSMAndTilePass>("ShadingWithRSMPass", 13));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 14));
	//ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightSourcePass>("LightSourcePass", 31));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}