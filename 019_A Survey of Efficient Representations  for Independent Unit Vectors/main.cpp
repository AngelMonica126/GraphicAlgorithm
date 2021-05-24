#include "Interface.h"
#include "EncodePass.h"
#include "DecodePass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1024, 1024);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);


	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<EncodePass>("EncodePass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<DecodePass>("DecodePass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}