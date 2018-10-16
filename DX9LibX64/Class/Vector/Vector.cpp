#include <windows.h>	
#include <d3dx9.h>
#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace CalcVector
{
	VOID UnitVec2(D3DXVECTOR2* pStraightUnitVec, D3DXVECTOR2 destPt, D3DXVECTOR2 oriPt)
	{
		D3DXVECTOR2 straightVec(0.0f, 0.0f);

		straightVec.x = oriPt.x - destPt.x;
		straightVec.y = oriPt.y - destPt.y;

		D3DXVec2Normalize(pStraightUnitVec, &straightVec);
	}

	VOID NormalUnitVec2Y(D3DXVECTOR2* pNormalUnitVec, D3DXVECTOR2 baseVec)
	{
		D3DXVECTOR2 unitBaseVec(0.0f, 0.0f);

		D3DXVec2Normalize(&unitBaseVec, &baseVec);

		D3DXMATRIX matRotation;
		D3DXMatrixRotationY(&matRotation, D3DXToRadian(90.0f));

		D3DXVec2TransformCoord(pNormalUnitVec, &unitBaseVec, &matRotation);
	}

	FLOAT Vec2Cross(D3DXVECTOR2 secondVec, D3DXVECTOR2 baseVec)
	{
		FLOAT cross = baseVec.x*secondVec.y - baseVec.y*secondVec.x;

		return cross;
	}

	FLOAT Vec2Rad(D3DXVECTOR2 secondVec, D3DXVECTOR2 baseVec)
	{
		FLOAT cos = D3DXVec2Dot(&secondVec, &baseVec) / (D3DXVec2Length(&secondVec)*D3DXVec2Length(&baseVec));

		FLOAT rad = (FLOAT)acos((DOUBLE)cos);

		FLOAT cross = Vec2Cross(secondVec, baseVec);

		if (cross >= 0)
		{
			return -rad;
		}

		if (cross < 0)
		{
			return rad;
		}

		return 0.0f;
	}
}