#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>
#include <dinput.h>
#include <crtdbg.h>
#include <iostream>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include "DX9LibX64.h"
#include "../Class/GameManager/GameManager.h"
#include "../Class/FBX/FbxRelated.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

VOID MainFunc()
{
	GameManager* pGameManager = GameManager::CopyInstance();
	InputData inputData = pGameManager->GetInputData();

	return;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	D3DXVECTOR2 displaySize(1280.0f, 720.0f);

	GameManager* pGameManager = GameManager::GetInstance(hInst, _T("TEST_TEST_TEST"), MainFunc, displaySize, TRUE);
	pGameManager->Create();
	pGameManager->DeleteInstance();
	return 0;
}

//INT LoopMainFunc(VOID(*func)(), Window* pWindow, DirectX* pDirectX)
//{
//	pWindow->SetSizeDisplay(1920, 1080);
//
//	pWindow->MakeWindow();
//
//	pDirectX->m_DirectXObject.SetWindowMode(pWindow->GetWindowMode());
//
//	pDirectX->SetHWND(pWindow->GetHWNDPtr());
//
//	if (FAILED(pDirectX->m_DirectXObject.Initialize()))
//	{
//		return FALSE;
//	}
//
//	if (FAILED(pDirectX->m_DirectX3DDevice.Initialize()))
//	{
//		return FALSE;
//	}
//
//	if (FAILED(pDirectX->m_DirectXInputDevices.Initialize()))
//	{
//		return FALSE;
//	}
//
//	FPS* pFPS = FPS::GetInstance();
//
//	DirectXInputDevices& rDirectXInputDevices = pDirectX->m_DirectXInputDevices;
//	DirectX3DDevice& rDirectX3DDevice = pDirectX->m_DirectX3DDevice;
//
//	while (pWindow->GetMSG().message != WM_QUIT)
//	{
//		if (!pWindow->ConfirmMessage())
//		{
//			continue;
//		}
//
//		pFPS->UpdateTime();
//
//		if (pFPS->CoordinateFrame())
//		{
//			rDirectXInputDevices.GetInputStates();
//			rDirectX3DDevice.PrepareRender();
//			(*func)();
//			rDirectX3DDevice.CleanUpRender();
//			rDirectXInputDevices.StoreInputStates();
//		}
//	}
//
//	return(INT)pWindow->GetMSG().wParam;
//}

//VOID Custom2DVertices(CustomVertex *pCustomVertex, FLOAT posX, FLOAT posY, FLOAT posZ, FLOAT scaleX, FLOAT scaleY,
//	DWORD color, FLOAT startPosTu, FLOAT startPosTv, FLOAT scaleTu, FLOAT scaleTv, FLOAT scaleImageX, FLOAT scaleImageY)
//{
//	pCustomVertex[0] = { posX - scaleX ,posY - scaleY,posZ,1,color,startPosTu / scaleImageX,startPosTv / scaleImageY };
//	pCustomVertex[1] = { posX + scaleX ,posY - scaleY,posZ,1,color,(startPosTu + scaleTu) / scaleImageX,startPosTv / scaleImageY };
//	pCustomVertex[2] = { posX + scaleX ,posY + scaleY,posZ,1,color,(startPosTu + scaleTu) / scaleImageX,(startPosTv + scaleTv) / scaleImageY };
//	pCustomVertex[3] = { posX - scaleX ,posY + scaleY,posZ,1,color,startPosTu / scaleImageX,(startPosTv + scaleTv) / scaleImageY };
//
//	return;
//}

//3Dの頂点フォーマット
//struct Vertex3D
//{
//public:
//	FLOAT m_x;
//	FLOAT m_y;
//	FLOAT m_z;
//	DWORD m_aRGB;
//	FLOAT m_tu;
//	FLOAT m_tv;
//};

