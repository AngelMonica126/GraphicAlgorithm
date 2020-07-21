#include <memory>
#include "GroundPass.h"
#include "Ground.h"
#include "Interface.h"
#include "TilePakagePass.h"
int main()
{
	{
		ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
		ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
		ElayGraphics::WINDOW_KEYWORD::setWindowSize(1080, 800);
		ElayGraphics::Camera::setMainCameraPos(glm::vec3(-0.79372, 4.02423, 13.3243));
		ElayGraphics::Camera::setMainCameraFront(glm::vec3(0.0708797, -0.280499, -1.0));

		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CTilePakagePass>("TilePakagePass", 1));
		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGroundPass>("GroundPass", 2));

		ElayGraphics::App::initApp();
		ElayGraphics::App::updateApp();
	}

	return 0;
}