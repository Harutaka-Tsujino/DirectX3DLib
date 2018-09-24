#include<windows.h>
#include <fbxsdk.h>
#include "ClassFBX.h"

#pragma comment(lib,"libfbxsdk-md.lib")

using namespace fbxsdk;

VOID GetVertexNormal(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- �@���Z�b�g�����擾 ---//
	INT normalLayerCount = pFbxMesh->GetElementNormalCount();

	//--- ���C���[��������� ---//
	for (INT normalLayer = 0; normalLayer < normalLayerCount; ++normalLayer)
	{
		//--- �@���Z�b�g���擾 ---//
		FbxGeometryElementNormal* normal = pFbxMesh->GetElementNormal(normalLayer);

		//--- �}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();

		//--- ���t�@�����X���[�h�̎擾 ---//
		FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

		//--- �}�b�s���O���[�h�̔��� ---//
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:

			//--- ���t�@�����X���[�h�̔��� ---//
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
			{
				//--- �@�������擾 ---//
				INT normalCount = normal->GetDirectArray().GetCount();

				//-----------------------------------------------------------------------
				// eDirect �̏ꍇ�f�[�^�͏��ԂɊi�[����Ă���̂ł��̂܂ܕێ�
				//-----------------------------------------------------------------------

				Vertex*& rPVertexBuffer = pFBXModel->m_pVertexBuffer;

				for (INT normalVect = 0; normalVect < normalCount; ++normalVect)
				{
					//--- �@���̎擾 ---//
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
	//--- �|���S�������擾 ---//
	INT polygonCount = pFBXModel->m_polygonCount = pFbxMesh->GetPolygonCount();

	//--- �����_�����擾 ---//
	INT vertexCount = pFBXModel->m_vertexCount = pFbxMesh->GetControlPointsCount();

	//--- �C���f�b�N�X�����擾 ---//
	INT indexCount = pFBXModel->m_indexCount = pFbxMesh->GetPolygonVertexCount();

	//--- ���_�o�b�t�@�̎擾 ---//
	FbxVector4* vertices = pFbxMesh->GetControlPoints();

	//--- �C���f�b�N�X�o�b�t�@�̎擾 ---//
	INT* indices = pFbxMesh->GetPolygonVertices();

	//--- �|���S���T�C�Y���擾 ---//
	INT*& rPPolygonSize = pFBXModel->m_pPolygonSize = new INT[polygonCount];

	for (INT polygon = 0; polygon < polygonCount; ++polygon)
	{
		rPPolygonSize[polygon] = pFbxMesh->GetPolygonSize(polygon);
	}

	//--- ���_���W�̃L���X�g�ƃn�[�h�R�s�[ ---//
	Vertex*& rPVertexBuffer = pFBXModel->m_pVertexBuffer = new Vertex[pFBXModel->m_vertexCount];

	for (INT vertex = 0; vertex < vertexCount; ++vertex)
	{
		//--- vertex�Ԗڂ̒��_�̕W�w���擾 ---//
		rPVertexBuffer[vertex].m_x = (float)vertices[vertex][0];

		//--- vertex�Ԗڂ̒��_�̍��W�x���擾 ---//
		rPVertexBuffer[vertex].m_y = (float)vertices[vertex][1];

		//--- vertex�Ԗڂ̒��_�̍��W�y���擾 ---//
		rPVertexBuffer[vertex].m_z = (float)vertices[vertex][2];
	}

	//--- �C���f�b�N�X�o�b�t�@�̃n�[�h�R�s�[ ---//
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