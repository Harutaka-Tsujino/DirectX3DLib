#include <windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <tchar.h>
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
	pWindow->SetSizeDisplay<INT, INT>(1920, 1080);

	pWindow->MakeWindow();

	pDirectX->m_DirectXObject.SetWindowMode(pWindow->m_canWindow);

	pDirectX->SetHWND(pWindow->GetHWNDPtr());

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

	while (pWindow->GetMSG().message != WM_QUIT)
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

	return(INT)pWindow->GetMSG().wParam;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	Window* pWindow = Window::GetInstance(hInst, TEXT("TestApp"));

	pWindow->SetWindowMode(FALSE);

	DirectX* pDirectX = DirectX::GetInstance();

	return LoopMainFunc(MainFunc, pWindow, pDirectX);
}

VOID CustomImageVerticies(CustomVertex *pCustomVertex, FLOAT posX, FLOAT posY, FLOAT posZ, FLOAT scaleX, FLOAT scaleY,
	DWORD color, FLOAT startPosTu, FLOAT startPosTv, FLOAT scaleTu, FLOAT scaleTv, FLOAT scaleImageX, FLOAT scaleImageY)
{
	pCustomVertex[0] = { posX - scaleX ,posY - scaleY,posZ,1,color,startPosTu / scaleImageX,startPosTv / scaleImageY };
	pCustomVertex[1] = { posX + scaleX ,posY - scaleY,posZ,1,color,(startPosTu + scaleTu) / scaleImageX,startPosTv / scaleImageY };
	pCustomVertex[2] = { posX + scaleX ,posY + scaleY,posZ,1,color,(startPosTu + scaleTu) / scaleImageX,(startPosTv + scaleTv) / scaleImageY };
	pCustomVertex[3] = { posX - scaleX ,posY + scaleY,posZ,1,color,startPosTu / scaleImageX,(startPosTv + scaleTv) / scaleImageY };

	return;
}

