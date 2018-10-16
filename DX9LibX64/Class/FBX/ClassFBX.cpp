include<windows.h>
#include <fbxsdk.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "ClassDirectX.h"
#include "ClassFBX.h"

#pragma comment(lib,"libfbxsdk-md.lib")

using namespace fbxsdk;

VOID FBX::GetTextureName(FbxSurfaceMaterial* pMaterial, const char* pMatAttr, FBXModel* pFBXModel)
{
	//	プロパティを取得
	fbxsdk::FbxProperty prop = pMaterial->FindProperty(pMatAttr);

	//	FbxLayerTexture の数を取得
	int layeredTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();

	//	アタッチされたテクスチャがFbxLayeredTexture の場合
	if (0 < layeredTextureCount)
	{
		//	アタッチされたテクスチャの数だけ繰り返す
		for (int i = 0; layeredTextureCount > i; i++)
		{
			//	テクスチャを取得
			fbxsdk::FbxLayeredTexture* pLayeredTexture = prop.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);

			//	レイヤー数を取得
			int textureCount = pLayeredTexture->GetSrcObjectCount<fbxsdk::FbxFileTexture>();

			//	レイヤー数だけ繰り返す
			for (int j = 0; textureCount > j; j++)
			{
				//	テクスチャを取得
				fbxsdk::FbxFileTexture* pFbxFileTexture = prop.GetSrcObject<fbxsdk::FbxFileTexture>(j);

				if (pFbxFileTexture)
				{
					//	テクスチャ名を取得
					//	std::string textureName = texture->GetName();
					LPCSTR textureName = pFbxFileTexture->GetRelativeFileName();

					//	UVSet名を取得
					std::string UVSetName = pFbxFileTexture->UVSet.Get().Buffer();

					//	UVSet名を比較し対応しているテクスチャなら保持してデータクラスにUV座標をいれる
					if (pFBXModel->m_uvSet.uvSetName == UVSetName)
					{
						pFBXModel->m_pTmpTexture = new TextureData;
						pFBXModel->m_pTmpTexture->m_TextureName = textureName;
						pFBXModel->m_pTextureData.push_back(pFBXModel->m_pTmpTexture);

						if (FAILED(D3DXCreateTextureFromFile(
							DirectX::m_directXInstances.m_pDirectX3DDevice,
							pFBXModel->m_pTextureData[pFBXModel->m_fileTextureCount]->m_TextureName,
							&pFBXModel->m_pTextureData[pFBXModel->m_fileTextureCount]->m_pTexture)))
						{
							pFBXModel->m_pTextureData[pFBXModel->m_fileTextureCount]->m_pTexture = NULL;
						}
						pFBXModel->m_fileTextureCount++;
					}
				}
			}
		}
	}

	else
	{
		//	テクスチャ数を取得
		int fileTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();

		if (0 < fileTextureCount)
		{
			//	テクスチャの数だけ繰り返す
			for (int i = 0; fileTextureCount > i; i++)
			{
				//	テクスチャを取得
				fbxsdk::FbxFileTexture* pFbxFileTexture = prop.GetSrcObject<fbxsdk::FbxFileTexture>(i);

				if (pFbxFileTexture)
				{
					//	テクスチャ名を取得
					//	std::string textureName = texture->GetName();
					LPCSTR textureName = pFbxFileTexture->GetRelativeFileName();

					//	UVSet名を取得
					std::string UVSetName = pFbxFileTexture->UVSet.Get().Buffer();

					//	UVSet名を比較し対応しているテクスチャなら保持
					if (pFBXModel->m_uvSet.uvSetName == UVSetName)
					{
						pFBXModel->m_pTmpTexture = new TextureData;
						pFBXModel->m_pTmpTexture->m_TextureName = textureName;
						pFBXModel->m_pTextureData.push_back(pFBXModel->m_pTmpTexture);

						if (FAILED(D3DXCreateTextureFromFile(
							DirectX::m_directXInstances.m_pDirectX3DDevice,
							pFBXModel->m_pTextureData[pFBXModel->m_fileTextureCount]->m_TextureName,
							&pFBXModel->m_pTextureData[pFBXModel->m_fileTextureCount]->m_pTexture)))
						{
							pFBXModel->m_pTextureData[pFBXModel->m_fileTextureCount]->m_pTexture = NULL;
						}
						pFBXModel->m_fileTextureCount++;
					}
				}
			}
		}
	}

	return;
}

