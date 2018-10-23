#include <d3dx9.h>
#include "CustomVertices.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

CustomVertices* CustomVertices::m_pCustomVertices = NULL;

CustomVertices* CustomVertices::GetInstance()
{
	if (!m_pCustomVertices)m_pCustomVertices = new CustomVertices;

	return m_pCustomVertices;
}

VOID CustomVertices::DeleteInstance()
{
	delete m_pCustomVertices;
	m_pCustomVertices = NULL;
}

VOID CustomVertices::Rotate(CustomVertex* pCustomVertices, const D3DXVECTOR3* pRelativeRotateCenter, const D3DXMATRIX* pMatRotate)
{
	D3DXVECTOR3 rectCenter(
		(pCustomVertices[0].m_pos.x + pCustomVertices[2].m_pos.x) * 0.5f,
		(pCustomVertices[0].m_pos.y + pCustomVertices[2].m_pos.y) * 0.5f,
		(pCustomVertices[0].m_pos.z + pCustomVertices[2].m_pos.z) * 0.5f);

	D3DXVECTOR3 distancesFromRectCenter[M_RECT_VERTICES_NUM];

	for (INT i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		distancesFromRectCenter[i].x = pCustomVertices[i].m_pos.x - rectCenter.x;
		distancesFromRectCenter[i].y = pCustomVertices[i].m_pos.y - rectCenter.y;
		distancesFromRectCenter[i].z = pCustomVertices[i].m_pos.z - rectCenter.z;
	};

	D3DXVECTOR3 movedPosesToRotation[M_RECT_VERTICES_NUM];

	for (INT i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		movedPosesToRotation[i].x = distancesFromRectCenter[i].x - pRelativeRotateCenter->x;
		movedPosesToRotation[i].y = distancesFromRectCenter[i].y - pRelativeRotateCenter->y;
		movedPosesToRotation[i].z = distancesFromRectCenter[i].z - pRelativeRotateCenter->z;
	};

	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		D3DXVec3TransformCoord(&pCustomVertices[i].m_pos, &movedPosesToRotation[i], pMatRotate);
		D3DXVec3Add(&pCustomVertices[i].m_pos, &pCustomVertices[i].m_pos, &rectCenter);
		D3DXVec3Add(&pCustomVertices[i].m_pos, &pCustomVertices[i].m_pos, pRelativeRotateCenter);
	}
}

VOID CustomVertices::RotateXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3* pDegree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	RotateX(pCustomVertices, pDegree->x, pRelativeRotateCenter);
	RotateY(pCustomVertices, pDegree->y, pRelativeRotateCenter);
	RotateZ(pCustomVertices, pDegree->z, pRelativeRotateCenter);
}

VOID CustomVertices::RotateX(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationX(&matRotate, D3DXToRadian(degree));

	Rotate(pCustomVertices, pRelativeRotateCenter, &matRotate);
}

VOID CustomVertices::RotateY(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(degree));

	Rotate(pCustomVertices, pRelativeRotateCenter, &matRotate);
}

VOID CustomVertices::RotateZ(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(degree));

	Rotate(pCustomVertices, pRelativeRotateCenter, &matRotate);
}

VOID CustomVertices::Rotate(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pRelativeRotateCenter, const D3DXMATRIX* pMatRotate)
{
	D3DXVECTOR3 rectCenter(
		(pCustom3DVertices[0].m_pos.x + pCustom3DVertices[2].m_pos.x) * 0.5f,
		(pCustom3DVertices[0].m_pos.y + pCustom3DVertices[2].m_pos.y) * 0.5f,
		(pCustom3DVertices[0].m_pos.z + pCustom3DVertices[2].m_pos.z) * 0.5f);

	D3DXVECTOR3 distancesFromRectCenter[M_RECT_VERTICES_NUM];

	for (INT i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		distancesFromRectCenter[i].x = pCustom3DVertices[i].m_pos.x - rectCenter.x;
		distancesFromRectCenter[i].y = pCustom3DVertices[i].m_pos.y - rectCenter.y;
		distancesFromRectCenter[i].z = pCustom3DVertices[i].m_pos.z - rectCenter.z;
	};

	D3DXVECTOR3 movedPosesToRotation[M_RECT_VERTICES_NUM];

	for (INT i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		movedPosesToRotation[i].x = distancesFromRectCenter[i].x - pRelativeRotateCenter->x;
		movedPosesToRotation[i].y = distancesFromRectCenter[i].y - pRelativeRotateCenter->y;
		movedPosesToRotation[i].z = distancesFromRectCenter[i].z - pRelativeRotateCenter->z;
	};

	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		D3DXVec3TransformCoord(&pCustom3DVertices[i].m_pos, &movedPosesToRotation[i], pMatRotate);
		D3DXVec3Add(&pCustom3DVertices[i].m_pos, &pCustom3DVertices[i].m_pos, &rectCenter);
		D3DXVec3Add(&pCustom3DVertices[i].m_pos, &pCustom3DVertices[i].m_pos, pRelativeRotateCenter);
	}
}

VOID CustomVertices::RotateXYZ(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pDegree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	RotateX(pCustom3DVertices, pDegree->x, pRelativeRotateCenter);
	RotateY(pCustom3DVertices, pDegree->y, pRelativeRotateCenter);
	RotateZ(pCustom3DVertices, pDegree->z, pRelativeRotateCenter);
}

