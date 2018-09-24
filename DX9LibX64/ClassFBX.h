#pragma once
#include<windows.h>
#include <fbxsdk.h>

using namespace fbxsdk;

struct Vertex
{
public:
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_z;
	FLOAT m_xNormal;
	FLOAT m_yNormal;
	FLOAT m_zNormal;
};

class FBXModel
{
public:
	FBXModel() {};
	~FBXModel() {};

	INT m_polygonCount = 0;
	INT m_vertexCount = 0;
	INT m_indexCount = 0;
	INT* m_pPolygonSize = NULL;
	Vertex* m_pVertexBuffer = NULL;
	INT* m_pIndexBuffer = NULL;
};

class FBX
{
public:
	FBX * GetInstance();
	BOOL Initialize();

private:
	FBX();
	~FBX() {};
};