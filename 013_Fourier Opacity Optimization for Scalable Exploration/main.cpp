#include "Interface.h"
#include "Sponza.h"
#include "Dragon.h"
#include "RecontructPass.h"
#include "OpacityShadingPass.h"
#include "FourierCoefficientPass.h"
#include "MergeShadingPass.h"
int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CDragon>("Dragon", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<COpacityShadingPass>("OpacityShadingPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CFourierCoefficientPass>("FourierCoefficientPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CRecontructPass>("RecontructPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CMergeShadingPass>("MergeShadingPass", 4));


	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}