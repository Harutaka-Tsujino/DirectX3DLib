#include <windows.h>
#include "../../BaseWorking/BaseWorking.h"
#include "../../GameManager/GameManager.h"
#include "../../FBX/ClassFBX.h"
#include "../../CustomVertices/CustomVertices.h"
#include "../../Draw/Draw.h"
#include "Scene.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

SceneID* Scene::m_pNextScene = NULL;

VOID Scene::syncNextScene(SceneID* pNextScene)
{
	m_pNextScene = pNextScene;
}

VOID Scene::syncLibInstance(GameManager* pGameManager, CustomVertices* pCustomVertices, Draw* pDraw, InputData inputData)
{
	m_pGameManager = pGameManager;
	m_pCustomVertices = m_pCustomVertices;
	m_pDraw = pDraw;
	m_inputData = inputData;
}