VOID MainFunc()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;
	Camera& rCamera = pDirectX->m_DirectX3DDevice.m_camera;

	const INT INIT_FRAME = -1;

	static INT frameCount = INIT_FRAME;

	//FBXインスタンスの生成
	static FbxRelated mP7;

	static FbxRelated cartridgeFbx;

	static std::map<TCHAR*, LPDIRECT3DTEXTURE9> textures;

	if (frameCount == INIT_FRAME)
	{
		//FBXモデルの読み込み
		mP7.LoadFbx("MP7.fbx");

		cartridgeFbx.LoadFbx("Cartridge.45ACP/SA_45ACP_Example.fbx");

		textures[_T("reticle_line")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/reticle_line.png"),
			&textures[_T("reticle_line")]);

		textures[_T("reticle_line2")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/reticle_line2.png"),
			&textures[_T("reticle_line2")]);

		textures[_T("reticle_dot")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/reticle_dot.png"),
			&textures[_T("reticle_dot")]);

		textures[_T("tan")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/tan.png"),
			&textures[_T("tan")]);

		textures[_T("matBlack")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/matBlack.png"),
			&textures[_T("matBlack")]);

		textures[_T("testBack")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("testBack.jpg"),
			&textures[_T("testBack")]);

		textures[_T("cartridge")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("Cartridge.45ACP/SA_45ACP_Specular.png"),
			&textures[_T("cartridge")]);

		textures[_T("mazulFlash")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("Cartridge.45ACP/mazulFlash.png"),
			&textures[_T("mazulFlash")]);

		textures[_T("mazulFlash1")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("Cartridge.45ACP/mazulFlash1.png"),
			&textures[_T("mazulFlash1")]);

		textures[_T("mazulFlash2")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("Cartridge.45ACP/mazulFlash2.png"),
			&textures[_T("mazulFlash2")]);

		frameCount = 0;
	}

	rpDirectX3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	CustomVertex backVertices[4];

	CustomImageVerticies(backVertices, 1920.0f*0.5f, 1080.0f*0.5f, 200.0f,
		1920.0f*0.5f, 1080.0f*0.5f,
		0xFFFFFFFF,
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	rpDirectX3DDevice->SetTexture(0, textures[_T("testBack")]);

	//rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backVertices, sizeof(CustomVertex));

	static float diffusionCount = 1.0f;

	MouseState& rMouseState = pDirectX->m_DirectXInputDevices.m_InputData.m_mouseState;

	if (rMouseState.m_buttonHold[0])
	{
		diffusionCount = diffusionCount * 2.0f;
	}

	if (rMouseState.m_buttonRelease[0])
	{
		diffusionCount -= 30.0f;
	}

	if (rMouseState.m_buttonUninput[0])
	{
		diffusionCount -= 10.0f;
	}

	CustomVertex reticleLengthVertices[4];

	const INT RETICLE_LENGTH_COUNT = 2;

	diffusionCount = max(min(diffusionCount, 300.0f), 1.0f);

	for (int i = 0; i < RETICLE_LENGTH_COUNT; ++i)
	{
		CustomImageVerticies(reticleLengthVertices, 1920.0f*0.5f, 1080.0f*(0.46f + (i*0.08f)) + ((i == 0) ? -diffusionCount : diffusionCount), 0.0f,
			12.0f, 18.0f,
			0xFF22BB22,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		if (!i)rpDirectX3DDevice->SetTexture(0, textures[_T("reticle_line")]);

		rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, reticleLengthVertices, sizeof(CustomVertex));
	}

	CustomVertex reticleWidthVertices[4];

	const INT RETICLE_WIDTH_COUNT = 2;

	for (int i = 0; i < RETICLE_WIDTH_COUNT; ++i)
	{
		CustomImageVerticies(reticleWidthVertices, (1920.0f*0.5f + 1080.0f*0.04f*((i == 0) ? -1.0f : 1.0f)) + ((i == 0) ? -diffusionCount : diffusionCount), 1080.0f*0.50f, 0.0f,
			18.0f, 12.0f,
			0xFF22BB22,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		if (!i)rpDirectX3DDevice->SetTexture(0, textures[_T("reticle_line2")]);

		rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, reticleWidthVertices, sizeof(CustomVertex));
	}

	CustomVertex reticleDotVertices[4];

	CustomImageVerticies(reticleDotVertices, 1920.0f*0.5f, 1080.0f*0.5f, 0.0f,
		30.0f, 30.0f,
		0xFF22BB22,
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	rpDirectX3DDevice->SetTexture(0, textures[_T("reticle_dot")]);

	rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, reticleDotVertices, sizeof(CustomVertex));

	static int renderCount = 0;
	if (rMouseState.m_buttonHold[0] && !renderCount)
	{
		renderCount = 1;
	}

	if (0 < renderCount&&renderCount < 5)
	{
		renderCount += 1;
	}

	if (renderCount >= 5)
	{
		renderCount = 0;
	}

	if (renderCount)
	{
		switch (renderCount)
		{
		case 2:
			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash")]);
			break;
		case 3:
			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash1")]);
			break;
		case 4:
			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash2")]);
			break;
		default:
			rpDirectX3DDevice->SetTexture(0, NULL);
			break;
		}

		CustomVertex mazulFlashVertices[4];

		CustomImageVerticies(mazulFlashVertices, 1920.0f*0.60718f, 1080.0f*0.757f, 0.985f,
			400.0f, 400.0f,
			0x88FE9A2E,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, mazulFlashVertices, sizeof(CustomVertex));

		CustomImageVerticies(mazulFlashVertices, 1920.0f*0.60718f, 1080.0f*0.757f, 0.985f,
			350.0f, 350.0f,
			0xAAF3F781,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, mazulFlashVertices, sizeof(CustomVertex));

		CustomImageVerticies(mazulFlashVertices, 1920.0f*0.60718f, 1080.0f*0.757f, 0.985f,
			230.0f, 230.0f,
			0xEEFFFFFF,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, mazulFlashVertices, sizeof(CustomVertex));
	}

	rpDirectX3DDevice->SetTexture(0, NULL);

	pDirectX->m_DirectX3DDevice.m_camera.SetTransform();

	D3DXVECTOR3 vecDirection(0.01f, 0.05f, 0.3f);
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
	rpDirectX3DDevice->SetLight(0, &light);
	rpDirectX3DDevice->LightEnable(0, TRUE);

	//FBXの描画
	for (int i = 0; i < mP7.m_modelDataCount; ++i)
	{
		D3DXMATRIX			m_MatWorld;
		D3DXMatrixIdentity(&m_MatWorld);

		D3DXMATRIX			matScal;
		D3DXMatrixScaling(&matScal, 0.003f,0.003f,0.003f);
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matScal);

		D3DXMATRIX			matRoll;
		D3DXMatrixRotationZ(&matRoll, 0.0f * (D3DX_PI / 180.f));
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matRoll);

		D3DXMATRIX			matPitch;
		D3DXMatrixRotationX(&matPitch, 0.0f * (D3DX_PI / 180.f));
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matPitch);

		D3DXMATRIX			matYaw;
		D3DXMatrixRotationY(&matYaw, 180.0f * (D3DX_PI / 180.0f));
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matYaw);

		//850/60/60*4 =0.94444444
		//銃を撃った時の演出フレーム
		static const int SHOT_RENDER_TIME = 4;

		//演出を測るカウント
		static int shotRenderCount = 0;

		if (rMouseState.m_buttonHold[0]&& !shotRenderCount)
		{
			shotRenderCount = 1;
		}

		//反動の表現
		D3DXMATRIX			matGunShock;
		D3DXMatrixRotationX(&matGunShock, -2.5f*shotRenderCount * (D3DX_PI / 180.f));
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matGunShock);

		//視点変更
		D3DXMatrixRotationY(&matYaw, -20.0f * (D3DX_PI / 180.0f));
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matYaw);

		D3DXVECTOR3 gunPos(0.05f, -0.06f, 0.1f - shotRenderCount * 0.005f);

		D3DXMATRIX			matLookPtMove;

		D3DXMatrixRotationY(&matLookPtMove, -20.0f * (D3DX_PI / 180.f));

		D3DXVec3TransformCoord(&gunPos, &gunPos, &matLookPtMove);

		D3DXVECTOR3 cameraPos;

		rCamera.GetCameraPos(&cameraPos);

		D3DXVec3Add(&gunPos, &gunPos, &cameraPos);

		D3DXMATRIX			matPosition;	// 位置座標行列
		D3DXMatrixTranslation(&matPosition, gunPos.x, gunPos.y, gunPos.z);
		D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matPosition);
		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &m_MatWorld);
	
		if (shotRenderCount)
		{
			++shotRenderCount;
		}

		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonHold[0])
		{
			shotRenderCount = SHOT_RENDER_TIME-2;
		}

		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonUninput[0])
		{
			shotRenderCount = 0;
		}

		rpDirectX3DDevice->SetTexture(0, textures[_T("matBlack")]);

		mP7.m_pModel[i]->DrawFbx();
	}

	for (int i = 0; i < cartridgeFbx.m_modelDataCount; ++i)
	{
		D3DXMATRIX			m_MatWorld;
		D3DXMatrixIdentity(&m_MatWorld);

		// 回転
		//D3DXMATRIX			matHeading;
		//D3DXMatrixRotationY(&matHeading, 170.0f * (3.145f / 180.f));//180.3f真後ろに来る角度
		//D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matHeading);

		static int renderCount = 0;

		static int cartridgeRenderRand = 0;

		if (rMouseState.m_buttonHold[0] && !renderCount)
		{
			renderCount = 1;
		}

		if (0 < renderCount&&renderCount < 5)
		{
			renderCount += 1;

			cartridgeRenderRand = rand() % 10;
		}

		if (renderCount >= 5)
		{
			renderCount = 0;
		}

		if (renderCount)
		{
			// 回転
			D3DXMATRIX			matPitching;
			D3DXMatrixRotationY(&matPitching, (20.0f*renderCount + cartridgeRenderRand) * (3.145f / 180.f));//180.3f真後ろに来る角度
			D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matPitching);

			D3DXMATRIX			matPosition;	// 位置座標行列
			D3DXMatrixTranslation(&matPosition, 7.9f + renderCount * 0.8f, -4.9f + renderCount * 0.1f + cartridgeRenderRand * 0.005f, 11.0f);
			D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matPosition);
			rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &m_MatWorld);

		}

		rpDirectX3DDevice->SetTexture(0, textures[_T("cartridge")]);

		cartridgeFbx.m_pModel[i]->DrawFbx();
	}

	return;
}