VOID CustomVertices::RotateX(Custom3DVertex* pCustom3DVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationX(&matRotate, D3DXToRadian(degree));

	Rotate(pCustom3DVertices, pRelativeRotateCenter, &matRotate);
}

VOID CustomVertices::RotateY(Custom3DVertex* pCustom3DVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(degree));

	Rotate(pCustom3DVertices, pRelativeRotateCenter, &matRotate);
}

VOID CustomVertices::RotateZ(Custom3DVertex* pCustom3DVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter)
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(degree));

	Rotate(pCustom3DVertices, pRelativeRotateCenter, &matRotate);
}

VOID CustomVertices::Rescale(CustomVertex* pCustomVertices, const D3DXVECTOR2* pScaleRate)
{
	D3DXVECTOR2 rectCenter(
		(pCustomVertices[0].m_pos.x + pCustomVertices[2].m_pos.x) * 0.5f,
		(pCustomVertices[0].m_pos.y + pCustomVertices[2].m_pos.y) * 0.5f);

	D3DXVECTOR2 distancesFromRectCenter[M_RECT_VERTICES_NUM];

	for (INT i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		distancesFromRectCenter[i].x = pCustomVertices[i].m_pos.x - rectCenter.x;
		distancesFromRectCenter[i].y = pCustomVertices[i].m_pos.y - rectCenter.y;
	};

	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustomVertices[i].m_pos.x = pScaleRate->x*distancesFromRectCenter[i].x + rectCenter.x;
		pCustomVertices[i].m_pos.y = pScaleRate->y*distancesFromRectCenter[i].y + rectCenter.y;
	}
}

VOID CustomVertices::Rescale(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR2* pScaleRate)
{
	D3DXVECTOR2 rectCenter(
		(pCustom3DVertices[0].m_pos.x + pCustom3DVertices[2].m_pos.x) * 0.5f,
		(pCustom3DVertices[0].m_pos.y + pCustom3DVertices[2].m_pos.y) * 0.5f);

	D3DXVECTOR2 distancesFromRectCenter[M_RECT_VERTICES_NUM];

	for (INT i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		distancesFromRectCenter[i].x = pCustom3DVertices[i].m_pos.x - rectCenter.x;
		distancesFromRectCenter[i].y = pCustom3DVertices[i].m_pos.y - rectCenter.y;
	};

	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustom3DVertices[i].m_pos.x = pScaleRate->x*distancesFromRectCenter[i].x + rectCenter.x;
		pCustom3DVertices[i].m_pos.y = pScaleRate->y*distancesFromRectCenter[i].y + rectCenter.y;
	}
}

VOID CustomVertices::Move(CustomVertex* pCustomVertices, const D3DXVECTOR3* pMovement)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		D3DXVec3Add(&pCustomVertices[i].m_pos, &pCustomVertices[i].m_pos, pMovement);
	}
}

VOID CustomVertices::Move(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pMovement)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		D3DXVec3Add(&pCustom3DVertices[i].m_pos, &pCustom3DVertices[i].m_pos, pMovement);
	}
}

VOID CustomVertices::Locale(CustomVertex* pCustomVertices, const D3DXVECTOR3* pPos)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustomVertices[i].m_pos = *pPos;
	}
}

VOID CustomVertices::Locale(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pPos)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustom3DVertices[i].m_pos = *pPos;
	}
}

VOID CustomVertices::SetTexUV(CustomVertex* pCustomVertices, FLOAT startTU, FLOAT startTV, FLOAT endTU, FLOAT endTV)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustomVertices[i].m_TexUV.x = (i % 3) ? endTU : startTU;
		pCustomVertices[i].m_TexUV.y = (i / 2) ? endTV : startTV;
	}
}

VOID CustomVertices::SetTexUV(Custom3DVertex* pCustom3DVertices, FLOAT startTU, FLOAT startTV, FLOAT endTU, FLOAT endTV)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustom3DVertices[i].m_TexUV.x = (i % 3) ? endTU : startTU;
		pCustom3DVertices[i].m_TexUV.y = (i / 2) ? endTV : startTV;
	}
}

VOID CustomVertices::Create(CustomVertex* pCustomVertices, const D3DXVECTOR3* pCenter, const D3DXVECTOR2* pHalfScale,
	DWORD color, FLOAT startTU, FLOAT startTV, FLOAT endTU, FLOAT endTV)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustomVertices[i].m_pos = *pCenter;
		pCustomVertices[i].m_pos.x += (i % 3) ? pHalfScale->x : -(pHalfScale->x);
		pCustomVertices[i].m_pos.y += (i / 2) ? pHalfScale->y : -(pHalfScale->y);

		pCustomVertices[i].m_aRGB = color;
	}

	SetTexUV(pCustomVertices, startTU, startTV, endTU, endTV);
}

VOID CustomVertices::Create(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pCenter, const D3DXVECTOR2* pHalfScale,
	DWORD color, FLOAT startTU, FLOAT startTV, FLOAT endTU, FLOAT endTV)
{
	for (int i = 0; i < M_RECT_VERTICES_NUM; ++i)
	{
		pCustom3DVertices->m_pos = *pCenter;
		pCustom3DVertices[i].m_pos.x += (i % 3) ? pHalfScale->x : -(pHalfScale->x);
		pCustom3DVertices[i].m_pos.x += (i / 2) ? pHalfScale->y : -(pHalfScale->y);

		pCustom3DVertices[i].m_aRGB = 0XFFFFFFFF;
	}

	SetTexUV(pCustom3DVertices, startTU, startTV, endTU, endTV);
}
