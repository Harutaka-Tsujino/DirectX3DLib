#pragma once
#include<windows.h>
#include<d3dx9.h>
#include <dinput.h>
#include"DX9LibX64.h"

//入力デバイス
enum class INPUT_DEVICES
{
	KEY_BOARD,
	MOUSE,
	MAX_INPUT_DEVICES
};

typedef struct
{
	//現在でフレームのキーボードのキーが押されていた場合上位ビットが立つ
	BYTE m_diks[256];

	//前回のでフレームのキーボードのキーが押されていた場合上位ビットが立つ
	BYTE m_prevDiks[256];

	//キーボードのキーが押された瞬間のフレームだけTRUE
	BOOL m_keyPush[256];

	//キーボードのキーが押され続けられている間TRUE
	BOOL m_keyHold[256];

	//キーボードのキーが離された瞬間のフレームだけTRUE
	BOOL m_keyRelease[256];

	//キーボードのキーが前回と現在のフレームで押されていない間TRUE
	BOOL m_keyUninput[256];
}KeyBoardState;

//マウスのカーソルの位置ボタンの入力状態を保持する
typedef struct
{
	//現在のフレームでの前回のカーソル位置からの相対位置とボタンの入力状態
	DIMOUSESTATE m_directInputMouseState;

	//前回のフレームでの前回のカーソル位置からの相対位置とボタンの入力状態
	DIMOUSESTATE m_prevDirectInputMouseState;

	//カーソルの絶対座標
	POINT m_absolutePos;

	//マウスのボタンが押された瞬間のフレームだけTRUE
	BOOL m_buttonPush[4];

	//マウスのボタンが押され続けられている間TRUE
	BOOL m_buttonHold[4];

	//マウスのボタンが離された瞬間のフレームだけTRUE
	BOOL m_buttonRelease[4];

	//マウスのボタンが前回と現在のフレームで押されていない間TRUE
	BOOL m_buttonUninput[4];
}MouseState;

class DirectX
{
public:
	static DirectX* GetInstancePointer();
	VOID Init(HWND* hWnd, BOOL window, BOOL m_cullPolygon, DWORD d3DFVF);
	HRESULT InitDirectX3DDevice();
	HRESULT InitDirectXInputDevices();
	HRESULT InitDirectX();
	VOID GetInputStates();
	VOID UpdateInputStates();
	VOID PrepareRender();
	VOID CleanUpRender();

	BOOL m_window;
	KeyBoardState m_keyBoardState;
	MouseState m_mouseState;
	HWND* m_pHWnd;

private:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectX)
	VOID SetBuckBufferOverall();
	VOID SetRenderState();
	VOID SetTextureStageState();

	static DirectX* m_pDirectXClass;
	LPDIRECT3D9 m_pDirectX;
	LPDIRECT3DDEVICE9 m_pDirectX3DDevice;
	LPDIRECTINPUT8 m_pDirectXInput;
	LPDIRECTINPUTDEVICE8 m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MAX_INPUT_DEVICES];
	LPD3DXFONT m_pDirectXFont;
	D3DPRESENT_PARAMETERS m_DirectXPresentParam;
	BOOL m_cullPolygon;
	DWORD m_d3DFVF;
};

//ポリゴンの頂点情報
struct CustomVertex
{
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_z;
	DWORD m_aRGB;
	FLOAT m_tu;
	FLOAT m_tv;
};
