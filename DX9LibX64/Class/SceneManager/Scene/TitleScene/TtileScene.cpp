#include <Windows.h>
#include "../../../GameManager/GameManager.h"
#include "TitleScene.h"

VOID TitleScene::Update()
{
	static GameManager* pGameManager =GameManager::CopyInstance();
	InputData inputData = pGameManager->GetInputData();

	if (inputData.m_keyBoardState.m_keyPush[DIK_RETURN])
	{
		*m_pNextScene = SceneID::HOME_SCENE;
	}
}

VOID TitleScene::Render()
{
}
