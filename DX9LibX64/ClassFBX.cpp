#include<windows.h>
#include <fbxsdk.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "ClassFBX.h"

#pragma comment(lib,"libfbxsdk-md.lib")

using namespace fbxsdk;

void GetMaterialData(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//	メッシュからノードを取得
	fbxsdk::FbxNode* pNode = pFbxMesh->GetNode();

	//	マテリアルの数を取得
	pFBXModel->m_materialCount = pNode->GetMaterialCount();

	//	テクスチァの数をカウントする
	static int fileTextureCount = 0;

	//	マテリアルの数だけ繰り返す
	for (int i = 0; i < pFBXModel->m_materialCount; ++i)
	{
		//	マテリアルを取得
		fbxsdk::FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		D3DMATERIAL9 MaterialData;

		if (pMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
		{
			// Lambertにダウンキャスト
			fbxsdk::FbxSurfaceLambert* lambert = (fbxsdk::FbxSurfaceLambert*)pMaterial;

			// アンビエント
			MaterialData.Ambient.r = (float)lambert->Ambient.Get().mData[0] * (float)lambert->AmbientFactor.Get();
			MaterialData.Ambient.g = (float)lambert->Ambient.Get().mData[1] * (float)lambert->AmbientFactor.Get();
			MaterialData.Ambient.b = (float)lambert->Ambient.Get().mData[2] * (float)lambert->AmbientFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sAmbient);

			// ディフューズ
			MaterialData.Diffuse.r = (float)lambert->Diffuse.Get().mData[0] * (float)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (float)lambert->Diffuse.Get().mData[1] * (float)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (float)lambert->Diffuse.Get().mData[2] * (float)lambert->DiffuseFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sDiffuse);

			// エミッシブ
			MaterialData.Emissive.r = (float)lambert->Emissive.Get().mData[0] * (float)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.g = (float)lambert->Emissive.Get().mData[1] * (float)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.b = (float)lambert->Emissive.Get().mData[2] * (float)lambert->EmissiveFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sEmissive);

			// 透過度
			MaterialData.Ambient.a = 0.f;
			MaterialData.Diffuse.a = 1.f;
			MaterialData.Emissive.a = 0.f;
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sTransparentColor);


			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sNormalMap);

			pFBXModel->m_MaterialData.push_back(MaterialData);
		}
		else if (pMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
		{
			// Phongにダウンキャスト
			fbxsdk::FbxSurfacePhong* phong = (fbxsdk::FbxSurfacePhong*)pMaterial;

			// アンビエント
			MaterialData.Ambient.r = (float)phong->Ambient.Get().mData[0] * (float)phong->AmbientFactor.Get();
			MaterialData.Ambient.g = (float)phong->Ambient.Get().mData[1] * (float)phong->AmbientFactor.Get();
			MaterialData.Ambient.b = (float)phong->Ambient.Get().mData[2] * (float)phong->AmbientFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sAmbient);

			// ディフューズ
			MaterialData.Diffuse.r = (float)phong->Diffuse.Get().mData[0] * (float)phong->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (float)phong->Diffuse.Get().mData[1] * (float)phong->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (float)phong->Diffuse.Get().mData[2] * (float)phong->DiffuseFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sDiffuse);

			// エミッシブ
			MaterialData.Emissive.r = (float)phong->Emissive.Get().mData[0] * (float)phong->EmissiveFactor.Get();
			MaterialData.Emissive.g = (float)phong->Emissive.Get().mData[1] * (float)phong->EmissiveFactor.Get();
			MaterialData.Emissive.b = (float)phong->Emissive.Get().mData[2] * (float)phong->EmissiveFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sEmissive);

			// 透過度
			MaterialData.Ambient.a = 0.f;
			MaterialData.Diffuse.a = 1.f;
			MaterialData.Emissive.a = 0.f;
			MaterialData.Specular.a = 0.f;
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sTransparentColor);

			// スペキュラ
			MaterialData.Specular.r = (float)phong->Specular.Get().mData[0] * (float)phong->SpecularFactor.Get();
			MaterialData.Specular.g = (float)phong->Specular.Get().mData[1] * (float)phong->SpecularFactor.Get();
			MaterialData.Specular.b = (float)phong->Specular.Get().mData[2] * (float)phong->SpecularFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sSpecular);

			// 光沢
			//			shininess_ = (float)phong->GetShininess().Get();		??????????????

			// 反射
			//			reflectivity_ = (float)phong->GetReflectionFactor().Get();		??????????????

			MaterialData.Power = (float)phong->Shininess.Get();

			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sNormalMap);

			pFBXModel->m_MaterialData.push_back(MaterialData);
		}

	}

}

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

	return;
}

