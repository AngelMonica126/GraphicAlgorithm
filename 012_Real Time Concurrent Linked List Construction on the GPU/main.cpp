#include "Interface.h"
#include "Sponza.h"
#include "Dragon.h"
#include "CreateLinkListPass.h"
#include "BlendPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(400, 400);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CDragon>("Dragon", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CreateLinkListPass>("CreateLinkListPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CBlendPass>("BlendPass", 2));


	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}