#include <windows.h>
#include "../BaseWorking/BaseWorking.h"
#include "Scene\Scene.h"
#include "Scene\TitleScene\TitleScene.h"
#include "Scene\HomeScene\HomeScene.h"
#include "SceneManager.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

SceneManager* SceneManager::m_pSceneManager = NULL;

SceneManager::SceneManager()
{
	m_pGameManager = GameManager::CopyInstance();
	m_inputData = m_pGameManager->GetInputData();
	m_pCustomVertices  = CustomVertices::GetInstance();
	m_pDraw = Draw::GetInstance();
}

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::GetInstance()
{
	if (!m_pSceneManager) m_pSceneManager = new SceneManager;

	return m_pSceneManager;
}

VOID SceneManager::DeleteInstnce()
{
	m_pCustomVertices->DeleteInstance();
	m_pDraw->DeleteInstance();
	delete m_pScene;
	delete m_pSceneManager;
	m_pSceneManager = NULL;
}

VOID SceneManager::Manage()
{
	if (m_currentScene == m_nextScene)
	{
		return;
	}

	switch (m_nextScene)
	{
	case SceneID::TITLE_SCENE:

		delete m_pScene;

		m_currentScene = SceneID::TITLE_SCENE;
		m_pScene = new TitleScene();

		break;
	case SceneID::HOME_SCENE:

		delete m_pScene;

		m_currentScene = SceneID::HOME_SCENE;
		m_pScene = new HomeScene();

		break;
	default:

		break;
	}
}

VOID SceneManager::Update()
{
	Manage();

	m_inputData = m_pGameManager->GetInputData();
	m_pScene->syncLibInstance(m_pGameManager, m_pCustomVertices, m_pDraw, m_inputData);
	m_pScene->Update();
	m_pScene->syncNextScene(&m_nextScene);
}

VOID SceneManager::Render()
{
	m_pScene->Render();
}
