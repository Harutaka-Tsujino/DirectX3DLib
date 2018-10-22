﻿/**
* @file		FbxModel.cpp
* @brief	FbxModelクラス実装
* @author	kawaguchi
*/

#include <fbxsdk.h>
#include <cstdio>
#include <cstdlib>
#include <crtdbg.h>
#include "FbxModel.h"
#include "../GameManager/DirectX/DirectX.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#pragma comment(lib,"libfbxsdk-md.lib")

FbxModel::FbxModel()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	m_pDevice = rpDirectX3DDevice;

	m_pFbxModelData = NULL;
}

FbxModel::~FbxModel()
{
}

VOID FbxModel::DrawFbx()
{
	m_pDevice->SetFVF(MY_FVF);

	for (int i = 0; i < m_pFbxModelData->materialCount; i++)
	{
		if (!m_pFbxModelData->MaterialData.size())continue;

		m_pDevice->SetMaterial(&m_pFbxModelData->MaterialData[i]);
	}

	for (unsigned int n = 0; n < m_pFbxModelData->pTextureData.size(); n++)
	{
		m_pDevice->SetTexture(n, m_pFbxModelData->pTextureData[n]->m_pTexture);
	}

	m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		m_pFbxModelData->polygonCount,
		m_pFbxModelData->pVertex,
		sizeof(Vertex));
}
