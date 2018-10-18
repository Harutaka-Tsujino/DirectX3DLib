#pragma once
#include <windows.h>
#include <d3dx9.h>
#include "../Draw/Draw.h"
#include"../../FBX/FbxRelated.h"

struct CustomVertex
{
public:
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_z;
	FLOAT m_rHW;
	DWORD m_aRGB;
	FLOAT m_tu;
	FLOAT m_tv;
};

VOID CustomVertices(CustomVertex *pCustomVertex, FLOAT posX, FLOAT posY, FLOAT posZ, FLOAT scaleX, FLOAT scaleY, DWORD color = 0xFFFFFFFF,
	FLOAT startPosTu = 0.0f, FLOAT startPosTv = 0.0f, FLOAT scaleTu = 1.0f, FLOAT scaleTv = 1.0f, FLOAT scaleImageX = 1.0f, FLOAT scaleImageY = 1.0f);

class Object
{
public:
	Object() {};
	~Object() {};

	virtual VOID Control() = 0;
	virtual VOID Render() = 0;
};
