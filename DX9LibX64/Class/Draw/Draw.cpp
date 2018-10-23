#include <windows.h>
#include <vector>
#include "Draw.h"
#include "../FBX/FbxRelated.h"
#include "../CustomVertices/CustomVertices.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

Draw* Draw::m_pDraw = NULL;
LPDIRECT3DDEVICE9 Draw::m_pDirectX3DDevice = NULL;

Draw* Draw::GetInstance()
{
	if (!m_pDraw)m_pDraw = new Draw;

	return m_pDraw;
}

VOID Draw::DeleteInstance()
{
	delete m_pDraw;
	m_pDraw = NULL;
}

VOID Draw::SetLPDIRECT3DDEVICE9(LPDIRECT3DDEVICE9 pDirectX3DDevice)
{
	m_pDirectX3DDevice = pDirectX3DDevice;
}

VOID Draw::Render(const Custom3DVertex* pCustom3DVertices, const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pDirectX3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDirectX3DDevice->SetTransform(D3DTS_WORLD, pMatWorld);
	m_pDirectX3DDevice->SetTexture(0, pTexture);
	m_pDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pCustom3DVertices, sizeof(Custom3DVertex));
}

VOID Draw::Render(const CustomVertex* pCustomVertices, const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pDirectX3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDirectX3DDevice->SetTexture(0, pTexture);
	m_pDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pCustomVertices, sizeof(CustomVertex));
}

VOID Draw::Render(const FbxRelated* pFBXModel, const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pDirectX3DDevice->SetTransform(D3DTS_WORLD, pMatWorld);

	m_pDirectX3DDevice->SetTexture(0, pTexture);

	for (FbxModel* pI : pFBXModel->m_pModel)
	{
		pI->DrawFbx();
	}
}