VOID GetTextureNames(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- メッシュからノードを取得 ---//
	FbxNode* node = pFbxMesh->GetNode();

	//--- マテリアルの数を取得 ---//
	INT materialCount = node->GetMaterialCount();

	//--- マテリアルの数だけ繰り返す ---//
	for (INT materialNum = 0; materialNum < materialCount; ++materialNum)
	{
		//--- マテリアルを取得 ---//
		FbxSurfaceMaterial* material = node->GetMaterial(materialNum);

		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//--- FbxLayeredTexture の数を取得 ---//
		INT layeredTextureCount = prop.GetSrcObjectCount();

		//--- アタッチされたテクスチャが FbxLayeredTexture の場合 ---//
		if (0 < layeredTextureCount)
		{
			//--- アタッチされたテクスチャの数だけ繰り返す ---//
			for (INT j = 0; layeredTextureCount > j; j++)
			{
				//--- テクスチャを取得 ---//
				FbxLayeredTexture* pLayeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);

				//--- レイヤー数を取得 ---//
				INT textureCount = pLayeredTexture->GetSrcObjectCount<FbxFileTexture>();

				//--- レイヤー数だけ繰り返す ---//
				for (INT k = 0; textureCount > k; k++)
				{
					//--- テクスチャを取得 ---//
					FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(k);

					if (!pTexture)
					{
						continue;
					}

					//--- テクスチャ名を取得 ---//
					//std::string textureName = pTexture->GetName();
					std::string textureName = pTexture->GetRelativeFileName();

					//--- UVSet名を取得 ---//
					std::string UVSetName = pTexture->UVSet.Get().Buffer();

					//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
					if (pFBXModel->m_uvSet.uvSetName == UVSetName)
					{
						pFBXModel->m_uvSet.textures.push_back(textureName);
					}
				}
			}
		}

		if (0 >= layeredTextureCount)
		{
			//--- テクスチャ数を取得 ---//
			INT fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 <= fileTextureCount)
			{
				break;
			}

			//--- テクスチャの数だけ繰り返す ---//
			for (INT j = 0; fileTextureCount > j; j++)
			{
				//--- テクスチャを取得 ---//
				FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(j);

				if (pTexture)
				{
					continue;
				}

				//--- テクスチャ名を取得 ---//
				//std::string textureName = pTexture->GetName();
				std::string textureName = pTexture->GetRelativeFileName();

				//--- UVSet名を取得 ---//
				std::string UVSetName = pTexture->UVSet.Get().Buffer();

				//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
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
	//--- UVセット数を取得 ---//
	INT UVLayerCount = pFbxMesh->GetElementUVCount();

	for (INT uvLayer = 0; uvLayer < UVLayerCount; uvLayer++)
	{
		//--- UVバッファを取得 ---//
		FbxGeometryElementUV* UV = pFbxMesh->GetElementUV(uvLayer);

		//--- マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//--- リファレンスモードの取得 ---//
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

		//--- UV数を取得 ---//
		INT uvCount = UV->GetDirectArray().GetCount();

		//--- マッピングモードの判別 ---//
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:

			break;

		case FbxGeometryElement::eByPolygonVertex:

			//--- リファレンスモードの判別 ---//
			switch (reference)
			{
			case FbxGeometryElement::eDirect:

				break;

			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<INT>* uvIndex = &UV->GetIndexArray();
				INT uvIndexCount = uvIndex->GetCount();

				//--- UVを保持 ---// 
				Point2 temp;

				UvSet& uvSet = pFBXModel->m_uvSet;

				for (INT index = 0; index < uvIndexCount; ++index)
				{
					temp.x = (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(index))[0];

					temp.y = 1.0f - (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(index))[1];

					uvSet.uvBuffer.push_back(temp);
				}

				//--- UVSet名を取得 ---//
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

	//--- メッシュノード発見 ---//
	if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pFbxMesh = pChildNode->GetMesh();

		GetShape(pFbxMesh, pFBXModel);

		//--- 頂点の法線座標を取得 ---//
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