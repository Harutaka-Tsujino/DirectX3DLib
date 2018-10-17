#pragma once
#include <windows.h>
#include <tchar.h>
#include "Window\Window.h"
#include "DirectX\DirectX.h"

class GameManager
{
public:
	static GameManager* GetInstance(HINSTANCE hInst, TCHAR* pAppName, VOID(*pMainFunc)(), D3DXVECTOR2 displaySize, BOOL canWindown);
	static GameManager* CopyInstance();

	VOID Create();
	InputData GetInputData();
	INT DeleteInstance();

private:
	GameManager(HINSTANCE hInst, TCHAR* pAppName, BOOL canWindow);
	~GameManager();

	VOID InitializeWindow();
	VOID InitializeDirectX();
	VOID UpdateInput();
	VOID LoopMainFunc();

	static GameManager* m_pGameManager;
	static D3DXVECTOR2 m_displaySize;
	static InputData m_inputData;
	static VOID(*m_pMainFunc)();
	Window* m_pWindow;
	DirectX* m_pDirectX;
};
