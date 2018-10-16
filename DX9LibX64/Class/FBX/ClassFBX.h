#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
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

struct TextureData
{
	LPCSTR m_TextureName;
	LPDIRECT3DTEXTURE9 m_pTexture;
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
	INT m_fileTextureCount = 0;
	INT* m_pPolygonSize = NULL;
	Vertex* m_pVertexBuffer = NULL;
	INT* m_pIndexBuffer = NULL;
	UvSet m_uvSet;
	TextureData* m_pTmpTexture;
	std::vector<TextureData*> m_pTextureData;
	std::vector<D3DMATERIAL9> m_materialData;
};

class FBX
{
public:
	BOOL Initialize();
	VOID GetMaterialData(FbxMesh* pFbxMesh, FBXModel* pFBXModel);
	VOID GetVertexNormal(FbxMesh* pFbxMesh, FBXModel* pFBXModel);
	VOID GetTextureName(FbxSurfaceMaterial* pMaterial, const char* pMatAttr, FBXModel* pFBXModel);
	VOID GetVertexUV(FbxMesh* pFbxMesh, FBXModel* pFBXModel);
	VOID GetShape(FbxMesh* pFbxMesh, FBXModel* pFBXModel);
	VOID GetMesh(FbxNode* pChildNode, FBXModel* pFBXModel);
	VOID DrawFBX(FBXModel* pFBXModel);
};

extern FBXModel g_fBXModel;
