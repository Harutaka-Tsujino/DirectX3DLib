#include <windows.h>
#include "../../../GameManager/GameManager.h"
#include "../../SceneManager.h"
#include "HomeScene.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

enum class SceneID;

VOID HomeScene::Update()
{
	static GameManager* pGameManager = GameManager::CopyInstance();
	InputData inputData = pGameManager->GetInputData();

	if (inputData.m_keyBoardState.m_keyPush[DIK_RETURN])
	{
		*m_pNextScene = SceneID::HOME_SCENE;
	}
}

VOID HomeScene::Render()
{
}
