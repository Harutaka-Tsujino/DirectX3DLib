/**
* @file Vector.h
* @brief ベクトルが関係した計算を行う
* @author Harutaka Tsujino
* @date 15-10-2018
*/

#pragma once
#include <windows.h>	
#include <d3dx9.h>

namespace CalcVector
{
	/**
	* @fn VOID UnitVec2(D3DXVECTOR2* pStraightUnitVec, D3DXVECTOR2 destPt, D3DXVECTOR2 oriPt)
	* @brief 引数の2点から単位ベクトルを作成する
	* @param (pStraightUnitVec) 生成したベクトルを代入する
	* @param (destPt) 終点
	* @param (oriPt) 始点
	* @return なし
	* @detail 引数の2点からベクトルを作成、その後正規化を行い引数に代入する
	*/
	VOID UnitVec2(D3DXVECTOR2* pStraightUnitVec, D3DXVECTOR2 destPt, D3DXVECTOR2 oriPt);

	/**
	* @fn VOID NormalUnitVec2Y(D3DXVECTOR2* pNormalUnitVec, D3DXVECTOR2 baseVec)
	* @brief baseVecをY軸右向き90度回転した単位ベクトルを生成する
	* @param (pNormalUnitVec) 生成したベクトルを代入する
	* @param (baseVec) 90度回転させる基となるベクトル
	* @return なし
	* @detail 引数のベクトルを正規化、その後回転行列を生成した後
	* 正規化したベクトルと掛け合わせその結果を引数に代入する
	*/
	VOID NormalUnitVec2Y(D3DXVECTOR2* pNormalUnitVec, D3DXVECTOR2 baseVec);

	/**
	* @fn FLOAT Vec2Cross(D3DXVECTOR2 secondVec, D3DXVECTOR2 baseVec)
	* @brief 2次元ベクトルの外積計算をする
	* @param (secondVec) 基底となるベクトルに掛け合わせるベクトル
	* @param (baseVec) 外積の基底となるベクトル
	* @return 外積の計算結果
	* @detail baseVecを基底とした,2次元ベクトルの外積計算をする
	*/
	FLOAT Vec2Cross(D3DXVECTOR2 secondVec, D3DXVECTOR2 baseVec);

	/**
	* @fn FLOAT Vec2Rad(D3DXVECTOR2 secondVec, D3DXVECTOR2 baseVec)
	* @brief 引数の2次元ベクトルからなる角度の計算をする
	* @param (secondVec) 基底となるベクトルに掛け合わせるベクトル
	* @param (baseVec) 角度計算の基底となるベクトル
	* @return 計算結果の角度
	* @detail 内積を用い引数の2次元ベクトルからなる角度を算出、その後外積を用い符号をつける
	*/
	FLOAT Vec2Rad(D3DXVECTOR2 secondVec, D3DXVECTOR2 baseVec);
}
