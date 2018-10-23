/**
* @file Draw.h
* @brief 描画クラス
* @author Harutaka Tsujino
* @date 18-10-2018
*/

#pragma once
#include <windows.h>
#include <map>
#include "../FBX/FbxRelated.h"
#include "../CustomVertices/CustomVertices.h"

struct CustomVertex;

/**
* @class Draw
* @brief オブジェクトの描画を行う
*/
class Draw
{
public:
	/**
	* @fnstatic Draw* GetInstance()
	* @brief singleton用のインスタンスを生成し、
	*	そのインスタンスのポインタを返す関数、
	*	すでに生成されていた場合そのインスタンスのポインタを返す
	* @return インスタンスのポインタ
	* @detail このクラスのメンバm_pDrawがNULLならnewを行い、
	*	m_pDrawに代入し戻り値で変えす、
	*	このクラスを破棄する場合はVOID DeleteInstance()を使用する
	*/
	static Draw* GetInstance();

	/**
	* @fnstatic  VOID DeleteInstance()
	* @brief このクラスを破棄する
	* @return なし
	* @detail このクラスのメンバm_pDrawをDeleteしてこのクラスを破棄し、NULLを代入する
	*/
	VOID DeleteInstance();

	/**
	* @fn static VOID SetLPDIRECT3DDEVICE9(LPDIRECT3DDEVICE9 pDirectX3DDevice)
	* @brief 描画に用いるDirectXの3Dデバイスを受け取る関数
	* @param (pDirectX3DDevice)　受け渡すDirectXの3Dデバイス
	* @return なし
	* @detail Drawクラスのメンバに引数で渡されたDirectXの3Dデバイスを渡す
	*/
	static VOID SetLPDIRECT3DDEVICE9(LPDIRECT3DDEVICE9 pDirectX3DDevice);

	/** 
	* @fn virtual VOID Render(const FbxRelated* pFBXModel,
	*	const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 texture = NULL)
	* @brief FBXの描画を行う
	* @param (pFBXModel) FBXのクラス モデルを読み込んだ後でないといけない
	* @param (pMatWorld) 拡大回転移動行列をまとめた行列のポインタ
	* @param (pTexture) モデルに張り付けるテクスチャ デフォルトで存在している場合はNULL
	* @return なし
	* @detail pMatWorldをSetTransformを行い,引数のテクスチャを設定した後
	*	pFBXModelのメンバ関数を用い描画する
	*/
	virtual VOID Render(const FbxRelated* pFBXModel,
		const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 pTexture = NULL);

	/**
	* @fn virtual VOID Render(const CustomVertex* pCustomVertices,
	*	const LPDIRECT3DTEXTURE9 pTexture = NULL)
	* @brief 2Dの描画を行う
	* @param (pCustomVertices) 頂点データの先頭ポインタ
	* @param (pTexture) ポリゴンに張り付けるテクスチャ
	* @return なし
	* @detail 引数のテクスチャを設定した後、DrawPrimitiveUpを用い描画する
	*/
	virtual VOID Render(const CustomVertex* pCustomVertices,
		const LPDIRECT3DTEXTURE9 pTexture = NULL);

	/**
	* @fn virtual VOID Render(const CustomVertex* pCustomVertices,
	*	const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 pTexture = NULL)
	* @brief 3Dの描画
	* @param (Custom3DVertex) 3D頂点データの先頭ポインタ
	* @param (pMatWorld) 拡大回転移動行列をまとめた行列のポインタ
	* @return なし
	* @detail pMatWorldをSetTransformを行い、virtual VOID Render(const CustomVertex* pCustomVertices,
	*　	const LPDIRECT3DTEXTURE9 pTexture = NULL)
	*/
	virtual VOID Render(const Custom3DVertex* pCustom3DVertices,
		const D3DXMATRIX* pMatWorld, const LPDIRECT3DTEXTURE9 pTexture = NULL);

protected:
	static LPDIRECT3DDEVICE9 m_pDirectX3DDevice;		//! DirectXの3Dデバイス

private:
	Draw() {};
	~Draw() {};

	static Draw* m_pDraw;		//! singleton用のこのクラスのポインタ
};