//VOID MainFunc()
//{
//	ShowCursor(FALSE);
//
//	DirectX* pDirectX = DirectX::GetInstance();
//	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
//	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;
//	Camera& rCamera = pDirectX->m_DirectX3DDevice.m_camera;
//	MouseState& rMouseState = pDirectX->m_DirectXInputDevices.m_InputData.m_mouseState;
//
//	const INT INIT_FRAME = -1;
//
//	static INT frameCount = INIT_FRAME;
//
//	//FBXインスタンスの生成
//	static FbxRelated mP7;
//
//	static FbxRelated cartridgeFbx;
//
//	static FbxRelated boardFbx;
//
//	static FbxRelated cubeFbx;
//
//	static std::map<TCHAR*, LPDIRECT3DTEXTURE9> textures;
//
//	//モデルや画像の読み込み
//	if (frameCount == INIT_FRAME)
//	{
//		//FBXモデルの読み込み
//		mP7.LoadFbx("MP7.fbx");
//
//		cartridgeFbx.LoadFbx("Cartridge.45ACP/SA_45ACP_Example.fbx");
//
//		boardFbx.LoadFbx("boad.fbx");
//
//		cubeFbx.LoadFbx("Portal_2_Cube_Dice.FBX");
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/reticle_line.png"),
//			&textures[_T("reticle_line")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/reticle_line2.png"),
//			&textures[_T("reticle_line2")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/reticle_dot.png"),
//			&textures[_T("reticle_dot")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/tan.png"),
//			&textures[_T("tan")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/matBlack.png"),
//			&textures[_T("matBlack")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("testBack.jpg"),
//			&textures[_T("testBack")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("Cartridge.45ACP/SA_45ACP_Specular.png"),
//			&textures[_T("cartridge")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/mazulFlash0.png"),
//			&textures[_T("mazulFlash0")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/mazulFlash1.png"),
//			&textures[_T("mazulFlash1")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/mazulFlash2.png"),
//			&textures[_T("mazulFlash2")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/mazulFlash3.png"),
//			&textures[_T("mazulFlash3")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/ground.png"),
//			&textures[_T("ground")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/lockOn.png"),
//			&textures[_T("lockOn")]);
//
//		D3DXCreateTextureFromFile(rpDirectX3DDevice,
//			TEXT("textures/redTexture.png"),
//			&textures[_T("redTex")]);
//
//		frameCount = 0;
//	}
//
//	//2Dの描画
//	{
//		rpDirectX3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//
//		CustomVertex backVertices[4];
//
//		Custom2DVertices(backVertices, 1920.0f*0.5f, 1080.0f*0.5f, 200.0f,
//			1920.0f*0.5f, 1080.0f*0.5f,
//			0xFFFFFFFF,
//			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
//
//		rpDirectX3DDevice->SetTexture(0, textures[_T("testBack")]);
//
//		//rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backVertices, sizeof(CustomVertex));
//
//		static FLOAT diffusionValue = 1.0f;
//		static INT diffusionCaunt = 1;
//
//		diffusionValue = 0.6f*(FLOAT)pow(diffusionCaunt, 2);
//
//		if (rMouseState.m_buttonHold[0])
//		{
//			++diffusionCaunt;
//		}
//
//		if (rMouseState.m_buttonUninput[0])
//		{
//			diffusionCaunt -= 1;
//		}
//
//		CustomVertex reticleLengthVertices[4];
//
//		const INT RETICLE_LENGTH_COUNT = 2;
//
//		diffusionCaunt = max(min(diffusionCaunt, 27), 1);
//
//		for (INT i = 0; i < RETICLE_LENGTH_COUNT; ++i)
//		{
//			Custom2DVertices(reticleLengthVertices, 1920.0f*0.5f, 1080.0f*(0.46f + (i*0.08f)) + ((i == 0) ? -diffusionValue : diffusionValue), 0.0f,
//				12.0f, 18.0f,
//				0xFF22BB22,
//				0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
//
//			if (!i)rpDirectX3DDevice->SetTexture(0, textures[_T("reticle_line")]);
//
//			rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, reticleLengthVertices, sizeof(CustomVertex));
//		}
//
//		CustomVertex reticleWidthVertices[4];
//
//		const INT RETICLE_WIDTH_COUNT = 2;
//
//		for (INT i = 0; i < RETICLE_WIDTH_COUNT; ++i)
//		{
//			Custom2DVertices(reticleWidthVertices, (1920.0f*0.5f + 1080.0f*0.04f*((i == 0) ? -1.0f : 1.0f)) + ((i == 0) ? -diffusionValue : diffusionValue), 1080.0f*0.50f, 0.0f,
//				18.0f, 12.0f,
//				0xFF22BB22,
//				0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
//
//			if (!i)rpDirectX3DDevice->SetTexture(0, textures[_T("reticle_line2")]);
//
//			rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, reticleWidthVertices, sizeof(CustomVertex));
//		}
//
//		CustomVertex reticleDotVertices[4];
//
//		Custom2DVertices(reticleDotVertices, 1920.0f*0.5f, 1080.0f*0.5f, 0.0f,
//			30.0f, 30.0f,
//			0xFF22BB22,
//			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
//
//		rpDirectX3DDevice->SetTexture(0, textures[_T("reticle_dot")]);
//
//		rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, reticleDotVertices, sizeof(CustomVertex));
//	}
//
//	//銃を撃った時の演出フレーム
//	static const INT SHOT_RENDER_TIME = 5;
//
//	//演出を測るカウント
//	static INT shotRenderCount = 0;
//
//	if (rMouseState.m_buttonHold[0] && !shotRenderCount)
//	{
//		shotRenderCount = 1;
//	}
//
//	pDirectX->m_DirectX3DDevice.m_camera.SetTransform();
//
//	//ライトの設定
//	{
//		D3DXVECTOR3 vecDirection(0.01f, 0.05f, 0.3f);
//		D3DLIGHT9 light;
//
//		ZeroMemory(&light, sizeof(D3DLIGHT9));
//
//		light.Type = D3DLIGHT_DIRECTIONAL;
//		light.Diffuse.r = 1.f;
//		light.Diffuse.g = 1.f;
//		light.Diffuse.b = 1.f;
//
//		light.Specular.r = 1.f;
//		light.Specular.b = 1.f;
//		light.Specular.g = 1.f;
//
//		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);
//
//		light.Range = 200.f;
//		rpDirectX3DDevice->SetLight(0, &light);
//		rpDirectX3DDevice->LightEnable(0, TRUE);
//	}
//
//	rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	const INT RECT_VERTICES_NUM = 4;
//
//	//ビルボードの描画
//	if (shotRenderCount)
//	{
//		rpDirectX3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//
//		LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;
//
//		rpDirectX3DDevice->CreateVertexBuffer(
//			4 * sizeof(Vertex3D),
//			D3DUSAGE_WRITEONLY,
//			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
//			D3DPOOL_MANAGED,
//			&vertexBuffer,
//			NULL);
//
//		Vertex3D* billBoard;
//		vertexBuffer->Lock(0, 0, (void**)&billBoard, 0);
//
//		for (INT i = 0; i < RECT_VERTICES_NUM; ++i)
//		{
//			billBoard[i].m_aRGB = 0xFFFFCC11;
//		}
//
//		billBoard[0].m_x = -5.0f;
//		billBoard[0].m_y = 5.0f;
//		billBoard[0].m_z = 0.0f;
//		billBoard[0].m_tu = 0.0f;
//		billBoard[0].m_tv = 0.0f;
//
//		billBoard[1].m_x = 5.0f;
//		billBoard[1].m_y = 5.0f;
//		billBoard[1].m_z = 0.0f;
//		billBoard[1].m_tu = 1.0f;
//		billBoard[1].m_tv = 0.0f;
//
//		billBoard[2].m_x = 5.0f;
//		billBoard[2].m_y = -5.0f;
//		billBoard[2].m_z = 0.0f;
//		billBoard[2].m_tu = 1.0f;
//		billBoard[2].m_tv = 1.0f;
//
//		billBoard[3].m_x = -5.0f;
//		billBoard[3].m_y = -5.0f;
//		billBoard[3].m_z = 0.0f;
//		billBoard[3].m_tu = 0.0f;
//		billBoard[3].m_tv = 1.0f;
//
//		D3DXMATRIX matWorld, matTrans, matRotX, matRotY, matRotZ, matScale;
//		D3DXMatrixIdentity(&matWorld);
//		D3DXMatrixIdentity(&matTrans);
//		D3DXMatrixIdentity(&matScale);
//
//		// 拡大
//		D3DXMatrixScaling(&matScale, 0.007f, 0.007f, 0.007f);
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
//
//		// 回転
//		D3DXMatrixRotationX(&matRotX, D3DXToRadian(0.0f));
//		D3DXMatrixRotationY(&matRotY, D3DXToRadian(0.0f));
//		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.0f));
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotY);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotX);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotZ);
//
//		// 移動
//		D3DXMatrixTranslation(&matTrans, 0.049f, -0.0285f, 0.137f);
//
//		// 掛け合わせ
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
//
//		//ビューの逆行列をかけてビルボード化
//		rCamera.NegateView(&matWorld);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		vertexBuffer->Unlock();
//
//		rpDirectX3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex3D));
//
//		switch (shotRenderCount)
//		{
//		case 1:
//
//			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash0")]);
//			break;
//		case 2:
//
//			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash1")]);
//			break;
//		case 3:
//
//			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash2")]);
//			break;
//		case 4:
//
//			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash3")]);
//			break;
//		case 5:
//
//			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash1")]);
//			break;
//		}
//
//		rpDirectX3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
//
//		vertexBuffer->Release();
//		vertexBuffer = NULL;
//	}
//
//	static bool canLockOn = false;
//
//	KeyBoardState& rKeyState = pDirectX->m_DirectXInputDevices.m_InputData.m_keyBoardState;
//
//	if (rKeyState.m_keyPush[DIK_Q])
//	{
//		canLockOn = (canLockOn) ? false : true;
//	}
//
//	if (canLockOn)
//	{
//		rpDirectX3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//
//		Vertex3D lockOnVertices[RECT_VERTICES_NUM];
//
//		for (INT i = 0; i < RECT_VERTICES_NUM; ++i)
//		{
//			lockOnVertices[i].m_aRGB = 0xFFFFFFFF;
//		}
//
//		lockOnVertices[0].m_x = -5.0f;
//		lockOnVertices[0].m_y = 5.0f;
//		lockOnVertices[0].m_z = 0.0f;
//		lockOnVertices[0].m_tu = 0.0f;
//		lockOnVertices[0].m_tv = 0.0f;
//
//		lockOnVertices[1].m_x = 5.0f;
//		lockOnVertices[1].m_y = 5.0f;
//		lockOnVertices[1].m_z = 0.0f;
//		lockOnVertices[1].m_tu = 1.0f;
//		lockOnVertices[1].m_tv = 0.0f;
//
//		lockOnVertices[2].m_x = 5.0f;
//		lockOnVertices[2].m_y = -5.0f;
//		lockOnVertices[2].m_z = 0.0f;
//		lockOnVertices[2].m_tu = 1.0f;
//		lockOnVertices[2].m_tv = 1.0f;
//
//		lockOnVertices[3].m_x = -5.0f;
//		lockOnVertices[3].m_y = -5.0f;
//		lockOnVertices[3].m_z = 0.0f;
//		lockOnVertices[3].m_tu = 0.0f;
//		lockOnVertices[3].m_tv = 1.0f;
//
//		D3DXMATRIX matWorld, matTrans, matRotX, matRotY, matRotZ, matScale;
//		D3DXMatrixIdentity(&matWorld);
//		D3DXMatrixIdentity(&matTrans);
//		D3DXMatrixIdentity(&matScale);
//
//		// 拡大
//		D3DXMatrixScaling(&matScale, 0.0012f, 0.0012f, 0.0012f);
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
//
//		// 回転
//		D3DXMatrixRotationX(&matRotX, D3DXToRadian(0.0f));
//		D3DXMatrixRotationY(&matRotY, D3DXToRadian(0.0f));
//		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.0f));
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotY);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotX);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotZ);
//
//		// 移動
//		D3DXMatrixTranslation(&matTrans, 0.0f, -0.0f, 0.1f);
//
//		// 掛け合わせ
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
//
//		//ビューの逆行列をかけてビルボード化
//		rCamera.NegateView(&matWorld);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		pDirectX->m_DirectX3DDevice.DrawVertex(lockOnVertices, &textures[_T("lockOn")],
//			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, sizeof(Vertex3D)*RECT_VERTICES_NUM);
//	}
//
//	rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	//地面の描画
//	if (TRUE)
//	{
//		rpDirectX3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//
//		rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//		const INT RECT_VERTICES_NUM = 4;
//
//		LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;
//
//		rpDirectX3DDevice->CreateVertexBuffer(
//			4 * sizeof(Vertex3D),
//			D3DUSAGE_WRITEONLY,
//			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
//			D3DPOOL_MANAGED,
//			&vertexBuffer,
//			NULL);
//
//		Vertex3D* billBoard;
//		vertexBuffer->Lock(0, 0, (void**)&billBoard, 0);
//
//		for (INT i = 0; i < RECT_VERTICES_NUM; ++i)
//		{
//			billBoard[i].m_aRGB = 0xFFFFFFFF;
//		}
//
//		billBoard[0].m_x = -5.0f;
//		billBoard[0].m_y = 5.0f;
//		billBoard[0].m_z = 0.0f;
//		billBoard[0].m_tu = 0.0f;
//		billBoard[0].m_tv = 0.0f;
//
//		billBoard[1].m_x = 5.0f;
//		billBoard[1].m_y = 5.0f;
//		billBoard[1].m_z = 0.0f;
//		billBoard[1].m_tu = 1.0f;
//		billBoard[1].m_tv = 0.0f;
//
//		billBoard[2].m_x = 5.0f;
//		billBoard[2].m_y = -5.0f;
//		billBoard[2].m_z = 0.0f;
//		billBoard[2].m_tu = 1.0f;
//		billBoard[2].m_tv = 1.0f;
//
//		billBoard[3].m_x = -5.0f;
//		billBoard[3].m_y = -5.0f;
//		billBoard[3].m_z = 0.0f;
//		billBoard[3].m_tu = 0.0f;
//		billBoard[3].m_tv = 1.0f;
//
//		D3DXMATRIX matWorld, matTrans, matRotX, matRotY, matRotZ, matScale;
//		D3DXMatrixIdentity(&matWorld);
//		D3DXMatrixIdentity(&matTrans);
//		D3DXMatrixIdentity(&matScale);
//
//		// 拡大
//		D3DXMatrixScaling(&matScale, 300.0f, 300.0f, 300.0f);
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
//
//		// 回転
//		D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.0f));
//		D3DXMatrixRotationY(&matRotY, D3DXToRadian(0.0f));
//		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.0f));
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotY);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotX);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotZ);
//
//		// 移動
//		D3DXMatrixTranslation(&matTrans, 0.0f, -1.8f, 0.0f);
//
//		// 掛け合わせ
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		vertexBuffer->Unlock();
//
//		rpDirectX3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex3D));
//
//		rpDirectX3DDevice->SetTexture(0, textures[_T("ground")]);
//		rpDirectX3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
//
//		vertexBuffer->Release();
//		vertexBuffer = NULL;
//
//		rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//		rpDirectX3DDevice->SetTexture(0, NULL);
//	}
//
//	rpDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	//壁の描画
//	for (INT i = 0; i < 2; ++i)
//	{
//		D3DXMATRIX			matWorld;
//		D3DXMatrixIdentity(&matWorld);
//
//		D3DXMATRIX			matScale;
//
//		D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
//
//		D3DXMATRIX			matPosition;	// 位置座標行列
//
//		FLOAT posZ = 0.0f;
//
//		if (i == 0)
//		{
//			posZ = 1000.0f;
//			rpDirectX3DDevice->SetTexture(0, textures[_T("tan")]);
//		}
//
//		if (i == 1)
//		{
//			posZ = -1000.0f;
//			rpDirectX3DDevice->SetTexture(0, textures[_T("ground")]);
//		}
//
//		D3DXMatrixTranslation(&matPosition, 0.0f, -40.0f, posZ);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		boardFbx.m_pModel[0]->DrawFbx();
//	}
//
//	//的の描画
//	{
//		D3DXMATRIX			matWorld;
//		D3DXMatrixIdentity(&matWorld);
//
//		D3DXMATRIX			matScale;
//
//		D3DXMatrixScaling(&matScale, 0.007f, 0.007f, 0.007f);
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
//
//		D3DXMATRIX			matPosition;	// 位置座標行列
//
//		FLOAT posZ = 0.0f;
//
//		D3DXMatrixTranslation(&matPosition, 0.095f, -0.61f, 10.0f);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		rpDirectX3DDevice->SetTexture(0, NULL);
//
//		boardFbx.m_pModel[0]->DrawFbx();
//	}
//
//	rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	rpDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	//MP7A1の描画
//	for (INT i = 0; i < mP7.m_modelDataCount; ++i)
//	{
//		D3DXMATRIX			matWorld;
//		D3DXMatrixIdentity(&matWorld);
//
//		D3DXMATRIX			matScal;
//		D3DXMatrixScaling(&matScal, 0.003f, 0.003f, 0.003f);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScal);
//
//		D3DXMATRIX			matRoll;
//		D3DXMatrixRotationZ(&matRoll, 0.0f * (D3DX_PI / 180.f));
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRoll);
//
//		D3DXMATRIX			matPitch;
//		D3DXMatrixRotationX(&matPitch, 0.0f * (D3DX_PI / 180.f));
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matPitch);
//
//		D3DXMATRIX			matYaw;
//		D3DXMatrixRotationY(&matYaw, 180.0f * (D3DX_PI / 180.0f));
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matYaw);
//
//		//反動の表現
//		D3DXMATRIX			matGunShock;
//		D3DXMatrixRotationX(&matGunShock, -2.2f*shotRenderCount * (D3DX_PI / 180.f));
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matGunShock);
//
//		D3DXVECTOR3 gunPos(0.05f, -0.06f, 0.1f - shotRenderCount * 0.005f);
//
//		D3DXMATRIX			matPosition;	// 位置座標行列
//		D3DXMatrixTranslation(&matPosition, gunPos.x, gunPos.y, gunPos.z);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
//
//		rCamera.NegateView(&matWorld);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		rpDirectX3DDevice->SetTexture(0, textures[_T("matBlack")]);
//		mP7.m_pModel[i]->DrawFbx();
//	}
//
//	//薬莢の描画
//	{
//		D3DXMATRIX			matWorld;
//		D3DXMatrixIdentity(&matWorld);
//
//		static INT renderCount = 0;
//
//		static INT cartridgeRenderRand = 0;
//
//		if (rMouseState.m_buttonHold[0] && !renderCount)
//		{
//			renderCount = 1;
//		}
//
//		if (renderCount)
//		{
//			//拡縮
//			D3DXMATRIX			matScaling;
//			D3DXMatrixScaling(&matScaling, 0.016f, 0.016f, 0.016f);
//			D3DXMatrixMultiply(&matWorld, &matWorld, &matScaling);
//
//			//回転
//			D3DXMATRIX			matPitching;
//			D3DXMatrixRotationY(&matPitching, D3DXToRadian(20.0f*renderCount + cartridgeRenderRand));//180.3f真後ろに来る角度
//			D3DXMatrixMultiply(&matWorld, &matWorld, &matPitching);
//
//			//移動
//			D3DXMATRIX			matPosition;
//			D3DXMatrixTranslation(&matPosition,
//				0.097f + (renderCount - 1) * 0.01f + cartridgeRenderRand * 0.0001f,
//				-0.0505f + (renderCount - 1) * 0.001f + cartridgeRenderRand * 0.0001f,
//				0.113f);
//
//			D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
//
//			rCamera.NegateView(&matWorld);
//			rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//		}
//
//		if (0 < renderCount&&renderCount < 5)
//		{
//			++renderCount;
//
//			cartridgeRenderRand = rand() % 10;
//		}
//
//		rpDirectX3DDevice->SetTexture(0, textures[_T("cartridge")]);
//
//		if (renderCount >= SHOT_RENDER_TIME)
//		{
//			renderCount = 0;
//		}
//
//		cartridgeFbx.m_pModel[0]->DrawFbx();
//	}
//
//	for (INT i = 0; i < cubeFbx.m_modelDataCount; ++i)
//	{
//		D3DXMATRIX matWorld, matTrans, matRotX, matRotY, matRotZ, matScale;
//		D3DXMatrixIdentity(&matWorld);
//		D3DXMatrixIdentity(&matTrans);
//		D3DXMatrixIdentity(&matScale);
//
//		// 拡大
//		D3DXMatrixScaling(&matScale, 0.08f, 0.08f, 0.08f);
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
//
//		// 回転
//		D3DXMatrixRotationX(&matRotX, D3DXToRadian(0.0f));
//		D3DXMatrixRotationY(&matRotY, D3DXToRadian(0.0f));
//		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.0f));
//
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotY);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotX);
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotZ);
//
//		// 移動
//		D3DXMatrixTranslation(&matTrans, 0.0f, -0.0f, 1.0f);
//
//		// 掛け合わせ
//		D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
//
//		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//		rpDirectX3DDevice->SetTexture(0, textures[_T("redTex")]);
//
//		cubeFbx.m_pModel[i]->DrawFbx();
//	}
//
//	//銃を撃った時の演出カウント
//	{
//		if (shotRenderCount)
//		{
//			++shotRenderCount;
//		}
//
//		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonHold[0])
//		{
//			shotRenderCount = SHOT_RENDER_TIME - 2;
//		}
//
//		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonPush[0])
//		{
//			shotRenderCount = SHOT_RENDER_TIME - 2;
//		}
//
//		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonRelease[0])
//		{
//			shotRenderCount = 0;
//		}
//
//		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonUninput[0])
//		{
//			shotRenderCount = 0;
//		}
//	}
//
//	return;
//}

