#include <windows.h>
#include <iostream>
#include <vector>
#include <d3dx9.h>
#include <dinput.h>
#include "DX9LibX64.h"
#include "ClassWindow.h"
#include "ClassFPS.h"
#include "ClassDirectX.h"

#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

INT LoopMainFunc(VOID(*func)(), Window* pWindow, DirectX* pDirectX)
{
	pWindow->MakeWindow();

	pDirectX->SetHWND(&pWindow->m_hWnd);

	if (FAILED(pDirectX->m_DirectXObject.Initialize()))
	{
		return FALSE;
	}

	if (FAILED(pDirectX->m_DirectX3DDevice.Initialize()))
	{
		return FALSE;
	}

	if (FAILED(pDirectX->m_DirectXInputDevices.Initialize()))
	{
		return FALSE;
	}

	FPS* pFPS = FPS::GetInstance();

	DirectXInputDevices& rDirectXInputDevices = pDirectX->m_DirectXInputDevices;
	DirectX3DDevice& rDirectX3DDevice = pDirectX->m_DirectX3DDevice;

	while (pWindow->m_msg.message != WM_QUIT)
	{
		if (!pWindow->ConfirmMessage())
		{
			continue;
		}

		pFPS->UpdateTime();

		if (pFPS->CoordinateFrame())
		{
			rDirectXInputDevices.GetInputStates();
			rDirectX3DDevice.PrepareRender();
			(*func)();
			rDirectX3DDevice.CleanUpRender();
			rDirectXInputDevices.StoreInputStates();
		}
	}

	return(INT)pWindow->m_msg.wParam;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	Window* pWindow = Window::GetInstance(hInst, u8"TestApp");

	DirectX* pDirectX = DirectX::GetInstance();

	return LoopMainFunc(MainFunc, pWindow, pDirectX);
}

VOID MainFunc()
{
	return;
}
