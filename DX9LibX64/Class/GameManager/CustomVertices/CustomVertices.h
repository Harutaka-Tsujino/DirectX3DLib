#pragma once
#include <windows.h>
#include <d3dx9.h>
#include "../Object/Object.h"

struct CustomVertex
{
public:
	D3DXVECTOR3 m_pos;
	FLOAT m_rHW = 1.0f;
	DWORD m_aRGB;
	D3DXVECTOR2 m_TexUV;
};

class CustomVertices
{
public:
	static CustomVertices* GetInstance();

	VOID deleteInstance();

	VOID RotateXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3* pDegree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateX(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateY(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateZ(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID Rescale(CustomVertex* pCustomVertices, const D3DXVECTOR2* pScaleRate);

	VOID Move(CustomVertex* pCustomVertices, const D3DXVECTOR3* pMovement);

	VOID Locale(CustomVertex* pCustomVertices, const D3DXVECTOR3* pCenter);

	VOID SetTexUV(CustomVertex* pCustomVertices, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f);

	VOID Create(CustomVertex *pCustomVertices, const D3DXVECTOR3* pCenter, const D3DXVECTOR2* pHalfScale,
		DWORD color = 0xFFFFFFFF, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f);

private:
	CustomVertices() {};
	~CustomVertices() {};

	static CustomVertices* m_pCustomVertices;

	static const INT M_RECT_VERTICES_NUM = 4;
};
