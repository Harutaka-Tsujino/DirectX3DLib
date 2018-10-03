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
#include <crtdbg.h>

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
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window* pWindow = Window::GetInstance(hInst, TEXT("TestApp"));

	pWindow->SetWindowMode(TRUE);

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

//3Dの頂点フォーマット
struct Vertex3D
{
public:
	float m_x;
	float m_y;
	float m_z;
	DWORD m_aRGB;
	float m_tu;
	float m_tv;
};

VOID MainFunc()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;
	Camera& rCamera = pDirectX->m_DirectX3DDevice.m_camera;
	MouseState& rMouseState = pDirectX->m_DirectXInputDevices.m_InputData.m_mouseState;

	const INT INIT_FRAME = -1;

	static INT frameCount = INIT_FRAME;

	//FBXインスタンスの生成
	static FbxRelated mP7;

	static FbxRelated cartridgeFbx;

	static FbxRelated boardFbx;

	static std::map<TCHAR*, LPDIRECT3DTEXTURE9> textures;

	//モデルや画像の読み込み
	if (frameCount == INIT_FRAME)
	{
		//FBXモデルの読み込み
		mP7.LoadFbx("MP7.fbx");

		cartridgeFbx.LoadFbx("Cartridge.45ACP/SA_45ACP_Example.fbx");

		boardFbx.LoadFbx("boad.fbx");

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

		textures[_T("mazulFlash0")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/mazulFlash0.png"),
			&textures[_T("mazulFlash0")]);

		textures[_T("mazulFlash1")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/mazulFlash1.png"),
			&textures[_T("mazulFlash1")]);

		textures[_T("mazulFlash2")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/mazulFlash2.png"),
			&textures[_T("mazulFlash2")]);

		textures[_T("mazulFlash3")] = nullptr;

		D3DXCreateTextureFromFile(rpDirectX3DDevice,
			TEXT("textures/mazulFlash3.png"),
			&textures[_T("mazulFlash3")]);

		frameCount = 0;
	}

	//2Dの描画
	{
		rpDirectX3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		CustomVertex backVertices[4];

		CustomImageVerticies(backVertices, 1920.0f*0.5f, 1080.0f*0.5f, 200.0f,
			1920.0f*0.5f, 1080.0f*0.5f,
			0xFFFFFFFF,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		rpDirectX3DDevice->SetTexture(0, textures[_T("testBack")]);

		//rpDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backVertices, sizeof(CustomVertex));


		static float diffusionValue = 1.0f;
		static int diffusionCaunt = 1;

		diffusionValue = 0.6f*(float)pow(diffusionCaunt, 2);

		if (rMouseState.m_buttonHold[0])
		{
			++diffusionCaunt;
		}

		if (rMouseState.m_buttonUninput[0])
		{
			diffusionCaunt -= 1;
		}

		CustomVertex reticleLengthVertices[4];

		const INT RETICLE_LENGTH_COUNT = 2;

		diffusionCaunt = max(min(diffusionCaunt, 27), 1);

		for (int i = 0; i < RETICLE_LENGTH_COUNT; ++i)
		{
			CustomImageVerticies(reticleLengthVertices, 1920.0f*0.5f, 1080.0f*(0.46f + (i*0.08f)) + ((i == 0) ? -diffusionValue : diffusionValue), 0.0f,
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
			CustomImageVerticies(reticleWidthVertices, (1920.0f*0.5f + 1080.0f*0.04f*((i == 0) ? -1.0f : 1.0f)) + ((i == 0) ? -diffusionValue : diffusionValue), 1080.0f*0.50f, 0.0f,
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
	}

	//銃を撃った時の演出フレーム
	static const int SHOT_RENDER_TIME = 5;

	//演出を測るカウント
	static int shotRenderCount = 0;

	if (rMouseState.m_buttonHold[0] && !shotRenderCount)
	{
		shotRenderCount = 1;
	}

	pDirectX->m_DirectX3DDevice.m_camera.SetTransform();

	//ライトの設定
	{
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
	}

	rpDirectX3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//ビルボードの描画
	if (shotRenderCount)
	{
		rpDirectX3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		const int RECT_VERTICES_NUM = 4;

		LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;

		rpDirectX3DDevice->CreateVertexBuffer(
			4 * sizeof(Vertex3D),
			D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
			D3DPOOL_MANAGED,
			&vertexBuffer,
			NULL);

		Vertex3D* billBoard;
		vertexBuffer->Lock(0, 0, (void**)&billBoard, 0);

		for (int i = 0; i < RECT_VERTICES_NUM; ++i)
		{
			billBoard[i].m_aRGB = 0xFFFFCC11;
		}

		billBoard[0].m_x = -5.0f;
		billBoard[0].m_y = 5.0f;
		billBoard[0].m_z = 0.0f;
		billBoard[0].m_tu = 0.0f;
		billBoard[0].m_tv = 0.0f;

		billBoard[1].m_x = 5.0f;
		billBoard[1].m_y = 5.0f;
		billBoard[1].m_z = 0.0f;
		billBoard[1].m_tu = 1.0f;
		billBoard[1].m_tv = 0.0f;

		billBoard[2].m_x = 5.0f;
		billBoard[2].m_y = -5.0f;
		billBoard[2].m_z = 0.0f;
		billBoard[2].m_tu = 1.0f;
		billBoard[2].m_tv = 1.0f;

		billBoard[3].m_x = -5.0f;
		billBoard[3].m_y = -5.0f;
		billBoard[3].m_z = 0.0f;
		billBoard[3].m_tu = 0.0f;
		billBoard[3].m_tv = 1.0f;

		D3DXMATRIX mat_world, mat_trans, mat_rotx, mat_roty, mat_rotz, mat_scale;
		D3DXMatrixIdentity(&mat_world);
		D3DXMatrixIdentity(&mat_trans);
		D3DXMatrixIdentity(&mat_scale);

		// 拡大
		D3DXMatrixScaling(&mat_scale, 0.007f, 0.007f, 0.007f);

		D3DXMatrixMultiply(&mat_world, &mat_world, &mat_scale);

		// 回転
		D3DXMatrixRotationX(&mat_rotx, D3DXToRadian(0.0f));
		D3DXMatrixRotationY(&mat_roty, D3DXToRadian(0.0f));
		D3DXMatrixRotationZ(&mat_rotz, D3DXToRadian(0.0f));

		D3DXMatrixMultiply(&mat_world, &mat_world, &mat_roty);
		D3DXMatrixMultiply(&mat_world, &mat_world, &mat_rotx);
		D3DXMatrixMultiply(&mat_world, &mat_world, &mat_rotz);

		// 移動
		D3DXMatrixTranslation(&mat_trans, 0.047f, -0.027f, 0.132f);

		// 掛け合わせ
		D3DXMatrixMultiply(&mat_world, &mat_world, &mat_trans);

		//ビューの逆行列をかけてビルボード化
		rCamera.NegateView(&mat_world);

		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &mat_world);

		vertexBuffer->Unlock();

		rpDirectX3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex3D));

		switch (shotRenderCount)
		{
		case 1:

			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash0")]);
			break;
		case 2:

			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash1")]);
			break;
		case 3:

			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash2")]);
			break;
		case 4:

			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash3")]);
			break;
		case 5:

			rpDirectX3DDevice->SetTexture(0, textures[_T("mazulFlash1")]);
			break;
		}

		rpDirectX3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		vertexBuffer->Release();
		vertexBuffer = NULL;

		rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		rpDirectX3DDevice->SetTexture(0, NULL);
	}

	//MP7A1の描画
	for (int i = 0; i < mP7.m_modelDataCount; ++i)
	{
		D3DXMATRIX			matWorld;
		D3DXMatrixIdentity(&matWorld);

		D3DXMATRIX			matScal;
		D3DXMatrixScaling(&matScal, 0.003f, 0.003f, 0.003f);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matScal);

		D3DXMATRIX			matRoll;
		D3DXMatrixRotationZ(&matRoll, 0.0f * (D3DX_PI / 180.f));
		D3DXMatrixMultiply(&matWorld, &matWorld, &matRoll);

		D3DXMATRIX			matPitch;
		D3DXMatrixRotationX(&matPitch, 0.0f * (D3DX_PI / 180.f));
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPitch);

		D3DXMATRIX			matYaw;
		D3DXMatrixRotationY(&matYaw, 180.0f * (D3DX_PI / 180.0f));
		D3DXMatrixMultiply(&matWorld, &matWorld, &matYaw);

		//反動の表現
		D3DXMATRIX			matGunShock;
		D3DXMatrixRotationX(&matGunShock, -2.2f*shotRenderCount * (D3DX_PI / 180.f));
		D3DXMatrixMultiply(&matWorld, &matWorld, &matGunShock);

		static int moveXMouse;

		moveXMouse += rMouseState.m_mouseState.lX;

		//視点変更
		D3DXMatrixRotationY(&matYaw, moveXMouse* 0.05f * (D3DX_PI / 180.0f));
		D3DXMatrixMultiply(&matWorld, &matWorld, &matYaw);

		D3DXVECTOR3 gunPos(0.05f, -0.06f, 0.1f - shotRenderCount * 0.005f);

		D3DXMATRIX			matLookPtMove;

		D3DXMatrixRotationY(&matLookPtMove, moveXMouse* 0.05f * (D3DX_PI / 180.f));

		D3DXVec3TransformCoord(&gunPos, &gunPos, &matLookPtMove);

		D3DXVECTOR3 cameraPos;

		rCamera.GetCameraPos(&cameraPos);

		D3DXVec3Add(&gunPos, &gunPos, &cameraPos);

		D3DXMATRIX			matPosition;	// 位置座標行列
		D3DXMatrixTranslation(&matPosition, gunPos.x, gunPos.y, gunPos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
		rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		rpDirectX3DDevice->SetTexture(0, textures[_T("matBlack")]);

		mP7.m_pModel[i]->DrawFbx();
	}

	//薬莢の描画
	{
		D3DXMATRIX			matWorld;
		D3DXMatrixIdentity(&matWorld);

		static int renderCount = 0;

		static int cartridgeRenderRand = 0;

		if (rMouseState.m_buttonHold[0] && !renderCount)
		{
			renderCount = 1;
		}

		if (renderCount)
		{
			//拡縮
			D3DXMATRIX			matScaling;
			D3DXMatrixScaling(&matScaling, 0.016f, 0.016f, 0.016f);
			D3DXMatrixMultiply(&matWorld, &matWorld, &matScaling);

			//回転
			D3DXMATRIX			matPitching;
			D3DXMatrixRotationY(&matPitching, D3DXToRadian(20.0f*renderCount + cartridgeRenderRand));//180.3f真後ろに来る角度
			D3DXMatrixMultiply(&matWorld, &matWorld, &matPitching);

			//移動
			D3DXMATRIX			matPosition;
			D3DXMatrixTranslation(&matPosition,
				0.097f + (renderCount - 1) * 0.01f + cartridgeRenderRand * 0.0001f,
				-0.0505f + (renderCount - 1) * 0.001f + cartridgeRenderRand * 0.0001f,
				0.113f);

			D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

			rCamera.NegateView(&matWorld);
			rpDirectX3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		}

		if (0 < renderCount&&renderCount < 5)
		{
			++renderCount;

			cartridgeRenderRand = rand() % 10;
		}

		rpDirectX3DDevice->SetTexture(0, textures[_T("cartridge")]);

		if (renderCount >= SHOT_RENDER_TIME)
		{
			renderCount = 0;
		}

		cartridgeFbx.m_pModel[0]->DrawFbx();
	}

	//銃を撃った時の演出カウント
	{
		if (shotRenderCount)
		{
			++shotRenderCount;
		}

		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonHold[0])
		{
			shotRenderCount = SHOT_RENDER_TIME - 2;
		}

		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonPush[0])
		{
			shotRenderCount = SHOT_RENDER_TIME - 2;
		}

		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonRelease[0])
		{
			shotRenderCount = 0;
		}

		if (shotRenderCount > SHOT_RENDER_TIME&&rMouseState.m_buttonUninput[0])
		{
			shotRenderCount = 0;
		}
	}

	return;
}
