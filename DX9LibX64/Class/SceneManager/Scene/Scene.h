#pragma once
#include <Windows.h>
#include "../../GameManager/GameManager.h"
#include "../../BaseWorking/BaseWorking.h"
#include "../../FBX/ClassFBX.h"
#include "../../CustomVertices/CustomVertices.h"
#include "../../Draw/Draw.h"

enum class SceneID;

class Scene :public BaseWorking
{
public:
	Scene() {};
	~Scene() {};

	VOID syncNextScene(SceneID* pNextScene);

	VOID syncLibInstance(GameManager* pGameManager, CustomVertices* pCustomVertices, Draw* pDraw, InputData inputData);

protected:
	static SceneID* m_pNextScene;
	GameManager* m_pGameManager;
	CustomVertices* m_pCustomVertices;
	Draw* m_pDraw;
	InputData m_inputData;
};
