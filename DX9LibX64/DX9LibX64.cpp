#include<windows.h>
#include<iostream>
#include<vector>
#include<d3dx9.h>
#include <dinput.h>
#include"DX9LibX64.h"
#include"ClassWindow.h"
#include"ClassFPS.h"
#include"ClassDirectX.h"

#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

INT LoopMainFunc(VOID(*func)(),Window* pWindow, DirectX* pDirectX)
{
	pWindow->MakeWindow();

	if (FAILED(pDirectX->InitDirectX()))
	{
		return FALSE;
	}

	if (FAILED(pDirectX->InitDirectX3DDevice()))
	{
		return FALSE;
	}

	if (FAILED(pDirectX->InitDirectXInputDevices()))
	{
		return FALSE;
	}
	
	FPS* pFPS = FPS::GetInstancePointer();

	while (pWindow->m_msg.message != WM_QUIT)
	{
		if (pWindow->ConfirmMessage())
		{
			pFPS->UpdateTime();

			if (pFPS->CoordinateFrame())
			{
				pDirectX->GetInputStates();
				pDirectX->PrepareRender();
				(*func)();
				pDirectX->CleanUpRender();
				pDirectX->UpdateInputStates();
			}
		}
	}

	return(INT)pWindow->m_msg.wParam;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	Window* pWindow = Window::GetInstancePointer();
	pWindow->Init<INT,INT>(hInst, u8"DX9LibX64", 1920, 1080, TRUE);

	DirectX* pDirectX = DirectX::GetInstancePointer();
	pDirectX->Init(&(pWindow->m_hWnd), pWindow->m_window,TRUE, (D3DFVF_XYZ | D3DFVF_DIFFUSE));

	return LoopMainFunc(MainFunc, pWindow, pDirectX);
}

VOID MainFunc()
{
	return;
}
