#pragma once
#include <windows.h>
#include "../Activity/Activity.h"
#include "Scene\Scene.h"

enum class SceneID
{
	VOID_SCENE,

	TITLE_SCENE,

	HOME_SCENE,
};

class Scene;

class SceneManager :public Activity
{
public:
	SceneManager* GetInstance();

	VOID DeleteInstnce();

	VOID Update();

	VOID Render();

private:
	SceneManager() {};
	~SceneManager() {};

	VOID Manage();

	static SceneManager* m_pSceneManager;

	Scene* m_pScene = NULL;

	SceneID m_currentScene = SceneID::VOID_SCENE;

	SceneID m_nextScene = SceneID::TITLE_SCENE;
};
