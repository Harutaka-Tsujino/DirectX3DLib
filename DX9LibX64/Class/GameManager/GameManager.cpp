#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>
#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#include "Window\Window.h"
#include "DirectX\DirectX.h"
#include "../FPS/FPS.h"
#include "../Draw/Draw.h"
#include "GameManager.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

GameManager* GameManager::m_pGameManager = NULL;
D3DXVECTOR2 GameManager::m_displaySize;
InputData GameManager::m_inputData;
VOID(*GameManager::m_pMainFunc)();

GameManager* GameManager::GetInstance(HINSTANCE hInst, TCHAR* pAppName, VOID(*pMainFunc)(), D3DXVECTOR2 displaySize, BOOL canWindow)
{
	if (!m_pGameManager)
	{
		m_displaySize = displaySize;
		m_pMainFunc = pMainFunc;
		m_pGameManager = new GameManager(hInst, pAppName, canWindow);
	}

	return m_pGameManager;
}

GameManager* GameManager::CopyInstance()
{
	return m_pGameManager;
}

GameManager::GameManager(HINSTANCE hInst, TCHAR* pAppName, BOOL canWindow)
{
	m_pWindow = Window::GetInstance(hInst, pAppName);
	m_pWindow->SetSizeDisplay(m_displaySize);
	m_pWindow->SetWindowMode(canWindow);

	m_pDirectX = DirectX::GetInstance();
	m_pDirectX->m_DirectXObject.SetWindowMode(canWindow);
}

GameManager::~GameManager()
{
}

VOID GameManager::Create()
{
	InitializeWindow();
	InitializeDirectX();
	Draw::SetLPDIRECT3DDEVICE9(GetDirectX3DDevice());
	LoopMainFunc();
}

VOID GameManager::InitializeDirectX()
{
	m_pDirectX->SetHWND(m_pWindow->GetHWNDPtr());

	m_pDirectX->m_DirectXObject.Initialize();
	m_pDirectX->m_DirectX3DDevice.Initialize();
	m_pDirectX->m_DirectXInputDevices.Initialize();
}

VOID GameManager::InitializeWindow()
{
	m_pWindow->MakeWindow();
}

VOID GameManager::UpdateInput()
{
	DirectXInputDevices& rDirectXInputDevices = m_pDirectX->m_DirectXInputDevices;
	rDirectXInputDevices.StoreInputStates();
	rDirectXInputDevices.GetInputStates();
	rDirectXInputDevices.GetInputData(&m_inputData);
}

InputData GameManager::GetInputData()
{
	return m_inputData;
}

VOID GameManager::LoopMainFunc()
{
	DirectX3DDevice& rDirectX3DDevice = m_pDirectX->m_DirectX3DDevice;

	FPS* pFPS = FPS::GetInstance();


	while (m_pWindow->GetMSG().message != WM_QUIT)
	{
		if (!m_pWindow->ConfirmMessage())
		{
			continue;
		}

		pFPS->UpdateTime();

		if (pFPS->CoordinateFrame())
		{
			UpdateInput();
			rDirectX3DDevice.PrepareRender();
			m_pDirectX->m_DirectX3DDevice.m_camera.SetTransform();
			m_pMainFunc();
			rDirectX3DDevice.CleanUpRender();
		}
	}

	pFPS->DeleteInstance();
}

LPDIRECT3DDEVICE9 GameManager::GetDirectX3DDevice()
{
	return m_pDirectX->GetDirectXInstances().m_pDirectX3DDevice;
}

INT GameManager::DeleteInstance()
{
	INT returnValue = (INT)m_pWindow->GetMSG().wParam;

	m_pWindow->DeleteInstance();
	m_pWindow = NULL;

	m_pDirectX->DeleteInstance();
	m_pDirectX = NULL;

	delete m_pGameManager;
	m_pGameManager = NULL;

	return returnValue;
}

VOID GameManager::GetDisplaySize(D3DXVECTOR2* pDisplaySize)
{
	*pDisplaySize = m_displaySize;
}
