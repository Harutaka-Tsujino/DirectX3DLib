#include<windows.h>
#include <fbxsdk.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "ClassFBX.h"

#pragma comment(lib,"libfbxsdk-md.lib")

using namespace fbxsdk;

void GetMaterialData(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//	���b�V������m�[�h���擾
	fbxsdk::FbxNode* pNode = pFbxMesh->GetNode();

	//	�}�e���A���̐����擾
	pFBXModel->m_materialCount = pNode->GetMaterialCount();

	//	�e�N�X�`�@�̐����J�E���g����
	static int fileTextureCount = 0;

	//	�}�e���A���̐������J��Ԃ�
	for (int i = 0; i < pFBXModel->m_materialCount; ++i)
	{
		//	�}�e���A�����擾
		fbxsdk::FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		D3DMATERIAL9 MaterialData;

		if (pMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
		{
			// Lambert�Ƀ_�E���L���X�g
			fbxsdk::FbxSurfaceLambert* lambert = (fbxsdk::FbxSurfaceLambert*)pMaterial;

			// �A���r�G���g
			MaterialData.Ambient.r = (float)lambert->Ambient.Get().mData[0] * (float)lambert->AmbientFactor.Get();
			MaterialData.Ambient.g = (float)lambert->Ambient.Get().mData[1] * (float)lambert->AmbientFactor.Get();
			MaterialData.Ambient.b = (float)lambert->Ambient.Get().mData[2] * (float)lambert->AmbientFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sAmbient);

			// �f�B�t���[�Y
			MaterialData.Diffuse.r = (float)lambert->Diffuse.Get().mData[0] * (float)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (float)lambert->Diffuse.Get().mData[1] * (float)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (float)lambert->Diffuse.Get().mData[2] * (float)lambert->DiffuseFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sDiffuse);

			// �G�~�b�V�u
			MaterialData.Emissive.r = (float)lambert->Emissive.Get().mData[0] * (float)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.g = (float)lambert->Emissive.Get().mData[1] * (float)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.b = (float)lambert->Emissive.Get().mData[2] * (float)lambert->EmissiveFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sEmissive);

			// ���ߓx
			MaterialData.Ambient.a = 0.f;
			MaterialData.Diffuse.a = 1.f;
			MaterialData.Emissive.a = 0.f;
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sTransparentColor);


			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sNormalMap);

			pFBXModel->m_MaterialData.push_back(MaterialData);
		}
		else if (pMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
		{
			// Phong�Ƀ_�E���L���X�g
			fbxsdk::FbxSurfacePhong* phong = (fbxsdk::FbxSurfacePhong*)pMaterial;

			// �A���r�G���g
			MaterialData.Ambient.r = (float)phong->Ambient.Get().mData[0] * (float)phong->AmbientFactor.Get();
			MaterialData.Ambient.g = (float)phong->Ambient.Get().mData[1] * (float)phong->AmbientFactor.Get();
			MaterialData.Ambient.b = (float)phong->Ambient.Get().mData[2] * (float)phong->AmbientFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sAmbient);

			// �f�B�t���[�Y
			MaterialData.Diffuse.r = (float)phong->Diffuse.Get().mData[0] * (float)phong->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (float)phong->Diffuse.Get().mData[1] * (float)phong->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (float)phong->Diffuse.Get().mData[2] * (float)phong->DiffuseFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sDiffuse);

			// �G�~�b�V�u
			MaterialData.Emissive.r = (float)phong->Emissive.Get().mData[0] * (float)phong->EmissiveFactor.Get();
			MaterialData.Emissive.g = (float)phong->Emissive.Get().mData[1] * (float)phong->EmissiveFactor.Get();
			MaterialData.Emissive.b = (float)phong->Emissive.Get().mData[2] * (float)phong->EmissiveFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sEmissive);

			// ���ߓx
			MaterialData.Ambient.a = 0.f;
			MaterialData.Diffuse.a = 1.f;
			MaterialData.Emissive.a = 0.f;
			MaterialData.Specular.a = 0.f;
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sTransparentColor);

			// �X�y�L����
			MaterialData.Specular.r = (float)phong->Specular.Get().mData[0] * (float)phong->SpecularFactor.Get();
			MaterialData.Specular.g = (float)phong->Specular.Get().mData[1] * (float)phong->SpecularFactor.Get();
			MaterialData.Specular.b = (float)phong->Specular.Get().mData[2] * (float)phong->SpecularFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sSpecular);

			// ����
			//			shininess_ = (float)phong->GetShininess().Get();		??????????????

			// ����
			//			reflectivity_ = (float)phong->GetReflectionFactor().Get();		??????????????

			MaterialData.Power = (float)phong->Shininess.Get();

			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sNormalMap);

			pFBXModel->m_MaterialData.push_back(MaterialData);
		}

	}

}

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

	return;
}

