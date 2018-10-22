#include <windows.h>
#include "../../../GameManager/GameManager.h"
#include "HomeScene.h"

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
