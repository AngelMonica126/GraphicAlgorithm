#include "Interface.h"
#include "Sponza.h"
#include "LightSource.h"
#include "SponzaGBufferPass.h"
#include "TilePass.h"
#include "ScreenQuadPass.h"

int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSources>("LightSources", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSponzaGBufferPass>("SponzaGBufferPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CTilePass>("TilePass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("SponzaGBufferPass", 3));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}