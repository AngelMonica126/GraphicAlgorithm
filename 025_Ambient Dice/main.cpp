#include "Interface.h"
#include "Sponza.h"
#include "ShadingPass.h"
#include "SkyBoxPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSkyboxPass>("SkyboxPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingPass>("ShadingPass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}