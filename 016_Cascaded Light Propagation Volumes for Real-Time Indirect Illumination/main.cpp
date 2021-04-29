#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "AABBDebugPass.h"
#include "LightInjectPass.h"
#include "GeometryInjectPass.h"
#include "PropagationPass.h"
#include "IndirectLightPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setSampleNum(1);
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(true);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightCamera>("LightCamera", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRSMBufferPass>("RSMBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDirectLightPass>("DirectLightPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightInjectPass>("LightInjectPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGeometryInjectPass>("GeometryInjectPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CPropagationPass>("PropagationPass", 5));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CIndirectLightPass>("IndirectLightPass", 6));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 7));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CAABBDebugPass>("AABBDebugPass", 8));


	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}