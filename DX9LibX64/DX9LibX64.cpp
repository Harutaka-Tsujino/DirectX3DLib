#include <windows.h>
#include <iostream>
#include <vector>
#include <d3dx9.h>
#include <dinput.h>
#include "DX9LibX64.h"
#include "ClassWindow.h"
#include "ClassFPS.h"
#include "ClassDirectX.h"
#include "ClassFBX.h"
#include "FbxRelated.h"

#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

INT LoopMainFunc(VOID(*func)(), Window* pWindow, DirectX* pDirectX)
{
	pWindow->SetSizeDisplay<INT,INT>(1920, 1080);

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
	Window* pWindow = Window::GetInstance(hInst, TEXT("TestApp"));

	DirectX* pDirectX = DirectX::GetInstance();

	return LoopMainFunc(MainFunc, pWindow, pDirectX);
}

VOID MainFunc()
{
	static INT frameCount = -1;

	static FbxRelated fbxRelated;

	if (frameCount == -1)
	{
		fbxRelated.LoadFbx("Sting-Sword lowpoly.fbx");

		frameCount = 0;
	}

	static Camera camera;


	D3DXMATRIX			m_MatWorld;
	D3DXMatrixIdentity(&m_MatWorld);

	camera.SetTransform();

	// ‰ñ“]
	D3DXMATRIX			matHeading;
	D3DXMatrixRotationX(&matHeading, 0);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matHeading);

	// ˆÚ“®
	D3DXMATRIX			matPosition;	// ˆÊ’uÀ•Ws—ñ
	D3DXMatrixTranslation(&matPosition, 0, 0, 70.f);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matPosition);
	DirectX::m_directXInstances.m_pDirectX3DDevice->SetTransform(D3DTS_WORLD, &m_MatWorld);

	D3DXVECTOR3 vecDirection(1, 1, 1);
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;

	light.Specular.r = 1.f;
	light.Specular.b = 1.f;
	light.Specular.g = 1.f;

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);

	light.Range = 200.f;
	DirectX::m_directXInstances.m_pDirectX3DDevice->SetLight(0, &light);
	DirectX::m_directXInstances.m_pDirectX3DDevice->LightEnable(0, TRUE);

	fbxRelated.m_pModel->DrawFbx();

	return;
}