VOID GetTextureNames(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- ���b�V������m�[�h���擾 ---//
	FbxNode* node = pFbxMesh->GetNode();

	//--- �}�e���A���̐����擾 ---//
	INT materialCount = node->GetMaterialCount();

	//--- �}�e���A���̐������J��Ԃ� ---//
	for (INT materialNum = 0; materialNum < materialCount; ++materialNum)
	{
		//--- �}�e���A�����擾 ---//
		FbxSurfaceMaterial* material = node->GetMaterial(materialNum);

		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//--- FbxLayeredTexture �̐����擾 ---//
		INT layeredTextureCount = prop.GetSrcObjectCount();

		//--- �A�^�b�`���ꂽ�e�N�X�`���� FbxLayeredTexture �̏ꍇ ---//
		if (0 < layeredTextureCount)
		{
			//--- �A�^�b�`���ꂽ�e�N�X�`���̐������J��Ԃ� ---//
			for (INT j = 0; layeredTextureCount > j; j++)
			{
				//--- �e�N�X�`�����擾 ---//
				FbxLayeredTexture* pLayeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);

				//--- ���C���[�����擾 ---//
				INT textureCount = pLayeredTexture->GetSrcObjectCount<FbxFileTexture>();

				//--- ���C���[�������J��Ԃ� ---//
				for (INT k = 0; textureCount > k; k++)
				{
					//--- �e�N�X�`�����擾 ---//
					FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(k);

					if (!pTexture)
					{
						continue;
					}

					//--- �e�N�X�`�������擾 ---//
					//std::string textureName = pTexture->GetName();
					std::string textureName = pTexture->GetRelativeFileName();

					//--- UVSet�����擾 ---//
					std::string UVSetName = pTexture->UVSet.Get().Buffer();

					//--- UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ� ---//
					if (pFBXModel->m_uvSet.uvSetName == UVSetName)
					{
						pFBXModel->m_uvSet.textures.push_back(textureName);
					}
				}
			}
		}

		if (0 >= layeredTextureCount)
		{
			//--- �e�N�X�`�������擾 ---//
			INT fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 <= fileTextureCount)
			{
				break;
			}

			//--- �e�N�X�`���̐������J��Ԃ� ---//
			for (INT j = 0; fileTextureCount > j; j++)
			{
				//--- �e�N�X�`�����擾 ---//
				FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(j);

				if (pTexture)
				{
					continue;
				}

				//--- �e�N�X�`�������擾 ---//
				//std::string textureName = pTexture->GetName();
				std::string textureName = pTexture->GetRelativeFileName();

				//--- UVSet�����擾 ---//
				std::string UVSetName = pTexture->UVSet.Get().Buffer();

				//--- UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ� ---//
				if (pFBXModel->m_uvSet.uvSetName == UVSetName)
				{
					pFBXModel->m_uvSet.textures.push_back(textureName);
				}
			}
		}
	}

	return;
}

VOID GetVertexUV(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- UV�Z�b�g�����擾 ---//
	INT UVLayerCount = pFbxMesh->GetElementUVCount();

	for (INT uvLayer = 0; uvLayer < UVLayerCount; uvLayer++)
	{
		//--- UV�o�b�t�@���擾 ---//
		FbxGeometryElementUV* UV = pFbxMesh->GetElementUV(uvLayer);

		//--- �}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//--- ���t�@�����X���[�h�̎擾 ---//
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

		//--- UV�����擾 ---//
		INT uvCount = UV->GetDirectArray().GetCount();

		//--- �}�b�s���O���[�h�̔��� ---//
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:

			break;

		case FbxGeometryElement::eByPolygonVertex:

			//--- ���t�@�����X���[�h�̔��� ---//
			switch (reference)
			{
			case FbxGeometryElement::eDirect:

				break;

			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<INT>* uvIndex = &UV->GetIndexArray();
				INT uvIndexCount = uvIndex->GetCount();

				//--- UV��ێ� ---// 
				Point2 temp;

				UvSet& uvSet = pFBXModel->m_uvSet;

				for (INT index = 0; index < uvIndexCount; ++index)
				{
					temp.x = (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(index))[0];

					temp.y = 1.0f - (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(index))[1];

					uvSet.uvBuffer.push_back(temp);
				}

				//--- UVSet�����擾 ---//
				uvSet.uvSetName = UV->GetName();
			}

			break;

			default:

				break;
			}

			break;

		case FbxGeometryElement::eByEdge:

			break;

		case FbxGeometryElement::eByPolygon:

			break;

		default:

			break;
		}
	}

	return;
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

	//--- ���b�V���m�[�h���� ---//
	if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pFbxMesh = pChildNode->GetMesh();

		GetShape(pFbxMesh, pFBXModel);

		//--- ���_�̖@�����W���擾 ---//
		GetVertexNormal(pFbxMesh, pFBXModel);

		GetVertexUV(pFbxMesh, pFBXModel);

		GetTextureNames(pFbxMesh, pFBXModel);
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