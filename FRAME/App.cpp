#include "App.h"
#include <crtdbg.h>
#include "Utils.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Camera.h"
#include "UBO4ProjectionWorld.h"
#include "RenderPass.h"
#include "GLFWWindow.h"
#include "InputManager.h"
#include "GUI.h"
#include "MainGUI.h"
#include "GameObject.h"

CApp::CApp()
{
}

CApp::~CApp()
{
}

//************************************************************************************
//Function:
GLvoid CApp::init()
{
	CResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->init();
	m_pWindow = CResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->fetchWindow();
	m_pResourceManager = CResourceManager::getOrCreateInstance();
	m_pResourceManager->init();
	m_pResourceManager->getOrCreateMainGUI()->init();
	m_pResourceManager->fecthOrCreateMainCamera()->init();
	CInputManager::getOrCreateInstance()->init();
	m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->init();

	for (auto &vItem : m_pResourceManager->getSubGUISet())
	{
		vItem->initV();
	}

	for (auto &vItem : m_pResourceManager->getGameObjectSet())
	{
		vItem->initV();
	}

	for (auto &vItem : m_pResourceManager->getRenderPassSet())
	{
		vItem->initV();
	}
}

//************************************************************************************
//Function:
GLvoid CApp::update()
{
	_ASSERT(m_pWindow);
	while (!glfwWindowShouldClose(m_pWindow))
	{
		__calculateTime();
		glfwPollEvents();
		m_pResourceManager->fecthOrCreateMainCamera()->update();
		m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->update();

		for (auto &vItem : m_pResourceManager->getGameObjectSet())
		{
			vItem->updateV();
		}

		glClear(GL_COLOR_BUFFER_BIT);

		ElayGraphics::MVector<std::shared_ptr<IRenderPass>> renderPassSets = m_pResourceManager->getRenderPassSet();
		for (int i = 0; i < renderPassSets.size(); i++)
		{
			if (renderPassSets[i]->getExecutionOrder() == -1) continue;
			switch (renderPassSets[i]->getPassType())
			{
			case ElayGraphics::ERenderPassType::RenderPassType_Once:
			{
				renderPassSets[i]->updateV();
				renderPassSets[i]->finishExecute();
				break;
			}
			case ElayGraphics::ERenderPassType::RenderPassType_Parallel:
			{
				int j = i;
				while (j < renderPassSets.size())
				{
					if (renderPassSets[i]->getExecutionOrder() != renderPassSets[j]->getExecutionOrder())break;
					renderPassSets[j++]->updateV();
				}
				i = j - 1;
				break;
			}
			case ElayGraphics::ERenderPassType::RenderPassType_ParallelOnce:
			{
				int j = i;
				while (j < renderPassSets.size())
				{
					if (renderPassSets[i]->getExecutionOrder() != renderPassSets[j]->getExecutionOrder())break;

					renderPassSets[j++]->updateV();
				}
				while (i < j)
				{
					renderPassSets[i++]->finishExecute();
				}
				i--;
				break;

			}
			case ElayGraphics::ERenderPassType::RenderPassType_Delay:
				renderPassSets[i]->setPassType(ElayGraphics::ERenderPassType::RenderPassType_Normal);
				break;
			default:
				renderPassSets[i]->updateV();
				break;
			}
		}

		if (ElayGraphics::COMPONENT_CONFIG::IS_ENABLE_GUI)
		{
			m_pResourceManager->getOrCreateMainGUI()->update();
			for (auto &vItem : m_pResourceManager->getSubGUISet())
			{
				vItem->updateV();
			}
			m_pResourceManager->getOrCreateMainGUI()->lateUpdate();
		}

		glfwSwapBuffers(m_pWindow);
	}
}

//************************************************************************************
//Function:
GLvoid CApp::__calculateTime()
{
	m_CurrentTime = glfwGetTime();
	m_DeltaTime = m_CurrentTime - m_LastFrameTime;
	m_LastFrameTime = m_CurrentTime;

	++m_FrameCounter;
	m_TimeCounter += m_DeltaTime;
	if (m_TimeCounter >= 1.0)
	{
		m_TimeCounter = 0.0;
		m_FramesPerSecond = m_FrameCounter;
		m_FrameCounter = 0;
	}
}

//************************************************************************************
//Function:
GLdouble CApp::getDeltaTime() const
{
	return m_DeltaTime;
}

//************************************************************************************
//Function:
GLdouble CApp::getFrameRateInMilliSecond() const
{
	return m_DeltaTime * 1000;
}

//************************************************************************************
//Function:
GLdouble CApp::getCurrentTime() const
{
	return m_CurrentTime;
}

//************************************************************************************
//Function:
GLuint CApp::getFramesPerSecond() const
{
	return m_FramesPerSecond;
}