VOID FBX::GetMaterialData(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
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
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sAmbient, pFBXModel);

			// ディフューズ
			MaterialData.Diffuse.r = (float)lambert->Diffuse.Get().mData[0] * (float)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (float)lambert->Diffuse.Get().mData[1] * (float)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (float)lambert->Diffuse.Get().mData[2] * (float)lambert->DiffuseFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sDiffuse, pFBXModel);

			// エミッシブ
			MaterialData.Emissive.r = (float)lambert->Emissive.Get().mData[0] * (float)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.g = (float)lambert->Emissive.Get().mData[1] * (float)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.b = (float)lambert->Emissive.Get().mData[2] * (float)lambert->EmissiveFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sEmissive, pFBXModel);

			// 透過度
			MaterialData.Ambient.a = 0.f;
			MaterialData.Diffuse.a = 1.f;
			MaterialData.Emissive.a = 0.f;
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sTransparentColor, pFBXModel);


			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sNormalMap, pFBXModel);


			pFBXModel->m_materialData.push_back(MaterialData);
		}

		else
		{
			if (pMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
			{
				// Phongにダウンキャスト
				fbxsdk::FbxSurfacePhong* phong = (fbxsdk::FbxSurfacePhong*)pMaterial;

				// アンビエント
				MaterialData.Ambient.r = (float)phong->Ambient.Get().mData[0] * (float)phong->AmbientFactor.Get();
				MaterialData.Ambient.g = (float)phong->Ambient.Get().mData[1] * (float)phong->AmbientFactor.Get();
				MaterialData.Ambient.b = (float)phong->Ambient.Get().mData[2] * (float)phong->AmbientFactor.Get();
				GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sAmbient, pFBXModel);

				// ディフューズ
				MaterialData.Diffuse.r = (float)phong->Diffuse.Get().mData[0] * (float)phong->DiffuseFactor.Get();
				MaterialData.Diffuse.g = (float)phong->Diffuse.Get().mData[1] * (float)phong->DiffuseFactor.Get();
				MaterialData.Diffuse.b = (float)phong->Diffuse.Get().mData[2] * (float)phong->DiffuseFactor.Get();
				GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sDiffuse, pFBXModel);

				// エミッシブ
				MaterialData.Emissive.r = (float)phong->Emissive.Get().mData[0] * (float)phong->EmissiveFactor.Get();
				MaterialData.Emissive.g = (float)phong->Emissive.Get().mData[1] * (float)phong->EmissiveFactor.Get();
				MaterialData.Emissive.b = (float)phong->Emissive.Get().mData[2] * (float)phong->EmissiveFactor.Get();
				GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sEmissive, pFBXModel);

				// 透過度
				MaterialData.Ambient.a = 0.f;
				MaterialData.Diffuse.a = 1.f;
				MaterialData.Emissive.a = 0.f;
				MaterialData.Specular.a = 0.f;
				GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sTransparentColor, pFBXModel);

				// スペキュラ
				MaterialData.Specular.r = (float)phong->Specular.Get().mData[0] * (float)phong->SpecularFactor.Get();
				MaterialData.Specular.g = (float)phong->Specular.Get().mData[1] * (float)phong->SpecularFactor.Get();
				MaterialData.Specular.b = (float)phong->Specular.Get().mData[2] * (float)phong->SpecularFactor.Get();
				GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sSpecular, pFBXModel);

				// 光沢
				//			shininess_ = (float)phong->GetShininess().Get();		??????????????

				// 反射
				//			reflectivity_ = (float)phong->GetReflectionFactor().Get();		??????????????

				MaterialData.Power = (float)phong->Shininess.Get();

				GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sNormalMap, pFBXModel);

				MaterialData.Power = (float)phong->Shininess.Get();

				pFBXModel->m_materialData.push_back(MaterialData);
			}
		}
	}

	return;
}