//Player::Player(const char* pPlayerModelPath)
//{
//	DirectX* pDirectX = DirectX::GetInstance();
//	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
//	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;
//
//	m_pCamera = &pDirectX->m_DirectX3DDevice.m_camera;
//
//	LPDIRECTINPUTDEVICE8* pDirectXInputDevices = rDirectXInstances.m_pDirectXInputDevices;
//
//	m_pInputData = &pDirectX->m_DirectXInputDevices.m_InputData;
//
//	m_playerFbx.LoadFbx(pPlayerModelPath);
//}
//
//VOID Player::GetPlayerMoveRad(D3DXVECTOR2 prevPlayerPos)
//{
//	D3DXVECTOR2 playerMoveVec(0.0f, 0.0f);
//
//	D3DXVECTOR2 playerPos(m_pos.z, m_pos.x);
//
//	UnitVec2(&playerMoveVec, playerPos, prevPlayerPos);
//
//	D3DXVECTOR2 defaultVec(0.0f, 10.0f);
//
//	m_eulerXYZ.y = Vec2Rad(playerMoveVec, defaultVec);
//
//	return;
//}
//
//VOID Player::Hover()
//{
//	MouseState& rMouseState = m_pInputData->m_mouseState;
//	KeyBoardState& rKeyState = m_pInputData->m_keyBoardState;
//
//	const FLOAT HOVER_SPEED = 0.01f;
//
//	const FLOAT GRAVITY = -9.8f;
//
//	static INT fallCount = 0;
//
//	const INT FPS = 60;
//
//	if (rKeyState.m_keyHold[DIK_SPACE])
//	{
//		m_pos.y += HOVER_SPEED;
//
//		fallCount = 0;
//	};
//
//	if (m_pos.y)
//	{
//		m_pos.y += (FLOAT)(GRAVITY * pow(fallCount, 2) / FPS);
//	}
//
//	if (m_pos.y <= 0)
//	{
//		m_pos.y = 0;
//
//		fallCount = 0;
//	}
//
//	++fallCount;
//
//	return;
//}
//
//VOID Player::Move()
//{
//	MouseState& rMouseState = m_pInputData->m_mouseState;
//	KeyBoardState& rKeyState = m_pInputData->m_keyBoardState;
//
//	D3DXVECTOR3 cameraPos;
//	m_pCamera->GetCameraPos(&cameraPos);
//	D3DXVECTOR2 frontBaseOrigin(cameraPos.x, cameraPos.z);
//
//	D3DXVECTOR3 cameraEyePt;
//	m_pCamera->GetEyePoint(&cameraEyePt);
//	D3DXVECTOR2 frontBaseDest(cameraEyePt.x, cameraEyePt.z);
//
//	D3DXVECTOR2 frontUnitVec(0.0f, 0.0f);
//	UnitVec2(&frontUnitVec, frontBaseDest, frontBaseOrigin);			//Y座標を考慮しない前方単位ベクトルを作成する
//
//	D3DXVECTOR2 rightUnitVec(0.0f, 0.0f);
//	NormalUnitVec2Y(&rightUnitVec, frontUnitVec);						//Y座標を考慮しない右方単位ベクトルを作成する
//
//	FLOAT speedMultiply = 0.3f;
//
//	if (rKeyState.m_keyHold[DIK_LCONTROL])
//	{
//		speedMultiply = 0.1f;
//	}
//
//	if (rKeyState.m_keyHold[DIK_LSHIFT] && !rKeyState.m_keyHold[DIK_LCONTROL])
//	{
//		speedMultiply = 1.3f;
//	}
//
//	if (rKeyState.m_keyHold[DIK_W])
//	{
//		cameraPos.z += frontUnitVec.y*speedMultiply;
//		m_pos.z += frontUnitVec.y*speedMultiply;
//
//		cameraPos.x += frontUnitVec.x*speedMultiply;
//		m_pos.x += frontUnitVec.x*speedMultiply;
//	}
//
//	if (rKeyState.m_keyHold[DIK_S])
//	{
//		cameraPos.z -= frontUnitVec.y*speedMultiply;
//		m_pos.z -= frontUnitVec.y*speedMultiply;
//
//		cameraPos.x -= frontUnitVec.x*speedMultiply;
//		m_pos.x -= frontUnitVec.y*speedMultiply;
//
//		frontUnitVec *= -1.0f;
//	}
//
//	if (rKeyState.m_keyHold[DIK_D])
//	{
//		cameraPos.z += rightUnitVec.y*speedMultiply;
//		m_pos.z += rightUnitVec.y*speedMultiply;
//
//		cameraPos.x += rightUnitVec.x*speedMultiply;
//		m_pos.x += rightUnitVec.x*speedMultiply;
//	}
//
//	if (rKeyState.m_keyHold[DIK_A])
//	{
//		cameraPos.z -= rightUnitVec.y*speedMultiply;
//		m_pos.z -= rightUnitVec.y*speedMultiply;
//
//		cameraPos.x -= rightUnitVec.x*speedMultiply;
//		m_pos.x -= rightUnitVec.x*speedMultiply;
//
//		rightUnitVec *= -1.0f;
//	}
//
//	return;
//}
//
//VOID Player::Control()
//{
//	D3DXVECTOR2 prevPlayerPos(m_pos.z, m_pos.x);
//
//	Move();
//
//	Hover();
//
//	GetPlayerMoveRad(prevPlayerPos);
//}
//
//VOID Player::Render()
//{
//	D3DXMATRIX  matWorld;
//	D3DXMatrixIdentity(&matWorld);
//
//	D3DXMATRIX  matScaling;
//	D3DXMatrixScaling(&matScaling, m_scale.x, m_scale.y, m_scale.z);
//
//	D3DXMatrixMultiply(&matWorld, &matWorld, &matScaling);
//
//	D3DXMATRIX matRotateX;
//	D3DXMatrixRotationX(&matRotateX, m_eulerXYZ.x);
//
//	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotateX);
//
//	D3DXMATRIX matRotateY;
//	D3DXMatrixRotationY(&matRotateY, m_eulerXYZ.y);
//
//	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotateY);
//
//	D3DXMATRIX matRotateZ;
//	D3DXMatrixRotationZ(&matRotateZ, m_eulerXYZ.z);
//
//	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotateZ);
//
//	D3DXMATRIX matMove;
//	D3DXMatrixTranslation(&matMove, m_pos.x, m_pos.y, m_pos.z);
//
//	D3DXMatrixMultiply(&matWorld, &matWorld, &matMove);
//}