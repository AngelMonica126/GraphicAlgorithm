#include "Interface.h"
#include "ScreenQuadPass.h"
#include "RCASPass.h"
#include "EASUPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	ElayGraphics::WINDOW_KEYWORD::setSampleNum(4);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CEASUPass>("EASUPass", 0));
	//ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRCASPass>("RCASPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}