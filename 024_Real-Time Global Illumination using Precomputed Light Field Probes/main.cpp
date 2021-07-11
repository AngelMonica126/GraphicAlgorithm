#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "AABBDebugPass.h"
#include "BakeProbePass.h"
#include "OctahedronPass.h"
#include "DynamicObject.h"
#include "DynamicObjectPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setSampleNum(1);
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(true);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightCamera>("LightCamera", 2));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CDynamicObject>("DynamicObject", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRSMBufferPass>("RSMBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDirectLightPass>("DirectLightPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CBakeProbePass>("BakeProbePass", 3, ElayGraphics::ERenderPassType::RenderPassType_Once));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<COctahedronPass>("OctahedronPass", 6, ElayGraphics::ERenderPassType::RenderPassType_Once));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 7));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDynamicObjectPass>("DynamicObjectPass", 8));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CAABBDebugPass>("AABBDebugPass", 9));


	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}