VOID FBX::GetVertexNormal(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
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

				break;
			}

			break;

			case FbxGeometryElement::eIndexToDirect:

				break;

			default:

				break;
			}

			break;

		case FbxGeometryElement::eByPolygonVertex:

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

				break;
			}

			break;

			case FbxGeometryElement::eIndexToDirect:

				break;

			default:

				break;
			}

			break;

		default:

			break;
		}
	}

	return;
}

//VOID FBX::GetTextureNames(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
//{
//	//--- メッシュからノードを取得 ---//
//	FbxNode* node = pFbxMesh->GetNode();
//
//	//--- マテリアルの数を取得 ---//
//	INT materialCount = node->GetMaterialCount();
//
//	//--- マテリアルの数だけ繰り返す ---//
//	for (INT materialNum = 0; materialNum < materialCount; ++materialNum)
//	{
//		//--- マテリアルを取得 ---//
//		FbxSurfaceMaterial* material = node->GetMaterial(materialNum);
//
//		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
//
//		//--- FbxLayeredTexture の数を取得 ---//
//		INT layeredTextureCount = prop.GetSrcObjectCount();
//
//		//--- アタッチされたテクスチャが FbxLayeredTexture の場合 ---//
//		if (0 < layeredTextureCount)
//		{
//			//--- アタッチされたテクスチャの数だけ繰り返す ---//
//			for (INT j = 0; layeredTextureCount > j; j++)
//			{
//				//--- テクスチャを取得 ---//
//				FbxLayeredTexture* pLayeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
//
//				//--- レイヤー数を取得 ---//
//				INT textureCount = pLayeredTexture->GetSrcObjectCount<FbxFileTexture>();
//
//				//--- レイヤー数だけ繰り返す ---//
//				for (INT k = 0; textureCount > k; k++)
//				{
//					//--- テクスチャを取得 ---//
//					FbxFileTexture* m_pTexture = prop.GetSrcObject<FbxFileTexture>(k);
//
//					if (!m_pTexture)
//					{
//						continue;
//					}
//
//					//--- テクスチャ名を取得 ---//
//					//std::string textureName = m_pTexture->GetName();
//					std::string textureName = m_pTexture->GetRelativeFileName();
//
//					//--- UVSet名を取得 ---//
//					std::string UVSetName = m_pTexture->UVSet.Get().Buffer();
//
//					//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
//					if (pFBXModel->m_uvSet.uvSetName == UVSetName)
//					{
//						pFBXModel->m_uvSet.textures.push_back(textureName);
//					}
//				}
//			}
//		}
//
//		if (0 >= layeredTextureCount)
//		{
//			//--- テクスチャ数を取得 ---//
//			INT fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
//
//			if (0 <= fileTextureCount)
//			{
//				break;
//			}
//
//			//--- テクスチャの数だけ繰り返す ---//
//			for (INT j = 0; fileTextureCount > j; j++)
//			{
//				//--- テクスチャを取得 ---//
//				FbxFileTexture* m_pTexture = prop.GetSrcObject<FbxFileTexture>(j);
//
//				if (m_pTexture)
//				{
//					continue;
//				}
//
//				//--- テクスチャ名を取得 ---//
//				//std::string textureName = m_pTexture->GetName();
//				std::string textureName = m_pTexture->GetRelativeFileName();
//
//				//--- UVSet名を取得 ---//
//				std::string UVSetName = m_pTexture->UVSet.Get().Buffer();
//
//				//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
//				if (pFBXModel->m_uvSet.uvSetName == UVSetName)
//				{
//					pFBXModel->m_uvSet.textures.push_back(textureName);
//				}
//			}
//		}
//	}
//
//	return;
//}

