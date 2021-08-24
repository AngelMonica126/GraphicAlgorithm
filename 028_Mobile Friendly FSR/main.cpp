#include "Interface.h"
#include "ScreenQuadPass.h"
#include "RCASPass.h"
#include "EASUPass.h"
#include "MobileFriendlyEASUPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
	ElayGraphics::WINDOW_KEYWORD::setWindowTile("O:switch to Normal Texture;  P:switch to FSR Texture");
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CEASUPass>("EASUPass", 0));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRCASPass>("RCASPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CMobileFriendlyEASUPass>("MobileFriendlyEASUPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 3));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}