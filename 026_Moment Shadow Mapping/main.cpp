#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "MSMBufferPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "NonuniformBlurPass.h"
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
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CMSMBufferPass>("MSMBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CNonuniformBlurPass>("NonuniformBlurPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDirectLightPass>("DirectLightPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 4));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CCustomGUI>("CustomGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}