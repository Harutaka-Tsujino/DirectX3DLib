#pragma once
#include <windows.h>
#include <d3dx9.h>

/**
* @struct CustomVertex
* @brief 頂点データ
*/
struct CustomVertex
{
public:
	D3DXVECTOR3 m_pos;		//! 座標

	FLOAT m_rHW = 1.0f;		//! 重みの逆数 基本1

	DWORD m_aRGB;			//! 色カラーコードARGB

	D3DXVECTOR2 m_TexUV;	//! テクスチャの座標
};

/**
* @struct CustomVertex
* @brief 3D頂点データ
*/
struct Custom3DVertex
{
public:
	D3DXVECTOR3 m_pos;		//! 座標

	DWORD m_aRGB;			//! 色カラーコードARGB

	D3DXVECTOR2 m_TexUV;	//! テクスチャの座標
};

/**
* @class CustomVertices
* @brief 頂点情報の操作を行う
*/
class CustomVertices
{
public:
	/**
	* @fnstatic CustomVertices* GetInstance()
	* @brief singleton用のインスタンスを生成し、
	*	そのインスタンスのポインタを返す関数、
	*	すでに生成されていた場合そのインスタンスのポインタを返す
	* @return インスタンスのポインタ
	* @detail このクラスのメンバm_pCustomVerticesがNULLならnewを行い、
	*	m_pCustomVerticesに代入し戻り値で変えす、
	*	このクラスを破棄する場合はVOID DeleteInstance()を使用する
	*/
	static CustomVertices* GetInstance();

	/**
	* @fnstatic  VOID DeleteInstance()
	* @brief このクラスを破棄する
	* @return なし
	* @detail このクラスのメンバm_pCustomVerticesをDeleteしてこのクラスを破棄し、NULLを代入する
	*/
	VOID DeleteInstance();

	/**
	* @fn static VOID RotateXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3* pDegree, const D3DXVECTOR3* pRelativeRotateCenter)
	* @brief XYZ順に回転を行う
	* @param (pCustomVertices) 頂点データ配列の先頭アドレス
	* @param (pDegree) 回転をさせる度数法での角度のポインタ
	* @param (pRelativeRotateCenter) どれほど回転の中心が矩形の中心よりずれているかのポインタ
	* @return なし
	* @detail 回転行列を作成し、矩形の中心を求め回転の中心を原点に移動させ、回転行列を用いて回転を行い原点へ移動させた分元に戻す
	*/
	VOID RotateXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3* pDegree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateX(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateY(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateZ(CustomVertex* pCustomVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateXYZ(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pDegree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateX(Custom3DVertex* pCustom3DVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateY(Custom3DVertex* pCustom3DVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	VOID RotateZ(Custom3DVertex* pCustom3DVertices, FLOAT degree, const D3DXVECTOR3* pRelativeRotateCenter);

	/**
	* @fn VOID Rescale(CustomVertex* pCustomVertices, const D3DXVECTOR2* pScaleRate)
	* @brief 矩形を拡縮させる
	* @param (pCustomVertices) 頂点データ配列の先頭アドレス
	* @param (pScaleRate) 拡縮率のポインタ
	* @return なし
	* @detail 矩形の中心を求め幅と高さを割り出し、拡縮率を幅と高さに掛け合わせ、矩形の中心点から再構成させる
	*/
	VOID Rescale(CustomVertex* pCustomVertices, const D3DXVECTOR2* pScaleRate);

	VOID Rescale(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR2* pScaleRate);

	/**
	* @fn Move(CustomVertex* pCustomVertices, const D3DXVECTOR3* pMovement)
	* @brief 矩形を移動させる
	* @param (pCustomVertices) 頂点データ配列の先頭アドレス
	* @param (pMovement) 移動量のポインタ
	* @return なし
	*/
	VOID Move(CustomVertex* pCustomVertices, const D3DXVECTOR3* pMovement);

	VOID Move(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pMovement);

	/**
	* @fn Move(CustomVertex* pCustomVertices, const D3DXVECTOR3* pMovement)
	* @brief 矩形を引数の位置に再配置させる
	* @param (pCustomVertices) 頂点データ配列の先頭アドレス
	* @param (pCenter) 矩形を移動させる座標のポインタ
	* @return なし
	*/
	VOID Locale(CustomVertex* pCustomVertices, const D3DXVECTOR3* pPos);

	VOID Locale(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pPos);

	/**
	* @fn VOID SetTexUV(CustomVertex* pCustomVertices, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f)
	* @brief テクスチャ座標を引数の座標に変更させる
	* @param (pCustomVertices) 頂点データ配列の先頭アドレス
	* @param (startTU) x方向のテクスチャ座標の始まりの値
	* @param (startTV) y方向のテクスチャ座標の始まりの値
	* @param (endTU) x方向のテクスチャ座標の終わりの値
	* @param (endTV) y方向のテクスチャ座標の終わりの値
	* @return なし
	*/
	VOID SetTexUV(CustomVertex* pCustomVertices, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f);

	VOID SetTexUV(Custom3DVertex* pCustom3DVertices, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f);

	/**
	* @fn VOID Create(CustomVertex *pCustomVertices, const D3DXVECTOR3* pCenter, const D3DXVECTOR2* pHalfScale,
	*	DWORD color = 0xFFFFFFFF, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f)
	* @brief 頂点データ構造体に引数の値を代入させる
	* @param (pCustomVertices) 頂点データ配列の先頭アドレス
	* @param (pCenter) 矩形の中心のポインタ
	* @param (pHalfScale) 矩形の高さ幅の半分のポインタ
	* @param (color) 頂点の色カラーコードARGB
	* @param (startTU) x方向のテクスチャ座標の始まりの値
	* @param (startTV) y方向のテクスチャ座標の始まりの値
	* @param (endTU) x方向のテクスチャ座標の終わりの値
	* @param (endTV) y方向のテクスチャ座標の終わりの値
	* @return なし
	*/
	VOID Create(CustomVertex *pCustomVertices, const D3DXVECTOR3* pCenter, const D3DXVECTOR2* pHalfScale,
		DWORD color = 0xFFFFFFFF, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f);

	VOID Create(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pCenter, const D3DXVECTOR2* pHalfScale,
		DWORD color = 0xFFFFFFFF, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f);

private:
	CustomVertices() {};
	~CustomVertices() {};

	static CustomVertices* m_pCustomVertices;		//! singleton用のこのクラスのポインタ

	static const INT M_RECT_VERTICES_NUM = 4;		//! 矩形を構成する頂点の数

	VOID Rotate(CustomVertex* pCustomVertices, const D3DXVECTOR3* pRelativeRotateCenter, const D3DXMATRIX* pMatRotate);

	VOID Rotate(Custom3DVertex* pCustom3DVertices, const D3DXVECTOR3* pRelativeRotateCenter, const D3DXMATRIX* pMatRotate);
};