VOID FBX::GetVertexUV(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
{
	//--- UVセット数を取得 ---//
	INT UVLayerCount = pFbxMesh->GetElementUVCount();

	for (INT uvLayer = 0; uvLayer < UVLayerCount; ++uvLayer)
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

VOID FBX::GetShape(FbxMesh* pFbxMesh, FBXModel* pFBXModel)
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
	Vertex*& rPVertexBuffer = pFBXModel->m_pVertexBuffer = new Vertex[pFBXModel->m_indexCount];

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

VOID FBX::GetMesh(FbxNode* pChildNode, FBXModel* pFBXModel)
{
	FbxNodeAttribute* nodeAttribute = pChildNode->GetNodeAttribute();

	if (nodeAttribute)
	{
		//--- メッシュノード発見 ---//
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pFbxMesh = pChildNode->GetMesh();

			GetShape(pFbxMesh, pFBXModel);

			//--- 頂点の法線座標を取得 ---//
			GetVertexNormal(pFbxMesh, pFBXModel);

			GetVertexUV(pFbxMesh, pFBXModel);

			GetMaterialData(pFbxMesh, pFBXModel);
		}
	}

	INT childNodes = pChildNode->GetChildCount();

	for (INT nodes = 0; nodes < childNodes; ++nodes)
	{
		GetMesh(pChildNode->GetChild(nodes), pFBXModel);
	}

	return;
}

FBXModel g_fBXModel;

BOOL FBX::Initialize()
{
	FbxManager* pFbxManager = FbxManager::Create();

	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");

	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "");

	pFbxImporter->Initialize("Handgun_fbx_6.1_ASCII.fbx");

	pFbxImporter->Import(pFbxScene);

	FbxNode* pRootNode = pFbxScene->GetRootNode();

	if (pRootNode)
	{
		INT childNodes = pRootNode->GetChildCount();

		for (INT nodes = 0; nodes < childNodes; ++nodes)
		{
			GetMesh(pRootNode->GetChild(nodes), &g_fBXModel);
		}
	}

	//importer->Destroy();

	return TRUE;
}

VOID FBX::DrawFBX(FBXModel* pFBXModel)
{
#define MY_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)

	CustomVertex cusV4Background[4] =
	{
		{ -1.f,   0 - 1.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ 1280 - 1.f,   0 - 1.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ 1280 - 1.f, 720 - 1.f, 0.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ -1.f, 720 - 1.f, 0.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	LPDIRECT3DDEVICE9& rDirectX3DDevice = DirectX::m_directXInstances.m_pDirectX3DDevice;

	rDirectX3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	rDirectX3DDevice->SetTexture(0, NULL);
	rDirectX3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, cusV4Background, sizeof(CustomVertex));

	DirectX::m_directXInstances.m_pDirectX3DDevice->SetFVF(MY_FVF);

	for (int i = 0; i < pFBXModel->m_materialCount; i++)
	{
		rDirectX3DDevice->SetMaterial(&pFBXModel->m_materialData[i]);
	}

	for (unsigned int n = 0; n < pFBXModel->m_pTextureData.size(); n++)
	{
		rDirectX3DDevice->SetTexture(n, pFBXModel->m_pTextureData[n]->m_pTexture);
	}

	Camera camera;

	camera.SetTransform();

	D3DXMATRIX			m_MatWorld;
	D3DXMatrixIdentity(&m_MatWorld);

	// 回転
	D3DXMATRIX			matHeading;
	D3DXMatrixRotationY(&matHeading, 0);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matHeading);

	// 移動
	D3DXMATRIX			matPosition;	// 位置座標行列
	D3DXMatrixTranslation(&matPosition, 0, 0, 0);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matPosition);
	rDirectX3DDevice->SetTransform(D3DTS_WORLD, &m_MatWorld);

	D3DXVECTOR3 vecDirection(0, 0, 1);
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;

	light.Specular.r = 1.f;
	light.Specular.b = 1.f;
	light.Specular.g = 1.f;

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);

	light.Range = 200.f;
	rDirectX3DDevice->SetLight(0, &light);
	rDirectX3DDevice->LightEnable(0, TRUE);

	rDirectX3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		pFBXModel->m_polygonCount,
		pFBXModel->m_pVertexBuffer,
		sizeof(Vertex));
}
