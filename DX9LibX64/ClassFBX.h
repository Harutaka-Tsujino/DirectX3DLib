#pragma once
#include <windows.h>
#include <iostream>
#include <list>
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

struct Point2
{
	FLOAT x;
	FLOAT y;

	BOOL operator==(Point2& val)
	{
		if (this->x == val.x && this->y == val.y)
		{
			return TRUE;
		}

		return FALSE;
	}
};

struct UvSet
{
	std::string uvSetName;
	std::list<std::string> textures;
	std::list<Point2> uvBuffer;
};

class FBXModel
{
public:
	FBXModel() {};
	~FBXModel() {};

	INT m_polygonCount = 0;
	INT m_vertexCount = 0;
	INT m_indexCount = 0;
	INT m_materialCount = 0;
	INT* m_pPolygonSize = NULL;
	Vertex* m_pVertexBuffer = NULL;
	INT* m_pIndexBuffer = NULL;
	UvSet m_uvSet;
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