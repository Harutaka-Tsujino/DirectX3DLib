#include <windows.h>
#include <vector>
#include "Draw.h"
#include "../Object/Object.h"
#include "../GameManager.h"

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

VOID Draw::Render(const CustomVertex* pCustomVertices, const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pDirectX3DDevice->SetTransform(D3DTS_WORLD, pMatWorld);

	Render(pCustomVertices, pTexture);
}

VOID Draw::Render(const CustomVertex* pCustomVertices, const LPDIRECT3DTEXTURE9 pTexture)
{
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
