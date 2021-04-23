#include "Interface.h"
#include "Sponza.h"
#include "DepthPass.h"
#include "RecontructPass.h"

int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1260, 720);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDepthPass>("DepthPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRecontructPass>("RecontructPass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}