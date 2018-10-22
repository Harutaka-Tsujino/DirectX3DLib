#pragma once
#include <windows.h>
#include "../../Activity/Activity.h"
#include "../SceneManager.h"

class Scene :public Activity
{
public:
	Scene() {};
	~Scene() {};

	VOID syncNextScene(SceneID* pNextScene);

protected:
	static SceneID* m_pNextScene;
};
