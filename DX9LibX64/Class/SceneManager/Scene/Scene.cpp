#include <windows.h>
#include "../../Activity/Activity.h"
#include "Scene.h"

SceneID* Scene::m_pNextScene = NULL;

VOID Scene::syncNextScene(SceneID* pNextScene)
{
	m_pNextScene = pNextScene;
}
