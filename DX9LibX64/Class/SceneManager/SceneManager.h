#pragma once
#include <windows.h>
#include "../BaseWorking/BaseWorking.h"
#include "Scene\Scene.h"

enum class SceneID
{
	VOID_SCENE,

	TITLE_SCENE,

	HOME_SCENE,
};

class Scene;

class SceneManager :public BaseWorking
{
public:
	static SceneManager * GetInstance();

	VOID DeleteInstnce();

	VOID Update();

	VOID Render();

private:
	SceneManager();
	~SceneManager();

	VOID Manage();

	static SceneManager* m_pSceneManager;

	Scene* m_pScene = NULL;
	SceneID m_currentScene = SceneID::VOID_SCENE;
	SceneID m_nextScene = SceneID::TITLE_SCENE;

	GameManager* m_pGameManager;
	CustomVertices* m_pCustomVertices;
	Draw* m_pDraw;

	InputData m_inputData;
};
