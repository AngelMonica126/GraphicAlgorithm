#include "Interface.h"
#include "ScreenQuadPass.h"
#include "GBufferPass.h"
#include "Sponza.h"
#include "CascadedShadowMapPass.h"
#include "DirectLightPass.h"
#include "CustomGUI.h"
#include "LightCamera.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1280, 720);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightCamera>("LightCamera", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("GBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CascadedShadowMapPass>("CascadedShadowMapPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDirectLightPass>("DirectLightPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 4));


	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));
	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}