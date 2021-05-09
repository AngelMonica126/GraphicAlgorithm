#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "TriangleCutPass.h"
#include "ShadowMapPass.h"
#include "DirectLightPass.h"
#include "IndirectShadingPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setSampleNum(1);
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightCamera>("LightCamera", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadowMapPass>("ShadowMapPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDirectLightPass>("DirectLightPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CTriangleCutPass>("TriangleCutPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CIndirectShadingPass>("IndirectShadingPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 5));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));
	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}