#include <Windows.h>
#include <d3dx9.h>
#include "../../../GameManager/GameManager.h"
#include "../../../FBX/FbxRelated.h"
#include "../../../CustomVertices/CustomVertices.h"
#include "../../../Draw/Draw.h"
#include "TitleScene.h"

VOID TitleScene::Update()
{
	static int frame = -1;

	D3DXVECTOR3 vecDirection(-0.5f, -1.0f, 1.0f);
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.5f;
	light.Diffuse.g = 1.5f;
	light.Diffuse.b = 1.5f;

	light.Specular.r = 10.0f;
	light.Specular.b = 10.0f;
	light.Specular.g = 10.0f;

	light.Ambient.r = 10.f;
	light.Ambient.b = 10.f;
	light.Ambient.g = 10.f;

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);

	light.Range = 1.f;
	LPDIRECT3DDEVICE9 pDirectX3DDevice = m_pGameManager->GetDirectX3DDevice();
	pDirectX3DDevice->SetLight(0, &light);
	pDirectX3DDevice->LightEnable(0, TRUE);

	static FbxRelated starFBX;
	static LPDIRECT3DTEXTURE9 pTexture;

	if (frame == -1)
	{
		starFBX.LoadFbx("3DModels/Eiwi/untitled.fbx");

		D3DXCreateTextureFromFile(pDirectX3DDevice,
			_T("2DTextures/circle.png"),
			&pTexture);

		++frame;
	}

	D3DXVECTOR2 displaySize;
	m_pGameManager->GetDisplaySize(&displaySize);

	CustomVertex background[4];

	D3DXVECTOR2 halfBackgroundSize;
	D3DXVECTOR3 backgroundCenter(0.0f,0.0f,1.0f);

	for (int n = 0; n < 32; ++n)
	{
		halfBackgroundSize.x = ((-n)+(frame/3))* 2.0f;
		halfBackgroundSize.y = halfBackgroundSize.x;

		backgroundCenter.x = displaySize.x/32*(float)(n+1);

			for (int i = 0; i < 18; ++i)
			{
				backgroundCenter.y = displaySize.y /18* (float)(i+1);
				m_pCustomVertices->Create(background, &backgroundCenter, &halfBackgroundSize,0xFFDDDD11);

				m_pDraw->Render(background, pTexture);
			}
	}

	D3DXVECTOR4 eiwiEmissive(0.0f, 0.3f, 0.3f, 0.01f);
	starFBX.SetEmissive(&eiwiEmissive);

	D3DXMATRIX			matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIX			matScal;
	D3DXMatrixScaling(&matScal, 1.06f, 1.06f, 1.06f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matScal);

	D3DXMATRIX			matPitch;
	D3DXMatrixRotationX(&matPitch, D3DXToRadian(frame*0.0f));
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPitch);

	D3DXMATRIX			matYaw;
	D3DXMatrixRotationY(&matYaw, D3DXToRadian(frame*3.0f));
	D3DXMatrixMultiply(&matWorld, &matWorld, &matYaw);

	D3DXMATRIX			matRoll;
	D3DXMatrixRotationZ(&matRoll, D3DXToRadian(frame*3.0f));
	D3DXMatrixMultiply(&matWorld, &matWorld, &matRoll);

	D3DXVECTOR3 matPos(0.0f, -0.0f, 4.0f);

	D3DXMATRIX			matPosition;	// ˆÊ’uÀ•Ws—ñ
	D3DXMatrixTranslation(&matPosition, matPos.x, matPos.y, matPos.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

	m_pDraw->Render(&starFBX, &matWorld, NULL);

	static bool canBack = false;

	if (canBack)
	{
		--frame;
	}

	if (frame <0)
	{
		canBack = false;
	}

	if (!canBack)
	{
		++frame;
	}

	if (frame > 90)
	{
		canBack = true;
	}
}

VOID TitleScene::Render()
{
}
