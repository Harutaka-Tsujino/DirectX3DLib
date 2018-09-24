#include<windows.h>
#include <fbxsdk.h>
#include "ClassFBX.h"

#pragma comment(lib,"libfbxsdk-md.lib")

using namespace fbxsdk;

VOID GetVertexNormal(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- 法線セット数を取得 ---//
	INT normalLayerCount = pFbxMesh->GetElementNormalCount();

	//--- レイヤー数だけ回る ---//
	for (INT normalLayer = 0; normalLayer < normalLayerCount; ++normalLayer)
	{
		//--- 法線セットを取得 ---//
		FbxGeometryElementNormal* normal = pFbxMesh->GetElementNormal(normalLayer);

		//--- マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();

		//--- リファレンスモードの取得 ---//
		FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

		//--- マッピングモードの判別 ---//
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:

			//--- リファレンスモードの判別 ---//
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
			{
				//--- 法線数を取得 ---//
				INT normalCount = normal->GetDirectArray().GetCount();

				//-----------------------------------------------------------------------
				// eDirect の場合データは順番に格納されているのでそのまま保持
				//-----------------------------------------------------------------------

				Vertex*& rPVertexBuffer = pFBXModel->m_pVertexBuffer;

				for (INT normalVect = 0; normalVect < normalCount; ++normalVect)
				{
					//--- 法線の取得 ---//
					rPVertexBuffer[normalVect].m_xNormal = (float)normal->GetDirectArray().GetAt(normalVect)[0];
					rPVertexBuffer[normalVect].m_yNormal = (float)normal->GetDirectArray().GetAt(normalVect)[1];
					rPVertexBuffer[normalVect].m_zNormal = (float)normal->GetDirectArray().GetAt(normalVect)[2];
				}
			}

			break;

			case FbxGeometryElement::eIndexToDirect:

				break;

			default:

				break;
			}

			break;

		case FbxGeometryElement::eByPolygonVertex:

			break;

		default:

			break;
		}
	}
}

VOID GetShape(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- ポリゴン数を取得 ---//
	INT polygonCount = pFBXModel->m_polygonCount = pFbxMesh->GetPolygonCount();

	//--- 総頂点数を取得 ---//
	INT vertexCount = pFBXModel->m_vertexCount = pFbxMesh->GetControlPointsCount();

	//--- インデックス数を取得 ---//
	INT indexCount = pFBXModel->m_indexCount = pFbxMesh->GetPolygonVertexCount();

	//--- 頂点バッファの取得 ---//
	FbxVector4* vertices = pFbxMesh->GetControlPoints();

	//--- インデックスバッファの取得 ---//
	INT* indices = pFbxMesh->GetPolygonVertices();

	//--- ポリゴンサイズを取得 ---//
	INT*& rPPolygonSize = pFBXModel->m_pPolygonSize = new INT[polygonCount];

	for (INT polygon = 0; polygon < polygonCount; ++polygon)
	{
		rPPolygonSize[polygon] = pFbxMesh->GetPolygonSize(polygon);
	}

	//--- 頂点座標のキャストとハードコピー ---//
	Vertex*& rPVertexBuffer = pFBXModel->m_pVertexBuffer = new Vertex[pFBXModel->m_vertexCount];

	for (INT vertex = 0; vertex < vertexCount; ++vertex)
	{
		//--- vertex番目の頂点の標Ｘを取得 ---//
		rPVertexBuffer[vertex].m_x = (float)vertices[vertex][0];

		//--- vertex番目の頂点の座標Ｙを取得 ---//
		rPVertexBuffer[vertex].m_y = (float)vertices[vertex][1];

		//--- vertex番目の頂点の座標Ｚを取得 ---//
		rPVertexBuffer[vertex].m_z = (float)vertices[vertex][2];
	}

	//--- インデックスバッファのハードコピー ---//
	INT*& rPIndexBuffer = pFBXModel->m_pIndexBuffer = new INT[indexCount];

	for (INT index = 0; index < indexCount; ++index)
	{
		rPIndexBuffer[index] = indices[index];
	}

	return;
}

VOID GetMesh(FbxNode* pChildNode, FBXModel* pFBXModel)
{
	FbxNodeAttribute* nodeAttribute = pChildNode->GetNodeAttribute();

	if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pFbxMesh = pChildNode->GetMesh();

		GetShape(pFbxMesh, pFBXModel);

		GetVertexNormal(pFbxMesh, pFBXModel);
	}

	INT childNodes = pChildNode->GetChildCount();

	for (INT nodes = 0; nodes < childNodes; ++nodes)
	{
		GetMesh(pChildNode->GetChild(nodes), pFBXModel);
	}

	return;
}

BOOL FBX::Initialize()
{
	FbxManager* pFbxManager = FbxManager::Create();

	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");

	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "");

	pFbxImporter->Initialize("Obj001.fbx");

	FBXModel fBXModel;

	pFbxImporter->Import(pFbxScene);

	FbxNode* pRootNode = pFbxScene->GetRootNode();

	INT childNodes = pRootNode->GetChildCount();

	for (INT nodes = 0; nodes < childNodes; ++nodes)
	{
		GetMesh(pRootNode->GetChild(nodes), &fBXModel);
	}



	//importer->Destroy();

	return TRUE;
}