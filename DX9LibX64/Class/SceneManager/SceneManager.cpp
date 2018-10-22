#include <windows.h>
#include "../Activity/Activity.h"
#include "Scene\Scene.h"
#include "Scene\TitleScene\TitleScene.h"
#include "Scene\HomeScene\HomeScene.h"
#include "SceneManager.h"

SceneManager* SceneManager::m_pSceneManager = NULL;

SceneManager* SceneManager::GetInstance()
{
	if (!m_pSceneManager) m_pSceneManager = new SceneManager;

	return m_pSceneManager;
}

VOID SceneManager::DeleteInstnce()
{
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

	m_pScene->Update();
	m_pScene->syncNextScene(&m_nextScene);
}

VOID SceneManager::Render()
{
	m_pScene->Render();
}
