#include "Interface.h"
#include "Sponza.h"
#include "SponzaGBufferPass.h"
#include "ScreenQuadPass.h"
#include "NonuniformBlurPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1024, 1024);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSponzaGBufferPass>("SponzaGBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CNonuniformBlurPass>("NonuniformBlurPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 7));


	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}